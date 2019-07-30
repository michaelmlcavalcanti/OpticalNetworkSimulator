/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ResourceDeviceAlloc.cpp
 * Author: brunovacorreia
 * 
 * Created on May 27, 2019, 10:41 PM
 */

#include "../../include/ResourceAllocation/ResourceDeviceAlloc.h"
#include "../../include/ResourceAllocation/Routing.h"
#include "../../include/ResourceAllocation/Resources.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/ResourceAllocation/SA.h"
#include "../../include/Calls/CallDevices.h"
#include "../../include/GeneralClasses/Def.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Structure/NodeDevices.h"

ResourceDeviceAlloc::ResourceDeviceAlloc(SimulationType *simulType)
:ResourceAlloc(simulType) {

}

ResourceDeviceAlloc::~ResourceDeviceAlloc() {

}

void ResourceDeviceAlloc::Load() {
    ResourceAlloc::Load();
}

void ResourceDeviceAlloc::ResourAlloc(Call* call) {
    CallDevices* callDev = dynamic_cast<CallDevices*>(call);
    
    if(options->GetRegenerationOption() != RegenerationDisabled)
        this->RoutingVirtRegSpecAlloc(callDev);
    else if(options->GetTransponderOption() == TransponderEnabled)
        this->RoutingTranspSpecAlloc(callDev);
    
    if(call->GetStatus() == NotEvaluated)
        call->SetStatus(Blocked);
}

void ResourceDeviceAlloc::RoutingVirtRegSpecAlloc(CallDevices* call) {
    this->routing->RoutingCall(call);
    
    //Tuple with route index and set of subRoutes index.
    std::vector<std::tuple<unsigned, unsigned>> routeSubIndexes(0);
    
    this->OrderRegenerationOptions(call, routeSubIndexes);
    
    for(unsigned int a = 0; a < routeSubIndexes.size(); a++){
        call->SetRoute(std::get<0>(routeSubIndexes.at(a)));
        unsigned int regOptionIndex = std::get<1>(routeSubIndexes.at(a));
        call->CreateTranspSegments(resources->GetRoutesTranspSegments(call, 
        regOptionIndex));
        
        if(!topology->CheckInsertFreeRegenerators(call))
            continue;
        call->SetTranspSegModulation(resources->GetTranspSegmentsModulation(
        call, regOptionIndex));
        this->modulation->SetModulationParam(call);
        
        if(!this->CheckOSNR(call))
            continue;
        
        this->specAlloc->SpecAllocation(call);
        
        if(topology->IsValidLigthPath(call)){
            call->ClearTrialRoutes();
            call->SetStatus(Accepted);
            call->SetUseRegeneration();
            break;
        }
    }
}

void ResourceDeviceAlloc::RoutingTranspSpecAlloc(CallDevices* call) {
    this->routing->RoutingCall(call);
    unsigned int numRoutes = call->GetNumRoutes();
    
    for(unsigned int a = 0; a < numRoutes; a++){
        call->ClearTransponders();
        call->SetRoute(call->GetRoute(a));
        call->SetModulation(resources->GetTranspSegmentsModulation(call, 0)
        .front());
        this->modulation->SetModulationParam(call);
            
        if(!topology->CheckInsertFreeBVTs(call))
            continue;
            
        if(!this->CheckOSNR(call))
            continue;
            
        this->specAlloc->SpecAllocation(call);
            
        if(this->topology->IsValidLigthPath(call)){
            call->SetStatus(Accepted);
            break;
        }
    }
    call->ClearTrialRoutes();
}

void ResourceDeviceAlloc::OrderRegenerationOptions(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    vec.clear();
    
    switch(options->GetRegAssOption()){
        case RegAssMinReg:
            this->SetMinRegChoiceOrder(call, vec);
            break;
        case RegAssMaxReg:
            this->SetMinSlotsChoiceOrder(call, vec);
            break;
        case DRE2BR:
        case SCRA:
            this->SetCostMetric(call, vec);
            break;
        default:
            std::cerr << "Invalid regenerator assignment option" << std::endl;
            std::abort();
    }
}

