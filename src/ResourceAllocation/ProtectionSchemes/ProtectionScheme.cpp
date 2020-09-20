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
routing(rsa->routing.get()), numSchProtRoutes(2) {
    
}

ProtectionScheme::~ProtectionScheme() {
    
}
