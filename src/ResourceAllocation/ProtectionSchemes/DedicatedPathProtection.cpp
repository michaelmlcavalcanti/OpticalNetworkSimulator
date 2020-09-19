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

#include "../../../include/ResourceAllocation/ProtectionSchemes/DedicatedPathProtection.h"
#include "../../../include/ResourceAllocation/SA.h"
#include "../../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../../include/ResourceAllocation/Routing.h"
#include "../../../include/ResourceAllocation/Modulation.h"
#include "../../../include/Calls/CallDevices.h"
#include "../../../include/Data/Parameters.h"
#include "math.h" 


DedicatedPathProtection::DedicatedPathProtection(ResourceDeviceAlloc* rsa) :
ProtectionScheme(rsa) {
    
}

DedicatedPathProtection::~DedicatedPathProtection() {
    
}

void DedicatedPathProtection::CreateProtectionRoutes() {
    routing->ProtectionDisjointYEN(); 
}

void DedicatedPathProtection::CreateProtectionCalls(CallDevices* call) {
    call->GetTranspSegments().clear();
    std::shared_ptr<Call> auxCall;
    std::vector<std::shared_ptr<Call>> auxVec(0);
    numSchProtRoutes = 2;
    
    for(unsigned a = 1; numSchProtRoutes; a++){
        auxCall = std::make_shared<Call>(call->GetOrNode(), 
        call->GetDeNode(), call->GetBitRate(), call->GetDeactivationTime());
        //condition for squeezing 
        if(parameters->GetBeta() != 0 && auxVec.size() > 1){
            double protBitRate = ceil ((1 - parameters->GetBeta()) * 
            call->GetBitRate());
            auxCall->SetBitRate(protBitRate);
        }
        auxVec.push_back(auxCall);
    }
    call->SetTranspSegments(auxVec); 
}

void DedicatedPathProtection::ResourceAlloc(CallDevices* call) {
    this->CreateProtectionCalls(call);
    resDevAlloc->RoutingOffNocontProtDPPSpecAlloc(call);
    
/*    std::shared_ptr<Call> callWork = protectionCalls.front();
    std::shared_ptr<Call> callBackup = protectionCalls.back();
    unsigned int numRoutes = call->GetNumRoutes();
    unsigned int orN = call->GetOrNode()->GetNodeId();
    unsigned int deN = call->GetDeNode()->GetNodeId();
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int nodePairIndex = orN * numNodes + deN;
    bool allocFound = false;
    const unsigned int topNumSlots = topology->GetNumSlots();
    std::vector<unsigned int> possibleSlots(0);
    possibleSlots = this->resDevAlloc->specAlloc->SpecAllocation();
    unsigned int auxSlot;
    
    for(unsigned int a = 0; a < numRoutes; a++){
        callWork->SetRoute(call->GetRoute(a));
        callWork->SetModulation(call->GetModulation(a));
        
        for(unsigned int b = 0; b < resources->protectionAllRoutes.at
            (nodePairIndex).at(a).size(); b++) {
            callBackup->SetRoute(resources->protectionAllRoutes.at
            (nodePairIndex).at(a).at(b));
            callBackup->SetModulation(call->GetModulation(b));
            
            // Calculate the number of slots for the vector of calls
            this->modulation->SetModulationParam(call);
            
            for(unsigned int c = 0; c < possibleSlots.size(); c++){
                auxSlot = possibleSlots.at(c);
                
                if(auxSlot + callWork->GetNumberSlots() - 1 >= topNumSlots)
                    break;
                else if(auxSlot + callBackup->GetNumberSlots() - 1 >= topNumSlots)
                    continue;
                
                //if(!this->resDevAlloc->CheckOSNR(callWork))
               //    break;
               // else if(!this->resDevAlloc->CheckOSNR(callBackup))
               //     continue;
                
                if(resDevAlloc->CheckSlotsDisp(callWork->GetRoute(), auxSlot, 
                auxSlot + callWork->GetNumberSlots() - 1) && 
                resDevAlloc->CheckSlotsDisp(callBackup->GetRoute(), auxSlot, 
                auxSlot + callBackup->GetNumberSlots() - 1)){                        
                    callWork->SetFirstSlot(auxSlot);
                    callWork->SetLastSlot(auxSlot + callWork->GetNumberSlots() - 1);
                    callBackup->SetFirstSlot(auxSlot);
                    callBackup->SetLastSlot(auxSlot + callBackup->GetNumberSlots() - 1);
                    call->ClearTrialModulations();
                    call->ClearTrialRoutes();
                    call->SetStatus(Accepted);
                    allocFound = true;
                    break;
                }
                
                if(allocFound)
                    break;
            }
            
            if(allocFound)
                    break;
        }
        if(allocFound)
            break;    
    }
    */
}
