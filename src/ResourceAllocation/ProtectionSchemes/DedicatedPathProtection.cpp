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
#include "../../../include/Calls/CallDevices.h"
#include "../../../include/Data/Parameters.h"
#include "math.h" 


DedicatedPathProtection::DedicatedPathProtection(ResourceDeviceAlloc* rsa) :
ProtectionScheme(rsa) {
    
}

DedicatedPathProtection::~DedicatedPathProtection() {
    
}


void DedicatedPathProtection::CreateProtectionRoutes() {
    routing->ProtectionDisjointYEN(); 
}

void DedicatedPathProtection::CreateProtectionCalls(CallDevices* call) {
    std::shared_ptr<Call> auxCall;
    numProtRoutes = 2;
    
    for(unsigned a = 1; numProtRoutes; a++){
        auxCall = std::make_shared<Call>(call->GetOrNode(), 
        call->GetDeNode(), call->GetBitRate(), call->GetDeactivationTime());
        calldevices->GetTranspSegmentsVec().push_back(auxCall);
            if(parameters->GetBeta() != 0){
                double protBitRate = ceil ((1 - parameters->GetBeta()) * 
                call->GetBitRate());
                call->SetBitRate(protBitRate);
            }  
    }
    double workBitRate = calldevices->GetTranspSegmentsVec().front()->GetBitRate();
    call->SetBitRate(workBitRate);    
}

void DedicatedPathProtection::ResourceAlloc(CallDevices* call) {
    resDevAlloc->RoutingContProtectionSpecAlloc(call);
 /*   std::shared_ptr<Call> callWork = protectionCalls.front();
    std::shared_ptr<Call> callBackup = protectionCalls.back();    
    unsigned int numRoutes = call->GetNumRoutes();
    unsigned int orN = call->GetOrNode()->GetNodeId();
    unsigned int deN = call->GetDeNode()->GetNodeId();
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int nodePairIndex = orN * numNodes + deN;
    bool allocFound = false;
    const unsigned int topNumSlots = topology->GetNumSlots();
    std::vector<unsigned int> possibleSlots(0);
    possibleSlots = this->resDevAlloc->specAlloc->SpecAllocation();
    unsigned int auxSlot;
    
    for(unsigned int a = 0; a < numRoutes; a++){        
        callWork->SetRoute(this->call->GetRoute(a));
        callWork->SetModulation(this->call->GetModulation(a));
        //this->modulation->SetModulationParam(callWork);        
        
        for(unsigned int b = 0; b < resources->protectionRoutes.at
            (nodePairIndex).at(a).size(); b++) {            
            callBackup->SetRoute(resources->protectionRoutes.at
            (nodePairIndex).at(a).at(b));
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
                    call->ClearTrialModulations();
                    call->ClearTrialRoutes();
                    call->SetStatus(Accepted);
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
    }*/
    
}
