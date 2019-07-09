/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BVT.cpp
 * Author: brunovacorreia
 * 
 * Created on June 24, 2019, 6:44 PM
 */

#include "../../../include/Structure/Devices/BVT.h"

BVT::BVT(Topology* topology, NodeDevices* node, TypeBVT typeBVT)
:Device(topology, DeviceTransponder), node(node), typeBVT(typeBVT), 
isActive(false) {
    
}

BVT::~BVT() {
    
}

void BVT::Initialize() {
    isActive = false;
}
