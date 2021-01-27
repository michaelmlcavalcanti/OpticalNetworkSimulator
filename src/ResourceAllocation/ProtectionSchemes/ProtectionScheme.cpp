/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProtectionScheme.cpp
 * Author: henrique
 * 
 * Created on October 26, 2019, 9:31 AM
 */

#include "../../../include/ResourceAllocation/ProtectionSchemes/ProtectionScheme.h"
#include "../../../include/Calls/Call.h"
#include "../../../include/Data/Parameters.h"
#include "math.h" 

ProtectionScheme::ProtectionScheme(ResourceDeviceAlloc* rsa)
:resDevAlloc(rsa), resources(rsa->GetResources()), topology(rsa->GetTopology()),
parameters(rsa->parameters), modulation(rsa->GetModulation()), 
routing(rsa->routing.get()), protectionScheme(this->protectionScheme),
numSchProtRoutes(2), PDPPBitRateDistribution(0) {
    
}

ProtectionScheme::~ProtectionScheme() {
    
}

long long int ProtectionScheme::GetNumProtectedCalls() const {
    return numProtectedCalls;
}

void ProtectionScheme::IncrementNumProtectedCalls() {
    this->numProtectedCalls++;
}

void ProtectionScheme::IncrementNumNonProtectedCalls() {
     this->numNonProtectedCalls++;
}

/*void ProtectionScheme::SetPDPPBitRateDistribution(int BitRateDistChoice) {
    unsigned int numNodes = this->topology->GetNumNodes();
    this->PDPPBitRateDistribution.resize(numNodes*numNodes);
    this->PDPPBitRateDistribution.assign(numNodes*numNodes, BitRateDistChoice);
}*/



