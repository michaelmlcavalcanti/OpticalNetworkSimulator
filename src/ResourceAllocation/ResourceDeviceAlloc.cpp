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
    
    if(this->options->GetRegenerationOption() == RegenerationEnabled)
        this->RoutingRegSpecAlloc(callDev);
    
    if(call->GetStatus() == NotEvaluated)
        call->SetStatus(Blocked);
}

void ResourceDeviceAlloc::RoutingRegSpecAlloc(CallDevices* call) {
    this->routing->RoutingCall(call);
    //Tuple with route index and set of subRoutes index
    std::vector<std::tuple<unsigned, unsigned>> routeSubIndexes(0);
    
    this->SetRegChoiceOrder(call, routeSubIndexes);
    
    for(unsigned int a = 0; a < routeSubIndexes.size(); a++){
        call->SetRoute(std::get<0>(routeSubIndexes.at(a)));
        unsigned int auxIndex = std::get<1>(routeSubIndexes.at(a));
        call->CreateTranspSegments(resources->GetVecSubRoute(call, auxIndex));
        
        if(!topology->CheckInsertFreeRegenerators(call))
            continue;
        call->SetTranspSegModulation(resources->GetSubRoutesMod(call,auxIndex));
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

void ResourceDeviceAlloc::SetRegChoiceOrder(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    vec.clear();
    
    switch(options->GetRegAssOption()){
        case RegAssMinReg:
            this->SetMinRegChoiceOrder(call, vec);
            break;
        case RegAssMaxReg:
            this->SetMaxRegChoiceOrder(call, vec);
            break;
        case RegMetric01:
            this->SetMetric01(call, vec);
            break;;
        default:
            std::cerr << "Invalid regenerator assignment option" << std::endl;
            std::abort();
    }
}

void ResourceDeviceAlloc::SetMinRegChoiceOrder(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    std::vector<std::vector<unsigned>> vecNumReg = 
    resources->GetNumAllRegPos(call);
    std::vector<std::vector<unsigned>> vecNumSlots = 
    resources->GetNumSlotsAllRegPos(call);
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

void ResourceDeviceAlloc::SetMaxRegChoiceOrder(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    std::vector<std::vector<unsigned>> vecNumReg = 
    resources->GetNumAllRegPos(call);
    std::vector<std::vector<unsigned>> vecNumSlots = 
    resources->GetNumSlotsAllRegPos(call);
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

void ResourceDeviceAlloc::SetMetric01(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    std::vector<std::vector<unsigned>> vecNumReg = 
    resources->GetNumAllRegPos(call);
    std::vector<std::vector<double>> vecCosts(0);
    
    //Calculate the cost of each possible regeneration option.
    vecCosts.resize(vecNumReg.size());
    for(unsigned a = 0; a < vecNumReg.size(); a++){
        vecCosts.resize(vecNumReg.at(a).size());
        
        for(unsigned b = 0; b < vecNumReg.at(a).size(); b++){
            vecCosts.at(a).at(b) = this->CalcRegCost(call, a, b);
        }
    }
    
    unsigned int auxRouteIndex = 0;
    unsigned int auxIndex = 0;
    double bestCost;
    bool foundOption = true;
    
    //Order the regeneration options, based on the cost.
    while(foundOption){
        foundOption = false;
        bestCost = Def::Max_UnInt;
        
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
            vecCosts.at(auxRouteIndex).at(auxIndex) = Def::Max_UnInt;
        }
    }
}

double ResourceDeviceAlloc::CalcRegCost(CallDevices* call, unsigned routeIndex, 
unsigned subRouteIndex) {
    double cost = 0.0;
    double LU = 0.0, TU = 0.0;
    unsigned int totalNumSlots = 0;
    unsigned int usedNumSlots = 0;
    unsigned int totalNumReg = 0;
    unsigned int usedNumReg = 0;
    const double alpha = 0.5;
    unsigned int numReg = resources->GetNumRegenerators(call, routeIndex,
                                                        subRouteIndex);
    unsigned int numSlots = resources->GetNumSlotsAllRegPos(call, routeIndex,
                                                            subRouteIndex);
    std::vector<std::shared_ptr<Route>> vecRoutes = 
    resources->GetVecSubRoute(call, routeIndex, subRouteIndex);
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
    
    LU = (double) usedNumSlots / (double) totalNumSlots;
    TU = (double) usedNumReg / (double) totalNumReg;
    cost = (1-alpha)*((double) numReg + (double) numSlots/3) + 
           (alpha)*(LU + TU);
    
    return cost;
}

bool ResourceDeviceAlloc::CheckOSNR(CallDevices* call) {
    std::vector<Call*> calls = call->GetTranspSegments();
    
    if(phyLayerOption == PhyLayerEnabled)
        for(auto it: calls){
            if(!topology->CheckOSNR(it->GetRoute(), it->GetOsnrTh()))
                return false;
        }
    
    return true;
}
