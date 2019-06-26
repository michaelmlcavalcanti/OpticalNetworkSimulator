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

unsigned int SBVT_TSS_ML::numSubCarriers = 6;

SBVT_TSS_ML::SBVT_TSS_ML(Topology* topology, NodeDevices* node)
:BVT(topology, node) {
    
}

SBVT_TSS_ML::~SBVT_TSS_ML() {
    
}

void SBVT_TSS_ML::Initialize() {
    BVT::Initialize();
}

bool SBVT_TSS_ML::IsPossibleAllocate(Call* call) {
    return true;
}
