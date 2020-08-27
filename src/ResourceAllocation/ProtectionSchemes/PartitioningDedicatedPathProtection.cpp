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

void PartitioningDedicatedPathProtection::CreateProtectionCalls(Call* call) {
 std::shared_ptr<Call> auxCall;
    numProtRoutes = 3;
    
    for(unsigned a = 1; numProtRoutes; a++){
        auxCall = std::make_shared<Call>(call->GetOrNode(), 
        call->GetDeNode(), call->GetBitRate(), call->GetDeactivationTime());
        double workBitRate = ceil (call->GetBitRate() / numProtRoutes);   
        auxCall->SetBitRate(workBitRate);
        protectionCalls.push_back(auxCall); 
        
            if(a = numProtRoutes && parameters->GetBeta() != 0){
                double protBitRate = ceil ((1 - parameters->GetBeta()) * 
                auxCall->GetBitRate());
                auxCall->SetBitRate(protBitRate);
                protectionCalls.push_back(auxCall); 
            }     
    }
}

void PartitioningDedicatedPathProtection::ResourceAlloc(CallDevices* call) {

}

