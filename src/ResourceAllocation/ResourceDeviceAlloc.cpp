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
#include "../../include/Calls/CallDevices.h"
#include "../../include/GeneralClasses/Def.h"

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
        
}

void ResourceDeviceAlloc::RoutingRegSpecAlloc(CallDevices* call) {
    this->routing->RoutingCall(call);
    call->UpdateTrialModulations();
    std::vector<unsigned> indexBestReg(0);
    unsigned auxIndex;
    
    if(call->IsThereTrialRoute()){
        do{
            call->SetRoute(call->PopTrialRoute());
            this->SetRegChoiceOrder(call, indexBestReg);
            
            for(unsigned int regIndex = 0; regIndex < indexBestReg.size(); 
            regIndex++){
                auxIndex = indexBestReg.at(regIndex);
                call->CreateTranspSegments(resources->GetVecSubRoute(call, 
                                           auxIndex));
                call->SetTranspSegModulation(resources->GetSubRoutesMod(call, 
                                             auxIndex));
                this->modulation->SetModulationParam(call->GetTranspSegments());
            }
            
        }while(call->IsThereTrialRoute());
    }
}

void ResourceDeviceAlloc::SetRegChoiceOrder(CallDevices* call, 
std::vector<unsigned>& vecIndex) {
    vecIndex.clear();
    
    switch(options->GetRegAssOption()){
        case RegAssMinReg:
            this->SetMinRegChoiceOrder(call, vecIndex);
            break;
        case RegAssMaxReg:
            this->SetMaxRegChoiceOrder(call, vecIndex);
            break;
        default:
            std::cerr << "Invalid regenerator assignment option" << std::endl;
            std::abort();
    }
}

void ResourceDeviceAlloc::SetMinRegChoiceOrder(CallDevices* call, 
std::vector<unsigned>& vecIndex) {
    std::vector<unsigned> vecNumReg = resources->GetNumRegenerators(call);
    unsigned int auxInt;
    unsigned int auxIndex;
    
    while(vecIndex.size() < vecNumReg.size()){
        auxInt = Def::Max_UnInt;
        
        for(unsigned int a = 0; a < vecNumReg.size(); a++){
            
            if(auxInt > vecNumReg.at(a)){
                auxInt = vecNumReg.at(a);
                auxIndex = a;
            }
        }
        vecIndex.push_back(auxIndex);
        vecNumReg.at(auxIndex) = Def::Max_UnInt;
    }
}

void ResourceDeviceAlloc::SetMaxRegChoiceOrder(CallDevices* call, 
std::vector<unsigned>& vecIndex) {
    std::vector<unsigned> vecNumReg = resources->GetNumRegenerators(call);
    std::vector<int> auxVecNumReg(0);
    int auxInt;
    unsigned int auxIndex;
    
    for(unsigned int a = 0; a < vecNumReg.size(); a++)
        auxVecNumReg.push_back((int) vecNumReg.at(a));
    
    
    while(vecIndex.size() < auxVecNumReg.size()){
        auxInt = -1;
        
        for(unsigned int a = 0; a < auxVecNumReg.size(); a++){
            
            if(auxInt < auxVecNumReg.at(a)){
                auxInt = auxVecNumReg.at(a);
                auxIndex = a;
            }
        }
        vecIndex.push_back(auxIndex);
        auxVecNumReg.at(auxIndex) = -1;
    }
}
