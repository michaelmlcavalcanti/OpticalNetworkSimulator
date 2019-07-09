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
#include "../../../include/Structure/Topology.h"
#include "../../../include/Structure/NodeDevices.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Calls/CallGenerator.h"

const double Regenerator::trafficSupported = 100E9;

Regenerator::Regenerator(Topology* topology, NodeDevices* node)
:Device(topology, node, DeviceRegenerator), isActive(false), onTime(0.0), 
totalOnTime(0.0) {
    
}

Regenerator::~Regenerator() {
    
}

void Regenerator::Initialize() {
    this->isActive = false;
    this->onTime = 0.0;
    this->totalOnTime = 0.0;
}

void Regenerator::SetRegeneratorOn() {
    assert(!this->isActive);
    
    this->isActive = true;
    this->onTime = topology->GetSimulType()->GetCallGenerator()->
                   GetSimulationTime();
    node->DecreaseNumFreeRegenerators();
}

void Regenerator::SetRegeneratorOff() {
    assert(this->isActive);
    
    this->isActive = false;
    this->totalOnTime += topology->GetSimulType()->
                         GetCallGenerator()->GetSimulationTime() - this->onTime;
    this->onTime = 0.0;
    node->IncreaseNumFreeRegenerators();
}

bool Regenerator::IsActive() const {
    return isActive;
}

const double Regenerator::GetTrafficSupported() {
    return trafficSupported;
}
