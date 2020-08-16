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
    std::shared_ptr<Route> auxRoute;
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
    }
}


void DedicatedPathProtection::ResourceAlloc(CallDevices* call) {
    CheckProtectionAllocDisp();

}

