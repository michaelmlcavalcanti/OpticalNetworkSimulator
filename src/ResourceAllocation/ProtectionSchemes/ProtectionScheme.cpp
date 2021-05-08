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
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Data/Data.h"
#include "math.h" 

ProtectionScheme::ProtectionScheme(ResourceDeviceAlloc* rsa)
:resDevAlloc(rsa), resources(rsa->GetResources()), topology(rsa->GetTopology()),
parameters(rsa->parameters), modulation(rsa->GetModulation()), 
routing(rsa->routing.get()), protectionScheme(this->protectionScheme),
numSchProtRoutes(2) {
    
}

ProtectionScheme::~ProtectionScheme() {
    
}

void ProtectionScheme::CalcBetaAverage(CallDevices* call) {
    double callBetaAverage;
 
    if(call->GetTranspSegmentsVec().size() == 3){
        double BR0 = call->GetTranspSegments().at(0)->GetBitRate();
        double BR1 = call->GetTranspSegments().at(1)->GetBitRate();
        double BR2 = call->GetTranspSegments().at(2)->GetBitRate();
        double BRT = call->GetBitRate();
        
        callBetaAverage = ((1 - ((BR0 + BR1)/BRT)) + (1 - ((BR0 + BR2)/BRT)) +
        (1 - ((BR1 + BR2)/BRT)))/3;

        this->callBetaAverage.push_back(callBetaAverage);
        resDevAlloc->simulType->GetData()->SetSumCallsBetaAverage(callBetaAverage);
    }
    
    if(call->GetTranspSegmentsVec().size() == 2){
        callBetaAverage = parameters->GetBeta();
        this->callBetaAverage.push_back(callBetaAverage);
        resDevAlloc->simulType->GetData()->SetSumCallsBetaAverage(callBetaAverage);
    }
}



