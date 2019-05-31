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
#include "../../include/ResourceAllocation/SA.h"
#include "../../include/Calls/CallDevices.h"
#include "../../include/GeneralClasses/Def.h"
#include "../../include/Structure/Topology.h"

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

bool ResourceDeviceAlloc::CheckOSNR(CallDevices* call) {
    std::vector<Call*> calls = call->GetTranspSegments();
    
    if(phyLayerOption == PhyLayerEnabled)
        for(auto it: calls){
            if(!topology->CheckOSNR(it->GetRoute(), it->GetOsnrTh()))
                return false;
        }
    
    return true;
}
