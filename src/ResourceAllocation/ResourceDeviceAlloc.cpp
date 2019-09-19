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
#include "../../include/ResourceAllocation/RegeneratorAssignment/RegAssAlgorithms.h"
#include "../../include/Calls/CallDevices.h"
#include "../../include/GeneralClasses/Def.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Structure/NodeDevices.h"

ResourceDeviceAlloc::ResourceDeviceAlloc(SimulationType *simulType)
:ResourceAlloc(simulType), regAssAlgorithm(nullptr) {

}

ResourceDeviceAlloc::~ResourceDeviceAlloc() {

}

void ResourceDeviceAlloc::Load() {
    ResourceAlloc::Load();
    
    if(options->GetRegenerationOption() != RegenerationDisabled)
        this->CreateRegeneratorAssignment();
}

void ResourceDeviceAlloc::ResourAlloc(Call* call) {
    CallDevices* callDev = dynamic_cast<CallDevices*>(call);
    
    if(options->GetRegenerationOption() != RegenerationDisabled)
        regAssAlgorithm->ResourceAlloc(callDev);
    else if(options->GetTransponderOption() == TransponderEnabled)
        this->RoutingTransponderSpecAlloc(callDev);
    
    if(call->GetStatus() == NotEvaluated)
        call->SetStatus(Blocked);
}

void ResourceDeviceAlloc::RoutingOffVirtRegSpecAlloc(CallDevices* call) {
    this->routing->RoutingCall(call);
    
    //Tuple with route index and set of subRoutes index.
    std::vector<std::tuple<unsigned, unsigned>> routeSubIndexes(0);
    
    regAssAlgorithm->OrderRegenerationOptions(call, routeSubIndexes);
    
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
        
        if(regAssAlgorithm->CreateRegOption(call, routeIndex, vecRoutes, 
        vecModulation)){
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

RegeneratorAssignment* ResourceDeviceAlloc::GetRegeneratorAssignment() const {
    return regAssAlgorithm.get();
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

void ResourceDeviceAlloc::CreateRegeneratorAssignment() {
    
    switch(options->GetRegAssOption()){
        case RegAssMinReg:
            regAssAlgorithm = std::make_shared<MinimumRegeneration>(this);
            break;
        case RegAssMinSlotsMinReg:
            regAssAlgorithm = std::make_shared<MinimumSlotsMinReg>(this);
            break;
        case RegAssMinSlotsMaxReg:
            regAssAlgorithm = std::make_shared<MinimumSlotsMaxReg>(this);
            break;
        case RegAssMaxReg:
            regAssAlgorithm = std::make_shared<MaximumRegeneration>(this);
            break;
        case RegAssFLR:
        case RegAssFLRonline:
            regAssAlgorithm = std::make_shared<FirstLongestReach>(this);
            break;
        case RegAssFNS:
            regAssAlgorithm = std::make_shared<FirstNarrowestSpectrum>(this);
            break;
        case RegAssDRE2BR:
            regAssAlgorithm = std::make_shared<DRE2BR>(this);
            break;
        case RegAssSCRA1:
        case RegAssSCRA2:
        case RegAssSCRA3:
        case RegAssSCRA4:
        case RegAssSCRA5:
            regAssAlgorithm = std::make_shared<SCRA>(this);
            break;
        default:
            std::cerr << "Invalid regenerator assignment" << std::endl;
            std::abort();
        }
}
