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
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/ResourceAllocation/ProtectionSchemes/ProtectionSchemes.h"  

ResourceDeviceAlloc::ResourceDeviceAlloc(SimulationType *simulType)
:ResourceAlloc(simulType), protectionScheme(this->protectionScheme),
regAssAlgorithm(nullptr) {

}

ResourceDeviceAlloc::~ResourceDeviceAlloc() {

}

void ResourceDeviceAlloc::Load() {
    ResourceAlloc::Load();
    
    if(options->GetRegenerationOption() != RegenerationDisabled)
        this->CreateRegeneratorAssignment();
    //Put a else? How proceed when the two options are enabled?
    if(options->GetProtectionOption() != ProtectionDisable)
        this->CreateProtectionScheme();

}

void ResourceDeviceAlloc::AdditionalSettings() {
    ResourceAlloc::AdditionalSettings();
    
    if(this->IsOfflineRouting()){
        if(this->options->GetRegenerationOption() != RegenerationDisabled){
            assert(options->GetPhyLayerOption() == PhyLayerEnabled);
            assert(options->GetResourAllocOption() == ResourAllocRMSA);
            this->resources->CreateRegenerationResources();
        }
    
        // Put functions to create the offline
        if(options->GetProtectionOption() != ProtectionDisable){
           protScheme->CreateProtectionRoutes();
           
        }
    }
}

void ResourceDeviceAlloc::ResourAlloc(Call* call) {
    CallDevices* callDev = dynamic_cast<CallDevices*>(call);
    
    if(options->GetRegenerationOption() != RegenerationDisabled)
        regAssAlgorithm->ResourceAlloc(callDev);
    else if(options->GetTransponderOption() == TransponderEnabled)
        this->RoutingTransponderSpecAlloc(callDev);
    else if(options->GetProtectionOption() != ProtectionDisable)
        protScheme->ResourceAlloc(callDev);
    
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
                
                if(!ResourceAlloc::CheckOSNR(it->GetRoute(), it->GetOsnrTh()))
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
            regAssAlgorithm = std::make_shared<SCRA>(this, -0.05, 1.0, 1.0);
            break;
        case RegAssSCRA2:
            regAssAlgorithm = std::make_shared<SCRA>(this, 0.0, 1.0, 1.0);
            break;
        case RegAssSCRA3:
            regAssAlgorithm = std::make_shared<SCRA>(this, -1.0, 1E-5, 0.0);
            break;
        case RegAssSCRA4:
            regAssAlgorithm = std::make_shared<SCRA>(this, 0.0, 1.0, -1E-5);
            break;
        case RegAssSCRA5:
            regAssAlgorithm = std::make_shared<SCRA>(this, 0.0, 1.0, 1E-5);
            break;
        case RegAssSCRA_PSO:{
            std::ifstream auxIfstream;
            simulType->GetInputOutput()->LoadCoefficientsSCRA(auxIfstream);
            regAssAlgorithm = std::make_shared<SCRA>(this, auxIfstream);
            break;
        }
        case RegAssSCRA_Mod:
            regAssAlgorithm = std::make_shared<SCRA2>(this, -0.05, 1.0, 1.0, 
                                                      1.0);
            break;
        case RegAssSCRA_ModPSO:{
            std::ifstream auxIfstream;
            simulType->GetInputOutput()->LoadCoefficientsSCRA(auxIfstream);
            regAssAlgorithm = std::make_shared<SCRA2>(this, auxIfstream);
            break;
        }
        default:
            std::cerr << "Invalid regenerator assignment" << std::endl;
            std::abort();
    }
}

void ResourceDeviceAlloc::CreateProtectionScheme() {
    
    switch (options->GetProtectionOption()){
        case ProtectionDPP:
            protScheme = std::make_shared<DedicatedPathProtection>(this);
            break;
        case ProtectionPDPP:
            protScheme = std::make_shared<PartitioningDedicatedPathProtection>
            (this);
            break;
        case ProtectionEPDPP_GA:
            protScheme = std::make_shared<PartitioningDedicatedPathProtection>
            (this);  
            break;
        default:
            std::cerr << "Invalid Protection Option" << std::endl;
            std::abort();
    }
}
