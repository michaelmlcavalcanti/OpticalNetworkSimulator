/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SCRA.cpp
 * Author: brunovacorreia
 * 
 * Created on August 26, 2019, 2:42 PM
 */

#include "../../../include/ResourceAllocation/RegeneratorAssignment/SCRA.h"

SCRA::SCRA(ResourceDeviceAlloc* resDevAlloc)
:RegeneratorAssignment(resDevAlloc) {
    
    switch(resDevAlloc->options->GetRegAssOption()){
        case RegAssSCRA1:
            alpha = -0.05;
            constSlot = 1.0;
            constReg = 1.0;
            break;
        case RegAssSCRA2:
            alpha = 0.0;
            constSlot = 1.0;
            constReg = 1.0;
            break;
        case RegAssSCRA3:
            alpha = -1.0;
            constSlot = 1E-5;
            constReg = 0.0;
            break;
        case RegAssSCRA4:
            alpha = 0.0;
            constSlot = 1.0;
            constReg = -1E-5;
            break;
        case RegAssSCRA5:
            alpha = 0.0;
            constSlot = 1.0;
            constReg = 1E-5;
            break;
        default:
            std::cerr << "Invalid SCRA type" << std::endl;
            std::abort();
    }
}

SCRA::~SCRA() {
    
}

void SCRA::ResourceAlloc(CallDevices* call) {
    resDevAlloc->RoutingOffVirtRegSpecAlloc(call);
}

double SCRA::CalcTupleCost(CallDevices* call, unsigned routeIndex, 
unsigned subRouteIndex) {
    std::shared_ptr<Route> auxRoute;
    NodeDevices* auxNode;
    double totalCost = 0.0, cost = 0.0;
    double totalNumLinks = 0.0;
    double auxNumLinks;
    double totalNumSlots = (double) this->GetN(call);
    double auxNumSlots;
    double totalFreeReg;
    double numUsedReg;
    std::vector<std::shared_ptr<Route>> vecSubRoutes = 
    resDevAlloc->resources->GetRoutesTranspSegments(call, routeIndex, 
                                                    subRouteIndex);
    std::vector<unsigned> vecNumSlots = 
    resDevAlloc->resources->GetNumSlotsPerTranspSegments(call, routeIndex,
                                                         subRouteIndex);
    
    for(unsigned int a = 0; a < vecSubRoutes.size(); a++){
        totalNumLinks += (double) vecSubRoutes.at(a)->GetNumHops();
    }
    
    for(unsigned int ind = 0; ind < vecSubRoutes.size(); ind++){
        cost = 0.0;
        cost += alpha;
        
        auxRoute = vecSubRoutes.at(ind);
        auxNode = dynamic_cast<NodeDevices*>(auxRoute->GetDeNode());
        
        if(auxNode->isThereFreeRegenerators(call->GetBitRate()) ||
           ind == vecSubRoutes.size() - 1){
            auxNumLinks = (double) auxRoute->GetNumHops();
            auxNumSlots = ((double) vecNumSlots.at(ind)) / auxNumLinks;
            cost += (constSlot*(auxNumSlots*auxNumLinks))/
                    (totalNumSlots*totalNumLinks);
        }
        else{
            totalCost = Def::Max_Double;
            break;
        }
        
        if(ind != vecSubRoutes.size() - 1){
            numUsedReg = (double) NodeDevices::GetNumRegRequired(call->
                                                                 GetBitRate());
            totalFreeReg = (double) auxNode->GetNumFreeRegenerators();
            cost += constReg*(numUsedReg/totalFreeReg);
        }
        
        totalCost += cost;
    }
    
    return totalCost;
}
