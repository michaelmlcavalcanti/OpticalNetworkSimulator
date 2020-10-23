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
    // assert(routing->GetKd() >= 2);
    if(routing->GetKd() < 2){
        routing->SetKd(2);
    }
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
            double bitrate = partialBitRate;
            double protBitRate = ceil (squeezingInd * bitrate);
            auxCall->SetBitRate(protBitRate);
        } 
        
        auxVec.push_back(auxCall);
    }
    call->SetTranspSegments(auxVec);
}

void PartitioningDedicatedPathProtection::ResourceAlloc(CallDevices* call) {

    this->RoutingOffNoSameSlotProtPDPPSpecAlloc(call);
}

void PartitioningDedicatedPathProtection::RoutingOffNoSameSlotProtPDPPSpecAlloc
(CallDevices* call) {

    this->routing->RoutingCall(call); //loading trialRoutes and trialprotRoutes
    
    this->CreateProtectionCalls(call); //loading transpsegments with calls
    
    std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
    std::shared_ptr<Call> callWork0 = callsVec.at(0);
    std::shared_ptr<Call> callWork1 = callsVec.at(1);
    std::shared_ptr<Call> callBackup = callsVec.at(3);
    unsigned int numRoutes = call->GetNumRoutes();

    for(unsigned int k = 0; k < numRoutes; k++){
        callWork0->SetRoute(call->GetRoute(k));
        callWork0->SetModulation(FixedModulation);
        unsigned int sizeProtRoutes = call->GetProtRoutes(k).size();
        
        for(unsigned int kd0 = 0; kd0 < sizeProtRoutes; kd0++) {
            
            if(call->GetProtRoute(k , kd0)){  //if to avoid null route pointer
                callWork1->SetRoute(call->GetProtRoute(k, kd0));
                callWork1->SetModulation(FixedModulation);    

                for(unsigned int kd1 = 0; kd1 < sizeProtRoutes; kd1++) {
            
                    if(call->GetProtRoute(k , kd1)){  //if to avoid null route pointer
                        if(kd1 == kd0){         
                            kd1 = kd0 + 1;
                            callBackup->SetRoute(call->GetProtRoute(k, kd1));
                            callBackup->SetModulation(FixedModulation);
                        }
                        else{
                            callBackup->SetRoute(call->GetProtRoute(k, kd1));
                            callBackup->SetModulation(FixedModulation);
                        }           
                    }
                
                    //calculate number of slots for the vector of calls (transpsegments)
                    this->modulation->SetModulationParam(call);

                    this->resDevAlloc->specAlloc->SpecAllocation(call);

                    if(topology->IsValidLigthPath(call)){
                        call->SetRoute(k);
                        call->SetFirstSlot(callWork0->GetFirstSlot());
                        call->SetLastSlot(callWork0->GetLastSlot());
                        call->ClearTrialRoutes();
                        call->ClearTrialProtRoutes();
                        call->SetStatus(Accepted);
                        return;           
                    }
                }
            }
        }    
    }
}



