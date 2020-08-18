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
    //unsigned int orN = callWork.get();
    //std::shared_ptr<Node> deN = callWork->GetDeNode();
    //unsigned int numNodes = this->topology->GetNumNodes();
    //unsigned int nodePairIndex = orN * numNodes + deN;
    //std::deque<std::shared_ptr<Route>> trialProtRoutes;
    //std::vector<std::shared_ptr<Route>> trialProtRoutes = resources->protectionRoutes.at(nodeIndex).at(numRoutes);
    
    for(unsigned int a = 0; a < numRoutes; a++){
        callWork->SetRoute(this->call->GetRoute(a));
        callWork->SetModulation(this->call->GetModulation(a));
        //this->modulation->SetModulationParam(callWork);
        
        
        for(unsigned int b = 0; b < numProtRoutes; b++) {
            callBackup->SetRoute(this->call->GetRoute(b));
            callBackup->SetModulation(this->call->GetModulation(b));
           // this->modulation->SetModulationParam(callBackup);
        }    
        
    }
} 


