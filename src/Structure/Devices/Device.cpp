/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Device.cpp
 * Author: BrunoVinicius
 * 
 * Created on April 28, 2019, 3:19 PM
 */

#include "../../../include/Structure/Devices/Device.h"

Device::Device(Topology* topology, NodeDevices* node, DeviceType type)
:topology(topology), node(node), type(type) {

}

Device::~Device() {
    
}
