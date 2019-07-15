/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SBVT_TSS_ML.cpp
 * Author: brunovacorreia
 * 
 * Created on June 25, 2019, 7:57 PM
 */

#include "../../../include/Structure/Devices/SBVT_TSS_ML.h"
#include "../../../include/Calls/CallDevices.h"

unsigned int SBVT_TSS_ML::numSubCarriers = 4;

SBVT_TSS_ML::SBVT_TSS_ML(Topology* topology, NodeDevices* node)
:BVT(topology, node, TypeBVT_TSS_ML), subCarriers(0), modulations(0), calls(0) {
    
}

SBVT_TSS_ML::~SBVT_TSS_ML() {
    
}

void SBVT_TSS_ML::Initialize() {
    BVT::Initialize();
    subCarriers.assign(numSubCarriers, free);
    modulations.assign(numSubCarriers, InvalidModulation);
    calls.assign(numSubCarriers, nullptr);
}

unsigned int SBVT_TSS_ML::GetNumberFreeSubCarriers() const {
    unsigned int numFreeSubCarriers = 0;
    
    for(auto it: subCarriers){
        
        if(it == free)
            numFreeSubCarriers++;
    }
    
    return numFreeSubCarriers;
}

void SBVT_TSS_ML::SetCallToSubCarriers(CallDevices* call, 
unsigned int numSubCarriers) {
    unsigned int numAllocatedSubCarriers = 0;
    
    for(unsigned ind = 0; ind < subCarriers.size(); ind++){
        
        if(subCarriers.at(ind) == free){
            modulations.at(ind) = call->GetModulation();
            calls.at(ind) = call;
            numAllocatedSubCarriers++;
        }
        
        if(numAllocatedSubCarriers == numSubCarriers)
            break;
    }
    assert(numAllocatedSubCarriers == numSubCarriers);
}

void SBVT_TSS_ML::ConnectSubCarriers(CallDevices* call) {
    unsigned int numCarriers = 0;
    
    for(unsigned ind = 0; ind < calls.size(); ind++){
        
        if(calls.at(ind) == call){
            subCarriers.at(ind) = occupied;
            numCarriers++;
        }
    }
    assert(numCarriers > 0);
}

void SBVT_TSS_ML::ReleaseSubCarriers(CallDevices* call) {
    unsigned int numCarriers = 0;
    
    for(unsigned ind = 0; ind < calls.size(); ind++){
        
        if(calls.at(ind) == call){
            subCarriers.at(ind) = free;
            modulations.at(ind) = InvalidModulation;
            calls.at(ind) = nullptr;
            numCarriers++;
        }
    }
    assert(numCarriers > 0);
}
