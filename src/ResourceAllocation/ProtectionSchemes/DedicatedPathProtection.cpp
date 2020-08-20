/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DedicatedPathProtection.cpp
 * Author: henrique
 * 
 * Created on October 26, 2019, 9:36 AM
 */

#include <vector>
#include <memory>

#include "../../../include/ResourceAllocation/ProtectionSchemes/DedicatedPathProtection.h"
#include "../../../include/ResourceAllocation/SA.h"
#include "../../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../../include/ResourceAllocation/Routing.h"
#include "../../../include/ResourceAllocation/Modulation.h"


DedicatedPathProtection::DedicatedPathProtection(ResourceDeviceAlloc* rsa) :
ProtectionScheme(rsa) {
    
}

DedicatedPathProtection::~DedicatedPathProtection() {
    
}

void DedicatedPathProtection::CreateProtectionRoutes() {
    routing->ProtectionDisjointYEN();
    /*std::shared_ptr<Route> auxRoute;
    unsigned int orN, deN; 
    resources->protectionAllRoutes.resize(resources->allRoutes.size());
    
    //creating protectionAllroutes vector (prot.) from allRoutes vector (work)
    for (int it = 0; it < resources->allRoutes.size(); it++){        
        
       for (int k = 0 ; this->resources->allRoutes.at(it).size(); k++){
            auxRoute = resources->allRoutes.at(it).at(k);
            orN = resources->allRoutes.at(it).at(k).get()->GetPath().front();
            deN = resources->allRoutes.at(it).at(k).get()->GetPath().back();
            
            if(auxRoute != nullptr){   
                resDevAlloc->DisableRouteLinks(auxRoute.get());
                resources->protectionAllRoutes.at(it).at(k) = 
                routing->Dijkstra(orN , deN);
                topology->SetAllLinksWorking();   
            } 
        } 
    }*/
}


void DedicatedPathProtection::ResourceAlloc(CallDevices* call) {
    unsigned int numRoutes = this->call->GetNumRoutes();
    std::shared_ptr<Call> callWork = protectionCalls.front();
    std::shared_ptr<Call> callBackup = protectionCalls.back();
    unsigned int orN = this->call->GetOrNode()->GetNodeId();
    unsigned int deN = this->call->GetDeNode()->GetNodeId();
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int nodePairIndex = orN * numNodes + deN;
    //std::deque<std::shared_ptr<Route>> trialProtRoutes;
    //std::vector<std::shared_ptr<Route>> trialProtRoutes = resources->protectionRoutes.at(nodePairIndex).at(numRoutes);
    bool allocFound = false;
    const unsigned int topNumSlots = topology->GetNumSlots();
    std::vector<unsigned int> possibleSlots(0);
    possibleSlots = this->resDevAlloc->specAlloc->SpecAllocation();
    unsigned int auxSlot;
    
    for(unsigned int a = 0; a < numRoutes; a++){
        callWork->SetRoute(this->call->GetRoute(a));
        callWork->SetModulation(this->call->GetModulation(a));
        //this->modulation->SetModulationParam(callWork);        
        
        for(unsigned int b = 0; b < numProtRoutes; b++) {
            callBackup->SetRoute(resources->protectionRoutes.at(nodePairIndex).at(a).at(b));
            callBackup->SetModulation(this->call->GetModulation(b));
           // this->modulation->SetModulationParam(callBackup);
            
            for(unsigned int c = 0; c < possibleSlots.size(); c++){
                auxSlot = possibleSlots.at(c);
                
                if(auxSlot + callWork->GetNumberSlots() - 1 >= topNumSlots)
                    break;
                else if(auxSlot + callBackup->GetNumberSlots() - 1 >= topNumSlots)
                    continue;
                
                //if(!this->resDevAlloc->CheckOSNR(callWork))
               //    break;
               // else if(!this->resDevAlloc->CheckOSNR(callBackup))
               //     continue;
                
                if(resDevAlloc->CheckSlotsDisp(callWork->GetRoute(), auxSlot, 
                auxSlot + callWork->GetNumberSlots() - 1) && 
                resDevAlloc->CheckSlotsDisp(callBackup->GetRoute(), auxSlot, 
                auxSlot + callBackup->GetNumberSlots() - 1)){                        
                    callWork->SetFirstSlot(auxSlot);
                    callWork->SetLastSlot(auxSlot + callWork->GetNumberSlots() - 1);
                    callBackup->SetFirstSlot(auxSlot);
                    callBackup->SetLastSlot(auxSlot + callBackup->GetNumberSlots() - 1);
                    this->call->ClearTrialModulations();
                    this->call->ClearTrialRoutes();
                    this->call->SetStatus(Accepted);
                    allocFound = true;
                    break;
                }
                
                if(allocFound)
                    break;
            }
            
            if(allocFound)
                    break;
        }
        if(allocFound)
            break;    
    }
    
}
