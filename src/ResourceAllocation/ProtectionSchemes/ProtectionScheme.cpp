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
:resDevAlloc(rsa), numSchProtRoutes(2) {  
    resources = resDevAlloc->GetResources();
    routing = resDevAlloc->routing;
    parameters = resDevAlloc->parameters;
    topology = resDevAlloc->GetTopology();
    modulation = resDevAlloc->GetModulation();
    
}

ProtectionScheme::~ProtectionScheme() {
}
