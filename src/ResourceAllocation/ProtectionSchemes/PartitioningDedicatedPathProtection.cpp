/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DedicatedPathProtection.cpp
 * Author: henrique
 * 
 * Created on October 26, 2019, 9:36 AM
 */

#include <vector>
#include <memory>

#include "../../../include/ResourceAllocation/ProtectionSchemes/PartitioningDedicatedPathProtection.h"
#include "../../../include/ResourceAllocation/SA.h"
#include "../../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../../include/ResourceAllocation/Routing.h"
#include "../../../include/ResourceAllocation/Modulation.h"
#include "../../../include/Calls/Call.h"
#include "../../../include/Data/Parameters.h"
#include "math.h" 

PartitioningDedicatedPathProtection::PartitioningDedicatedPathProtection
(ResourceDeviceAlloc* rsa): ProtectionScheme(rsa) {

}

PartitioningDedicatedPathProtection::~PartitioningDedicatedPathProtection() {

}

void PartitioningDedicatedPathProtection::CreateProtectionRoutes() {
 routing->ProtectionDisjointYEN();
}

void PartitioningDedicatedPathProtection::CreateProtectionCalls(CallDevices* call) {
    call->GetTranspSegments().clear();
    std::shared_ptr<Call> auxCall;
    std::vector<std::shared_ptr<Call>> auxVec(0);
    numSchProtRoutes = 3;
    int numWorkRoutes = numSchProtRoutes -1;
    double partialBitRate = (call->GetBitRate()/numWorkRoutes);
    
    for(unsigned a = 1; a <= numSchProtRoutes; a++){
        auxCall = std::make_shared<Call>(call->GetOrNode(), 
        call->GetDeNode(), partialBitRate, call->GetDeactivationTime());
        
        //condition for squeezing 
        if(parameters->GetBeta() != 0 && a > numSchProtRoutes - 1){            
            double percent = 100.0;
            double squeezingInd = (1 - (parameters->GetBeta() / percent));
            double bitrate = call->GetBitRate();
            double protBitRate = ceil (squeezingInd * bitrate);
            auxCall->SetBitRate(protBitRate);
        } 
        
        auxVec.push_back(auxCall);
    }
    call->SetTranspSegments(auxVec);
}

void PartitioningDedicatedPathProtection::ResourceAlloc(CallDevices* call) {

}

