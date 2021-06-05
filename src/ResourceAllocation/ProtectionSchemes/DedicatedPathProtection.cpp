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
#include "../../../include/ResourceAllocation/ProtectionSchemes/ProtectionScheme.h"
#include "../../../include/ResourceAllocation/SA.h"
#include "../../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../../include/ResourceAllocation/Routing.h"
#include "../../../include/ResourceAllocation/Modulation.h"
#include "../../../include/Calls/CallDevices.h"
#include "../../../include/Data/Parameters.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Data/Data.h"
#include "math.h" 

DedicatedPathProtection::DedicatedPathProtection(ResourceDeviceAlloc* rsa) 
: ProtectionScheme(rsa) {
    
}

DedicatedPathProtection::~DedicatedPathProtection() {
    
}

void DedicatedPathProtection::CreateProtectionRoutes() {
    routing->ProtectionDisjointYEN();
}

void DedicatedPathProtection::ResourceAlloc(CallDevices* call) {

    switch(resDevAlloc->simulType->GetOptions()->GetOrderRSA()){
        case OrderRoutingSa:
            this->RoutingSpecDPP(call);
            break;
        case OrderSaRouting:
            this->SpecRoutingDPP(call);
            //this->SpecRoutingSameSlotDPP(call);
            break;
        default:
            std::cout << "Invalid RSA Ordering" << std::endl;
    }
   
}

void DedicatedPathProtection::RoutingSpecDPP(CallDevices* call) {
    
    this->routing->RoutingCall(call); //loading trialRoutes and trialprotRoutes
    
    this->CreateProtectionCalls(call); //loading transpsegments with calls
    
    std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
    std::shared_ptr<Call> callWork = callsVec.front();
    std::shared_ptr<Call> callBackup = callsVec.back(); 
    unsigned int numRoutes = call->GetNumRoutes();
    
    //Try work and protection together
    for(unsigned int a = 0; a < numRoutes; a++){
        callWork->SetRoute(call->GetRoute(a));
        callWork->SetModulation(FixedModulation);
        unsigned int sizeProtRoutes = call->GetProtRoutes(a).size();
        
        for(unsigned int b = 0; b < sizeProtRoutes; b++) {
            
            if(call->GetProtRoute(a , b)){  //if to avoid null route pointer
                callBackup->SetRoute(call->GetProtRoute(a, b));
                callBackup->SetModulation(FixedModulation);

                //calculate number of slots for the vector of calls (transpsegments)
                this->modulation->SetModulationParam(call);

                this->resDevAlloc->specAlloc->SpecAllocation(call);

                if(topology->IsValidLigthPath(call)){
                    call->SetRoute(a);
                    call->SetModulation(FixedModulation);
                    call->SetFirstSlot(callWork->GetFirstSlot());
                    call->SetLastSlot(callWork->GetLastSlot());
                    call->ClearTrialRoutes();
                    call->ClearTrialProtRoutes();
                    call->SetStatus(Accepted);
                    resDevAlloc->simulType->GetData()->SetProtectedCalls();
                    CalcBetaAverage(call);
                    CalcAlpha(call);
                    return;
                }
            }
        }
    }
    
    /*//Delete protection route
    callsVec.pop_back();
    call->SetTranspSegments(callsVec);
    
    //Try only work connection (without protection)
    for(unsigned int k = 0; k < numRoutes; k++){
        callWork->SetRoute(call->GetRoute(k));
        callWork->SetModulation(FixedModulation);
        this->modulation->SetModulationParam(call);
        this->resDevAlloc->specAlloc->SpecAllocation(call);
        
        if(topology->IsValidLigthPath(call)){
            call->SetRoute(call->GetRoute(k));
            call->SetFirstSlot(callWork->GetFirstSlot());
            call->SetLastSlot(callWork->GetLastSlot());
            call->ClearTrialRoutes();
            call->ClearTrialProtRoutes();
            call->SetStatus(Accepted);
            resDevAlloc->simulType->GetData()->SetNonProtectedCalls();
            return;
        }
    } */
}