void ResourceDeviceAlloc::SetMinRegChoiceOrder(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    std::vector<std::vector<unsigned>> vecNumReg = 
    resources->GetNumberRegSet(call);
    std::vector<std::vector<unsigned>> vecNumSlots = 
    resources->GetNumberSlotsSet(call);
    unsigned int posSize = 0;
    unsigned int auxRouteIndex = 0;
    unsigned int auxIndex = 0;
    unsigned int auxRegInt;
    unsigned int auxSlotsInt;
    
    for(unsigned int a = 0; a < vecNumReg.size(); a++){
        posSize += vecNumReg.at(a).size();
    }
    
    while(vec.size() < posSize){
        auxRegInt = Def::Max_UnInt;
        auxSlotsInt = Def::Max_UnInt;
        
        for(unsigned int a = 0; a < vecNumReg.size(); a++){
            for(unsigned int b = 0; b < vecNumReg.at(a).size(); b++){
                
                if(auxRegInt >= vecNumReg.at(a).at(b)){
                    if(auxRegInt == vecNumReg.at(a).at(b) && 
                    auxSlotsInt <= vecNumSlots.at(a).at(b)){
                        continue;
                    }
                    else{
                        auxRegInt = vecNumReg.at(a).at(b);
                        auxSlotsInt = vecNumSlots.at(a).at(b);
                        auxRouteIndex = a;
                        auxIndex = b;
                    }
                }
            }
        }
        vec.push_back(std::make_tuple(auxRouteIndex, auxIndex));
        vecNumReg.at(auxRouteIndex).at(auxIndex) = Def::Max_UnInt;
        vecNumSlots.at(auxRouteIndex).at(auxIndex) = Def::Max_UnInt;
    }
}

void ResourceDeviceAlloc::SetMinSlotsChoiceOrder(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    std::vector<std::vector<unsigned>> vecNumReg = 
    resources->GetNumberRegSet(call);
    std::vector<std::vector<unsigned>> vecNumSlots = 
    resources->GetNumberSlotsSet(call);
    std::vector<std::vector<std::vector<TypeModulation>>> modulations =
    resources->GetSetsTranpSegmentsModulation(call);
    std::vector<std::vector<std::vector<unsigned int>>> numSlots =
    resources->GetSetsTranspSegmentsNumSlots(call);
    unsigned int posSize = 0;
    unsigned int auxRouteIndex = 0;
    unsigned int auxIndex = 0;
    unsigned int auxNumReg;
    unsigned int auxNumSlots;
    
    for(unsigned int a = 0; a < vecNumReg.size(); a++){
        posSize += vecNumReg.at(a).size();
    }
    
    while(vec.size() < posSize){
        auxNumReg = Def::Max_UnInt;
        auxNumSlots = Def::Max_UnInt;
        
        for(unsigned int a = 0; a < vecNumReg.size(); a++){
            for(unsigned int b = 0; b < vecNumReg.at(a).size(); b++){
                
                if(auxNumSlots >= vecNumSlots.at(a).at(b)){
                    if(auxNumSlots == vecNumSlots.at(a).at(b) && 
                    auxNumReg <= vecNumReg.at(a).at(b)){
                        continue;
                    }
                    else{
                        auxNumSlots = vecNumSlots.at(a).at(b);
                        auxNumReg = vecNumReg.at(a).at(b);
                        auxRouteIndex = a;
                        auxIndex = b;
                    }
                }
            }
        }
        vec.push_back(std::make_tuple(auxRouteIndex, auxIndex));
        vecNumReg.at(auxRouteIndex).at(auxIndex) = Def::Max_UnInt;
        vecNumSlots.at(auxRouteIndex).at(auxIndex) = Def::Max_UnInt;
    }
}

void ResourceDeviceAlloc::SetCostMetric(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
        std::vector<std::vector<unsigned>> vecNumReg = 
    resources->GetNumberRegSet(call);
    std::vector<std::vector<double>> vecCosts(0);
    
    //Calculate the cost of each possible regeneration option.
    vecCosts.resize(vecNumReg.size());
    for(unsigned a = 0; a < vecNumReg.size(); a++){
        vecCosts.at(a).resize(vecNumReg.at(a).size());
        
        for(unsigned b = 0; b < vecNumReg.at(a).size(); b++){
            vecCosts.at(a).at(b) = this->CalcTupleCost(call, a, b);
        }
    }
    
    unsigned int auxRouteIndex = 0;
    unsigned int auxIndex = 0;
    double bestCost;
    bool foundOption = true;
    
    //Order the regeneration options, based on the cost.
    while(foundOption){
        foundOption = false;
        bestCost = Def::Max_Double;
        
        for(unsigned a = 0; a < vecCosts.size(); a++){
            for(unsigned b = 0; b < vecCosts.at(a).size(); b++){
                
                if(vecCosts.at(a).at(b) < bestCost){
                    bestCost = vecCosts.at(a).at(b);
                    auxRouteIndex = a;
                    auxIndex = b;
                    foundOption = true;
                }
            }
        }
        
        if(foundOption){
            vec.push_back(std::make_tuple(auxRouteIndex, auxIndex));
            vecCosts.at(auxRouteIndex).at(auxIndex) = Def::Max_Double;
        }
    }
}

