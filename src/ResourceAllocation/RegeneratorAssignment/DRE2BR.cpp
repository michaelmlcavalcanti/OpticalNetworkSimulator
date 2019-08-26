/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DRE2BR.cpp
 * Author: brunovacorreia
 * 
 * Created on August 26, 2019, 10:34 AM
 */

#include "../../../include/ResourceAllocation/RegeneratorAssignment/DRE2BR.h"

DRE2BR::DRE2BR(ResourceDeviceAlloc* resDevAlloc)
:RegeneratorAssignment(resDevAlloc) {
    
}

DRE2BR::~DRE2BR() {
    
}

void DRE2BR::ResourceAlloc(CallDevices* call) {
    resDevAlloc->RoutingOffVirtRegSpecAlloc(call);
}

double DRE2BR::CalcTupleCost(CallDevices* call, unsigned routeIndex, 
unsigned subRouteIndex) {
    double cost = 0.0;
    double LU = 0.0, TU = 0.0;
    unsigned int totalNumSlots = 0;
    unsigned int usedNumSlots = 0;
    unsigned int totalNumReg = 0;
    unsigned int usedNumReg = 0;
    const double alpha = 0.5;
    unsigned int numReg = resDevAlloc->resources->GetNumberReg(call, routeIndex,
                                                        subRouteIndex);
    unsigned int numSlots = resDevAlloc->resources->GetNumberSlots(call, 
    routeIndex, subRouteIndex);
    std::vector<std::shared_ptr<Route>> vecRoutes = 
    resDevAlloc->resources->GetRoutesTranspSegments(call, routeIndex, 
    subRouteIndex);
    std::shared_ptr<Route> auxRoute;
    NodeDevices* auxNode;
    
    for(unsigned int a = 0; a < vecRoutes.size(); a++){
        auxRoute = vecRoutes.at(a);
        usedNumSlots += resDevAlloc->topology->GetNumUsedSlots(auxRoute.get());
        totalNumSlots += resDevAlloc->topology->GetNumSlots(auxRoute.get());
        
        if(a != vecRoutes.size()-1){
            auxNode = dynamic_cast<NodeDevices*>(auxRoute->GetDeNode());
            usedNumReg += auxNode->GetNumOccRegenerators();
            totalNumReg += auxNode->GetNumRegenerator();
        }
    }
    
    if(totalNumReg == 0)
        totalNumReg = 1;
    
    LU = (double) usedNumSlots / (double) totalNumSlots;
    TU = (double) usedNumReg / (double) totalNumReg;
    cost = (1-alpha)*((double) numReg + (double) numSlots/3) + 
           (alpha)*(LU + TU);
    
    return cost;
}