void DedicatedPathProtection::SpecRoutingDPP(CallDevices* call) {
    this->routing->RoutingCall(call); //loading trialRoutes and trialprotRoutes

    this->CreateProtectionCalls(call); //loading transpsegments with calls

    //seting 2 protection calls to allocation
    std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
    std::shared_ptr<Call> callWork0 = callsVec.at(0);
    std::shared_ptr<Call> callWork1 = callsVec.at(1);

    //call->RepeatModulation();
    unsigned int numRoutes = call->GetNumRoutes();
    call->SetCore(0);
    bool allocCallWork0Found = false;
    bool allocCallWork1Found = false;

    const unsigned int topNumSlots = topology->GetNumSlots();
    std::vector<unsigned int> possibleSlots(0);
    possibleSlots = this->resDevAlloc->specAlloc->SpecAllocation();
    unsigned int auxSlot0 = 0;
    unsigned int auxSlot1 = 0;

    //slot loop for callWork0
    for (unsigned int s0 = 0; s0 < possibleSlots.size(); s0++) {
        auxSlot0 = possibleSlots.at(s0);

        for (unsigned int k = 0; k < numRoutes; k++) {
            callWork0->SetRoute(call->GetRoute(k));
            callWork0->SetModulation(FixedModulation);

            //getting protection routes to use in next loop (FOR)
            std::deque <std::shared_ptr<Route>> ProtRoutes = call->GetProtRoutes(k);
            ProtRoutes.erase(std::remove(std::begin(ProtRoutes),
                                         std::end(ProtRoutes), nullptr),std::end(ProtRoutes));
            unsigned int sizeProtRoutes = ProtRoutes.size();

            //calculate number of slots for current of call
            this->modulation->SetModulationParam(callWork0.get());

            if (auxSlot0 + callWork0->GetNumberSlots() - 1 >= topNumSlots)
                continue;
            //checking if callWork0 number of slots are available in its route
            if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(), auxSlot0,
                                           auxSlot0 +callWork0->GetNumberSlots() -1)) {
                callWork0->SetFirstSlot(auxSlot0);
                callWork0->SetLastSlot(auxSlot0 + callWork0->GetNumberSlots() - 1);
                callWork0->SetCore(0);

                if (sizeProtRoutes >= 1) {  //if to skip case which it is no routes enough
                    for (unsigned int s1 = 0; s1 < possibleSlots.size(); s1++) {
                        auxSlot1 = possibleSlots.at(s1);
                        for (unsigned int kd0 = 0; kd0 < sizeProtRoutes; kd0++) {
                            if (call->GetProtRoute(k, kd0)) {  //if to avoid null route pointer
                                callWork1->SetRoute(call->GetProtRoute(k, kd0));
                                callWork1->SetModulation(FixedModulation);

                                this->modulation->SetModulationParam(callWork1.get());

                                if (auxSlot1 + callWork1->GetNumberSlots() - 1 >= topNumSlots)
                                    continue;
                                //checking if callWork1 slots are available in its route
                                if (this->resDevAlloc->CheckSlotsDisp(callWork1->GetRoute(), auxSlot1,
                                        auxSlot1 + callWork1->GetNumberSlots() - 1)) {
                                    callWork1->SetFirstSlot(auxSlot1);
                                    callWork1->SetLastSlot(auxSlot1 + callWork1->GetNumberSlots() - 1);
                                    callWork1->SetCore(0);

                                    call->SetRoute(call->GetRoute(k));
                                    call->SetModulation(FixedModulation);
                                    call->SetFirstSlot(callWork0->GetFirstSlot());
                                    call->SetLastSlot(callWork0->GetLastSlot());
                                    call->ClearTrialRoutes();
                                    call->ClearTrialProtRoutes();
                                    call->SetStatus(Accepted);
                                    //increment proCalls counter
                                    resDevAlloc->simulType->GetData()->SetProtectedCalls();
                                    CalcBetaAverage(call);
                                    CalcAlpha(call);
                                    allocCallWork0Found = true;
                                    allocCallWork1Found = true;
                                    break;
                                }
                            }
                        }
                        if (allocCallWork1Found)
                            break;
                    }
                }
            }
            if (allocCallWork0Found)
                break;
        }
        if (allocCallWork0Found)
            break;
    }

    /*if(allocCallWork0Found == false)
    //Delete one route and try allocating just 1 route (without protection)
        callsVec.pop_back();
        callWork0->SetBitRate(call->GetBitRate());
        call->SetTranspSegments(callsVec);

    for (unsigned int s = 0; s < possibleSlots.size(); s++) {
        auxSlot = possibleSlots.at(s);
        for (unsigned int k = 0; k < numRoutes; k++) {
            callWork0->SetRoute(call->GetRoute(k));
            callWork0->SetModulation(FixedModulation);

            //calculate number of slots for current of call
            this->modulation->SetModulationParam(callWork0.get());

            if (auxSlot + callWork0->GetNumberSlots() - 1 >= topNumSlots)
                continue;
            //checking if callWork0 number of slots are available in its route
            if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(), auxSlot,
                                         auxSlot +callWork0->GetNumberSlots() -1)) {
                callWork0->SetFirstSlot(auxSlot);
                callWork0->SetLastSlot(auxSlot + callWork0->GetNumberSlots() - 1);
                callWork0->SetCore(0);

                call->SetRoute(call->GetRoute(k));
                call->SetModulation(FixedModulation);
                call->SetFirstSlot(callWork0->GetFirstSlot());
                call->SetLastSlot(callWork0->GetLastSlot());
                call->ClearTrialRoutes();
                call->ClearTrialProtRoutes();
                call->SetStatus(Accepted);
                resDevAlloc->simulType->GetData()->SetNonProtectedCalls(); //increment proCalls counter
                CalcBetaAverage(call);
                CalcAlpha(call);
                break;
            }
        }
    }*/
}