double ResourceDeviceAlloc::CalcTupleCost(CallDevices* call, 
unsigned routeIndex, unsigned subRouteIndex) {
    double tupleCost = 0.0;
    
    switch(options->GetRegAssOption()){
        case DRE2BR:
            tupleCost = this->DRE2BR_Cost(call, routeIndex, subRouteIndex);
            break;
        case SCRA:
            tupleCost = this->SCRA_Cost(call, routeIndex, subRouteIndex);
            break;
        default:
            std::cerr << "Invalid regenerator assignment option" << std::endl;
            std::abort();
    }
    
    return tupleCost;
}

double ResourceDeviceAlloc::DRE2BR_Cost(CallDevices* call, 
unsigned routeIndex, unsigned subRouteIndex) {
    double cost = 0.0;
    double LU = 0.0, TU = 0.0;
    unsigned int totalNumSlots = 0;
    unsigned int usedNumSlots = 0;
    unsigned int totalNumReg = 0;
    unsigned int usedNumReg = 0;
    const double alpha = 0.5;
    unsigned int numReg = resources->GetNumberReg(call, routeIndex,
                                                        subRouteIndex);
    unsigned int numSlots = resources->GetNumberSlots(call, routeIndex,
                                                            subRouteIndex);
    std::vector<std::shared_ptr<Route>> vecRoutes = 
    resources->GetRoutesTranspSegments(call, routeIndex, subRouteIndex);
    std::shared_ptr<Route> auxRoute;
    NodeDevices* auxNode;
    
    for(unsigned int a = 0; a < vecRoutes.size(); a++){
        auxRoute = vecRoutes.at(a);
        usedNumSlots += topology->GetNumUsedSlots(auxRoute.get());
        totalNumSlots += topology->GetNumSlots(auxRoute.get());
        
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

double ResourceDeviceAlloc::SCRA_Cost(CallDevices* call, unsigned routeIndex, 
unsigned subRouteIndex) {
    std::shared_ptr<Route> auxRoute;
    NodeDevices* auxNode;
    double totalCost = 0.0, cost = 0.0;
    double alpha = -0.05;
    double totalNumLinks = 0.0;
    double auxNumLinks;
    double totalNumSlots = (double) this->GetN(call);
    double auxNumSlots;
    double totalFreeReg;
    double numUsedReg;
    std::vector<std::shared_ptr<Route>> vecSubRoutes = 
    resources->GetRoutesTranspSegments(call, routeIndex, subRouteIndex);
    std::vector<unsigned> vecNumSlots = 
    resources->GetNumSlotsPerTranspSegments(call, routeIndex, subRouteIndex);
    
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
            auxNumSlots = (double) vecNumSlots.at(ind);
            cost += (auxNumSlots*auxNumLinks)/(totalNumSlots*totalNumLinks);
        }
        else{
            totalCost = Def::Max_Double;
            break;
        }
        
        if(ind != vecSubRoutes.size() - 1){
            numUsedReg = (double) NodeDevices::GetNumRegRequired(call->
                                                                 GetBitRate());
            totalFreeReg = (double) auxNode->GetNumFreeRegenerators();
            cost += (numUsedReg/totalFreeReg);
        }
        
        totalCost += cost;
    }
    
    return totalCost;
}

unsigned int ResourceDeviceAlloc::GetN(CallDevices* call) {
    double bandwidth = 0.0;
    unsigned int numSlots = 0;
    
    bandwidth = modulation->BandwidthQAM(FirstModulation, call->GetBitRate());
    if(modulation->isEON())
        numSlots = std::ceil(bandwidth/modulation->GetSlotBandwidth());
    else
        numSlots = 1;
    
    return numSlots;
}

bool ResourceDeviceAlloc::CheckOSNR(CallDevices* call) {
    std::vector<Call*> calls = call->GetTranspSegments();
    
    if(phyLayerOption == PhyLayerEnabled){
        if(!calls.empty()){
            for(auto it: calls){
                
                if(!topology->CheckOSNR(it->GetRoute(), it->GetOsnrTh()))
                    return false;
            }
        }
        else{            
            if(!ResourceAlloc::CheckOSNR(call))
                return false;
        }
    }
    
    return true;
}
