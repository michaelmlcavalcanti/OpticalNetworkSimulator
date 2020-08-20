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
:resDevAlloc(rsa), numProtRoutes(1) {  
    
}

ProtectionScheme::~ProtectionScheme() {
}

void ProtectionScheme::CreateProtectionCalls(Call* call) {
    std::shared_ptr<Call> auxCall;
    
    for(unsigned a = 0; numProtRoutes+1; a++){
        auxCall = std::make_shared<Call>(call->GetOrNode(), 
        call->GetDeNode(), call->GetBitRate(), call->GetDeactivationTime());
        auxCall->SetRoute(a);
        protectionCalls.push_back(auxCall); 
        
            if(parameters->GetBeta() != 0){
                double protBitRate = ceil (parameters->GetBeta() * call->GetBitRate());
                call->SetBitRate(protBitRate);
            }                     
    }
    double workBitRate = protectionCalls.front()->GetBitRate();
    call->SetBitRate(workBitRate);
}
