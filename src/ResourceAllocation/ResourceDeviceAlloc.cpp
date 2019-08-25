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
        this->RoutingTransponderSpecAlloc(callDev);
    
    if(call->GetStatus() == NotEvaluated)
        call->SetStatus(Blocked);
}

void ResourceDeviceAlloc::RoutingVirtRegSpecAlloc(CallDevices* call) {
    
    switch(options->GetRegAssOption()){
        case RegAssFLRonline:
        case RegAssFNS:
            this->RoutingOnVirtRegSpecAlloc(call);
            break;
        default:
            this->RoutingOffVirtRegSpecAlloc(call);
    }
}

void ResourceDeviceAlloc::RoutingOffVirtRegSpecAlloc(CallDevices* call) {
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

void ResourceDeviceAlloc::RoutingOnVirtRegSpecAlloc(CallDevices* call) {
    this->routing->RoutingCall(call);
    
    unsigned int numRoutes = call->GetNumRoutes();
    std::vector<std::shared_ptr<Route>> vecRoutes(0);
    std::vector<TypeModulation> vecModulation(0);
    
    for(unsigned int routeIndex = 0; routeIndex < numRoutes; routeIndex++){
        call->SetRoute(call->GetRoute(routeIndex));
        
        if(this->CreateRegOption(call, routeIndex, vecRoutes, vecModulation)){
            call->CreateTranspSegments(vecRoutes);
            
            if(!topology->CheckInsertFreeRegenerators(call))
                continue;
            call->SetTranspSegModulation(vecModulation);
            this->modulation->SetModulationParam(call);
            
            //Retirar???
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
}

void ResourceDeviceAlloc::RoutingTransponderSpecAlloc(CallDevices* call) {
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
        case RegAssMinSlotsMinReg:
            this->SetMinSlotsChoiceOrder(call, vec);
            break;
        case RegAssMinSlotsMaxReg:
            this->SetMinSlotsMaxRegChoiceOrder(call, vec);
            break;
        case RegAssMaxReg:
            this->SetMaxRegChoiceOrder(call, vec);
            break;
        case RegAssFLR:
            this->SetRegChoiceOrderFLR(call, vec);
            break;
        case RegAssDRE2BR:
        case RegAssSCRA1:
        case RegAssSCRA2:
        case RegAssSCRA3:
        case RegAssSCRA4:
        case RegAssSCRA5:
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

void ResourceDeviceAlloc::SetMinSlotsMaxRegChoiceOrder(CallDevices* call,
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    std::vector<std::vector<unsigned>> vecNumReg = 
    resources->GetNumberRegSet(call);
    std::vector<std::vector<unsigned>> vecNumSlots = 
    resources->GetNumberSlotsSet(call);
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
                    auxNumReg >= vecNumReg.at(a).at(b)){
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

void ResourceDeviceAlloc::SetMaxRegChoiceOrder(CallDevices* call, 
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
        auxRegInt = 0;
        auxSlotsInt = Def::Max_UnInt;
        
        for(unsigned int a = 0; a < vecNumReg.size(); a++){
            for(unsigned int b = 0; b < vecNumReg.at(a).size(); b++){
                
                if(auxRegInt <= vecNumReg.at(a).at(b)){
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
        vecNumReg.at(auxRouteIndex).at(auxIndex) = 0;
        vecNumSlots.at(auxRouteIndex).at(auxIndex) = Def::Max_UnInt;
    }
}

void ResourceDeviceAlloc::SetRegChoiceOrderFLR(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    std::vector<std::vector<std::vector<std::shared_ptr<Route>>>> vecRoutes =
    resources->GetRoutesTranspSegments(call);
    
    unsigned int sizeRoutes = vecRoutes.size();
    unsigned int auxRegOpIndex;
    
    for(unsigned routeIndex = 0; routeIndex < sizeRoutes; routeIndex++){
        
        while(!vecRoutes.at(routeIndex).empty()){
            auxRegOpIndex = vecRoutes.at(routeIndex).size() - 1;
            vec.push_back(std::make_tuple(routeIndex, auxRegOpIndex));
            vecRoutes.at(routeIndex).pop_back();
        }
    }
}

void ResourceDeviceAlloc::SetRegChoiceOrderFNS(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    std::vector<std::vector<std::vector<std::shared_ptr<Route>>>> vecRoutes =
    resources->GetRoutesTranspSegments(call);
    std::vector<std::vector<std::vector<TypeModulation>>>
    vecModulation = resources->GetSetsTranpSegmentsModulation(call);
    unsigned int numPos = 0;
    
    for(auto routeIt: vecRoutes){
        numPos += routeIt.size();
    }
    
    while(vec.size() < numPos){
        
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
        case RegAssDRE2BR:
            tupleCost = this->DRE2BR_Cost(call, routeIndex, subRouteIndex);
            break;
        case RegAssSCRA1:
            tupleCost = this->SCRA1_Cost(call, routeIndex, subRouteIndex);
            break;
        case RegAssSCRA2:
            tupleCost = this->SCRA2_Cost(call, routeIndex, subRouteIndex);
            break;
        case RegAssSCRA3:
            tupleCost = this->SCRA3_Cost(call, routeIndex, subRouteIndex);
            break;
        case RegAssSCRA4:
            tupleCost = this->SCRA4_Cost(call, routeIndex, subRouteIndex);
            break;
        case RegAssSCRA5:
            tupleCost = this->SCRA5_Cost(call, routeIndex, subRouteIndex);
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

double ResourceDeviceAlloc::SCRA1_Cost(CallDevices* call, unsigned routeIndex, 
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
            auxNumSlots = ((double) vecNumSlots.at(ind)) / auxNumLinks;
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

double ResourceDeviceAlloc::SCRA2_Cost(CallDevices* call, unsigned routeIndex, 
unsigned subRouteIndex) {
    std::shared_ptr<Route> auxRoute;
    NodeDevices* auxNode;
    double totalCost = 0.0, cost = 0.0;
    double alpha = 0.0;
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
            auxNumSlots = ((double) vecNumSlots.at(ind)) / auxNumLinks;
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

double ResourceDeviceAlloc::SCRA3_Cost(CallDevices* call, unsigned routeIndex, 
unsigned subRouteIndex) {
    std::shared_ptr<Route> auxRoute;
    NodeDevices* auxNode;
    double totalCost = 0.0, cost = 0.0;
    double alpha = -1.0;
    double epslon = 1E-6;
    double totalNumLinks = 0.0;
    double auxNumLinks;
    double totalNumSlots = (double) this->GetN(call);
    double auxNumSlots;
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
            auxNumSlots = ((double) vecNumSlots.at(ind)) / auxNumLinks;
            cost += (epslon*auxNumSlots*auxNumLinks)/
                    (totalNumSlots*totalNumLinks);
        }
        else{
            totalCost = Def::Max_Double;
            break;
        }
        
        totalCost += cost;
    }
    
    return totalCost;
}

double ResourceDeviceAlloc::SCRA4_Cost(CallDevices* call, unsigned routeIndex, 
unsigned subRouteIndex) {
    std::shared_ptr<Route> auxRoute;
    NodeDevices* auxNode;
    double totalCost = 0.0, cost = 0.0;
    double alpha = 0.0;
    double epslon = 1E-6;
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
            auxNumSlots = ((double) vecNumSlots.at(ind)) / auxNumLinks;
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
            cost -= (epslon*numUsedReg/totalFreeReg);
        }
        
        totalCost += cost;
    }
    
    return totalCost;
}

double ResourceDeviceAlloc::SCRA5_Cost(CallDevices* call, unsigned routeIndex, 
unsigned subRouteIndex) {
    std::shared_ptr<Route> auxRoute;
    NodeDevices* auxNode;
    double totalCost = 0.0, cost = 0.0;
    double alpha = 0.0;
    double epslon = 1E-6;
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
            auxNumSlots = ((double) vecNumSlots.at(ind)) / auxNumLinks;
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
            cost += (epslon*numUsedReg/totalFreeReg);
        }
        
        totalCost += cost;
    }
    
    return totalCost;
}

unsigned int ResourceDeviceAlloc::GetN(CallDevices* call) {
    double bandwidth = 0.0;
    unsigned int numSlots = 0;
    
    //Se precisar modificar, pegar a modulação da última combinação de 
    //segmentos transparentes.
    bandwidth = modulation->BandwidthQAM(FirstModulation, call->GetBitRate());
    if(modulation->isEON())
        numSlots = std::ceil(bandwidth/modulation->GetSlotBandwidth());
    else
        numSlots = 1;
    
    return numSlots;
}

bool ResourceDeviceAlloc::CreateRegOption(CallDevices* call, unsigned routeInd,
std::vector<std::shared_ptr<Route> >& routes, 
std::vector<TypeModulation>& modulations) {
    
    switch(options->GetRegAssOption()){
        case RegAssFLRonline:
            return this->SetRegChoiceOrderFLR(call, routeInd, routes, 
            modulations);
        case RegAssFNS:
            return this->SetRegChoiceOrderFNS(call, routeInd, routes, 
            modulations);
        default:
            std::cerr << "Invalid regeneration option" << std::endl;
            std::abort();
    }
}

bool ResourceDeviceAlloc::SetRegChoiceOrderFLR(CallDevices* call, 
unsigned routeInd, std::vector<std::shared_ptr<Route> >& routes, 
std::vector<TypeModulation>& modulations) {
    std::shared_ptr<Route> orRoute = call->GetRoute(routeInd);
    unsigned numRouteNodes = orRoute->GetNumNodes();
    double bitRate = call->GetBitRate();
    std::vector<NodeDevices*> nodes(0);
    std::shared_ptr<Route> auxRoute;
    unsigned numReg = NodeDevices::GetNumRegRequired(call->GetBitRate());
    
    for(unsigned a = 0; a < numRouteNodes; a++)
        nodes.push_back(dynamic_cast<NodeDevices*>(orRoute->GetNode(a)));
    
    unsigned curNodeIndex = 0;
    
    for(unsigned sourIndex = 0; sourIndex < numRouteNodes; sourIndex++){
        for(unsigned testIndex = sourIndex + 1; testIndex < numRouteNodes; 
        testIndex++){
            auxRoute = orRoute->CreatePartialRoute(sourIndex, testIndex);
            
            if((nodes.at(testIndex)->GetNumFreeRegenerators() >= numReg) ||
            auxRoute->GetDeNode() == orRoute->GetDeNode()){
                
                if(this->CheckSpectrumAndOSNR(bitRate, auxRoute.get())){
                    
                    if(auxRoute->GetDeNode() == orRoute->GetDeNode()){
                        routes.push_back(auxRoute);
                        modulations.push_back(this->GetBestModulation(bitRate, 
                        auxRoute.get()));
                        return true;
                    }
                    else
                        curNodeIndex = testIndex;
                }
                else{
                    if(curNodeIndex != sourIndex){
                        auxRoute = orRoute->CreatePartialRoute(sourIndex, 
                        curNodeIndex);
                        routes.push_back(auxRoute);
                        modulations.push_back(this->GetBestModulation(bitRate,
                        auxRoute.get()));
                        sourIndex = curNodeIndex;
                        testIndex = curNodeIndex;
                    }
                    else{
                        routes.clear();
                        modulations.clear();
                        return false;
                    }
                }
            }
        }
    }
    
    routes.clear();
    modulations.clear();
    return false;
}

bool ResourceDeviceAlloc::SetRegChoiceOrderFNS(CallDevices* call, 
unsigned routeInd, std::vector<std::shared_ptr<Route> >& routes, 
std::vector<TypeModulation>& modulations) {
    std::shared_ptr<Route> orRoute = call->GetRoute(routeInd);
    unsigned numRouteNodes = orRoute->GetNumNodes();
    double bitRate = call->GetBitRate();
    std::vector<NodeDevices*> nodes(0);
    std::shared_ptr<Route> auxRoute;
    std::vector<TypeModulation> auxVecModulations = 
    Modulation::GetModulationFormats();
    TypeModulation scheme = auxVecModulations.front();
    unsigned numReg = NodeDevices::GetNumRegRequired(call->GetBitRate());
    
    for(unsigned a = 0; a < numRouteNodes; a++)
        nodes.push_back(dynamic_cast<NodeDevices*>(orRoute->GetNode(a)));
    
    unsigned curNodeIndex = 0;
    
    for(unsigned sourIndex = 0; sourIndex < numRouteNodes; sourIndex++){
        for(unsigned testIndex = sourIndex + 1; testIndex < numRouteNodes; 
        testIndex++){
            auxRoute = orRoute->CreatePartialRoute(sourIndex, testIndex);
            
            if((nodes.at(testIndex)->GetNumFreeRegenerators() >= numReg) ||
            auxRoute->GetDeNode() == orRoute->GetDeNode()){
                
                if(this->CheckSpectrumAndOSNR(bitRate, auxRoute.get(), scheme)){
                    
                    if(auxRoute->GetDeNode() == orRoute->GetDeNode()){
                        routes.push_back(auxRoute);
                        modulations.push_back(scheme);
                        return true;
                    }
                    else{
                        if(scheme != auxVecModulations.front()){
                            routes.push_back(auxRoute);
                            modulations.push_back(scheme);
                            sourIndex = testIndex;
                            curNodeIndex = testIndex;
                            scheme = auxVecModulations.front();
                        }
                        else{
                            curNodeIndex = testIndex;
                        }
                        //Colocar a expressão "curNodeIndex = testIndex" aqui?
                    }
                }
                else{
                    if(curNodeIndex != sourIndex){
                        auxRoute = orRoute->CreatePartialRoute(sourIndex, 
                        curNodeIndex);
                        routes.push_back(auxRoute);
                        modulations.push_back(scheme);
                        sourIndex = curNodeIndex;
                        testIndex = curNodeIndex;
                    }
                    else{
                        testIndex--;
                        scheme = TypeModulation(scheme - 1);
                        
                        if(scheme == auxVecModulations.back()){
                            routes.clear();
                            modulations.clear();
                            return false;
                        }
                    }
                }
            }
        }
    }
    
    routes.clear();
    modulations.clear();
    return false;
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

bool ResourceDeviceAlloc::CheckSpectrumAndOSNR(const double bitRate, 
Route* route, TypeModulation modulation) {
    double osnrThreshold = this->modulation->GetOSNRQAM(modulation, bitRate);
    unsigned numSlots = this->modulation->GetNumSlots(bitRate, modulation);
    
    if(!topology->CheckOSNR(route, osnrThreshold))
        return false;
    
    if(!topology->CheckBlockSlotsDisp(route, numSlots))
        return false;
    
    return true;
}

bool ResourceDeviceAlloc::CheckSpectrumAndOSNR(const double bitRate, 
Route* route) {
    std::vector<TypeModulation> auxVecModulations = 
    Modulation::GetModulationFormats();
    TypeModulation auxMod;
    
    for(unsigned a = 0; a < auxVecModulations.size(); a++){
        auxMod = auxVecModulations.at(a);
        
        if(auxMod != InvalidModulation){
            if(this->CheckSpectrumAndOSNR(bitRate, route, auxMod))
                return true;
        }
    }
    
    return false;
}

TypeModulation ResourceDeviceAlloc::GetBestModulation(const double bitRate, 
Route* route) {
    std::vector<TypeModulation> auxVecModulations = 
    Modulation::GetModulationFormats();
    TypeModulation auxMod;
    double osnrThreshold;
    unsigned numSlots;
    
    for(unsigned a = 0; a < auxVecModulations.size(); a++){
        auxMod = auxVecModulations.at(a);
        
        if(auxMod != InvalidModulation){
            osnrThreshold = this->modulation->GetOSNRQAM(auxMod, bitRate);
            numSlots = this->modulation->GetNumSlots(bitRate, auxMod);

            if(topology->CheckOSNR(route, osnrThreshold) && 
            topology->CheckBlockSlotsDisp(route, numSlots))
                return auxMod;
        }
    }
    
    return auxVecModulations.back();
}
