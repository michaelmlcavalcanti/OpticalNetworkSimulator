/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Regenerator.cpp
 * Author: BrunoVinicius
 * 
 * Created on April 28, 2019, 4:51 PM
 */

#include "../../../include/Structure/Devices/Regenerator.h"

Regenerator::Regenerator(Topology* topology, Node* node, DeviceType type)
:Device(topology, type), node(node), state(false), onTime(0.0) {
    
}

Regenerator::~Regenerator() {
    
}

