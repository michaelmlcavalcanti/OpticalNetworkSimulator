/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SCRA2.cpp
 * Author: brunovacorreia
 * 
 * Created on October 15, 2019, 8:11 PM
 */

#include "../../../include/ResourceAllocation/RegeneratorAssignment/SCRA2.h"

SCRA2::SCRA2(ResourceDeviceAlloc* resDevAlloc, double alpha, double constSlot, 
double constReg, double constNumForms):SCRA(resDevAlloc, alpha, constSlot, 
constReg), constNumForms(constNumForms) {

}

SCRA2::SCRA2(ResourceDeviceAlloc* resDevAlloc, std::ifstream& file)
:SCRA(resDevAlloc, file) {
    double auxDouble;
    
    file >> auxDouble;
    this->SetConstNumForms(auxDouble);
}

SCRA2::~SCRA2() {

}

double SCRA2::CalcTupleCost(CallDevices* call, unsigned routeIndex, 
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
    //double totalNumForms
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
            
            if(numUsedReg > totalFreeReg){
                totalCost = Def::Max_Double;
                break;
            }
            cost += constReg*(numUsedReg/totalFreeReg);
        }
        
        totalCost += cost;
    }
    
    return totalCost;
}

void SCRA2::SetConstNumForms(double constNumForms) {
    this->constNumForms = constNumForms;
}