void DedicatedPathProtection::SpecRoutingSameSlotDPP(CallDevices* call) {
    this->routing->RoutingCall(call); //loading trialRoutes and trialprotRoutes
    unsigned int numRoutes = call->GetNumRoutes();

    this->CreateProtectionCalls(call); //loading transpsegments with calls

    //seting 2 protection calls to allocation
    std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
    std::shared_ptr<Call> callWork0 = callsVec.at(0);
    std::shared_ptr<Call> callWork1 = callsVec.at(1);

    //call->RepeatModulation();
    call->SetCore(0);
    bool allocCallWork0Found = false;
    bool allocCallWork1Found = false;

    const unsigned int topNumSlots = topology->GetNumSlots();
    std::vector<unsigned int> possibleSlots(0);
    possibleSlots = this->resDevAlloc->specAlloc->SpecAllocation();
    unsigned int auxSlot;

    //try allocation with 2 routes
    for (unsigned int s = 0; s < possibleSlots.size(); s++) {
        auxSlot = possibleSlots.at(s);

        for (unsigned int k = 0; k < numRoutes; k++) {
            callWork0->SetRoute(call->GetRoute(k));
            callWork0->SetModulation(FixedModulation);

            //getting protection routes to use in next loop (FOR)
            std::deque <std::shared_ptr<Route>> ProtRoutes = call->GetProtRoutes(
                    k);
            ProtRoutes.erase(std::remove(std::begin(ProtRoutes),
                                         std::end(ProtRoutes), nullptr),std::end(ProtRoutes));
            unsigned int sizeProtRoutes = ProtRoutes.size();

            //calculate number of slots for current of call
            this->modulation->SetModulationParam(callWork0.get());

            if (auxSlot + callWork0->GetNumberSlots() - 1 >= topNumSlots)
                continue;
            //checking if callWork0 number of slots are available in its route
            if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(), auxSlot,
                                                  auxSlot +callWork0->GetNumberSlots() -1)) {
                callWork0->SetFirstSlot(auxSlot);
                callWork0->SetLastSlot(auxSlot + callWork0->GetNumberSlots() - 1);
                callWork0->SetCore(0);

                if (sizeProtRoutes >= 2) {  //if to skip case which it is no routes enough
                    for (unsigned int kd0 = 0; kd0 < sizeProtRoutes; kd0++) {
                        if (call->GetProtRoute(k, kd0)) {  //if to avoid null route pointer
                            callWork1->SetRoute(call->GetProtRoute(k, kd0));
                            callWork1->SetModulation(FixedModulation);

                            this->modulation->SetModulationParam(callWork1.get());

                            if (auxSlot + callWork1->GetNumberSlots() - 1 >= topNumSlots)
                                continue;
                            //checking if callWork1 slots are available in its route
                            if (this->resDevAlloc->CheckSlotsDisp(callWork1->GetRoute(), auxSlot,
                                                                  auxSlot + callWork1->GetNumberSlots() - 1)) {
                                callWork1->SetFirstSlot(auxSlot);
                                callWork1->SetLastSlot(auxSlot + callWork1->GetNumberSlots() - 1);
                                callWork1->SetCore(0);

                                call->SetRoute(call->GetRoute(k));
                                call->SetModulation(FixedModulation);
                                call->SetFirstSlot(callWork0->GetFirstSlot());
                                call->SetLastSlot(callWork0->GetLastSlot());
                                call->ClearTrialRoutes();
                                call->ClearTrialProtRoutes();
                                call->SetStatus(Accepted);
                                resDevAlloc->simulType->GetData()->SetProtectedCalls(); //increment proCalls counter
                                CalcBetaAverage(call);
                                CalcAlpha(call);
                                allocCallWork0Found = true;
                                allocCallWork1Found = true;
                                break;
                            }
                        }
                    }
                }
            }
            if (allocCallWork0Found)
                break;
        }
        if (allocCallWork1Found)
            break;
    }

    /*if(allocCallWork1Found == false)
    //Delete one route and try allocating just 1 route (without protection)
        callsVec.pop_back();
        callWork0->SetBitRate(call->GetBitRate());
        call->SetTranspSegments(callsVec);

    for (unsigned int s = 0; s < possibleSlots.size(); s++) {
        auxSlot = possibleSlots.at(s);
        for (unsigned int k = 0; k < numRoutes; k++) {
            callWork0->SetRoute(call->GetRoute(k));
            callWork0->SetModulation(FixedModulation);

            //calculate number of slots for current of call
            this->modulation->SetModulationParam(callWork0.get());

            if (auxSlot + callWork0->GetNumberSlots() - 1 >= topNumSlots)
                continue;
            //checking if callWork0 number of slots are available in its route
            if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(), auxSlot,
                                         auxSlot +callWork0->GetNumberSlots() -1)) {
                callWork0->SetFirstSlot(auxSlot);
                callWork0->SetLastSlot(auxSlot + callWork0->GetNumberSlots() - 1);
                callWork0->SetCore(0);

                call->SetRoute(call->GetRoute(k));
                call->SetModulation(FixedModulation);
                call->SetFirstSlot(callWork0->GetFirstSlot());
                call->SetLastSlot(callWork0->GetLastSlot());
                call->ClearTrialRoutes();
                call->ClearTrialProtRoutes();
                call->SetStatus(Accepted);
                resDevAlloc->simulType->GetData()->SetNonProtectedCalls(); //increment proCalls counter
                CalcBetaAverage(call);
                CalcAlpha(call);
                break;
            }
        }
    }*/
}

void DedicatedPathProtection::CreateProtectionCalls(CallDevices* call) {
    call->GetTranspSegments().clear();
    std::shared_ptr<Call> auxCall;
    std::vector<std::shared_ptr<Call>> auxVec(0);
    numSchProtRoutes = 2;
    
    for(unsigned a = 1; a <= numSchProtRoutes; a++){
        auxCall = std::make_shared<Call>(call->GetOrNode(), 
        call->GetDeNode(), call->GetBitRate(), call->GetDeactivationTime());
        
        //condition for squeezing 
        if(parameters->GetBeta() != 0 && a > numSchProtRoutes - 1){            
            double squeezingInd = (1 - (parameters->GetBeta()));
            double bitrate = call->GetBitRate();
            double protBitRate = ceil (squeezingInd * bitrate);
            auxCall->SetBitRate(protBitRate);
        } 
        
        auxVec.push_back(auxCall);
    }
    call->SetTranspSegments(auxVec);
}