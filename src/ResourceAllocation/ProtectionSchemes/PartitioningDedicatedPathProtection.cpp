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
#include <tuple>

#include "../../../include/ResourceAllocation/ProtectionSchemes/PartitioningDedicatedPathProtection.h"
#include "../../../include/ResourceAllocation/SA.h"
#include "../../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../../include/ResourceAllocation/Routing.h"
#include "../../../include/ResourceAllocation/Modulation.h"
#include "../../../include/Calls/Call.h"
#include "../../../include/Data/Parameters.h"
#include "../../../include/Calls/Traffic.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Data/Data.h"
#include "../../../include/Data/InputOutput.h"
#include "math.h" 

PartitioningDedicatedPathProtection::PartitioningDedicatedPathProtection
(ResourceDeviceAlloc* rsa): ProtectionScheme(rsa),PDPPBitRateDistOptions(0),
PDPPBitRateNodePairsDist(0) {

}

PartitioningDedicatedPathProtection::~PartitioningDedicatedPathProtection() {

}

void PartitioningDedicatedPathProtection::CreateProtectionRoutes() {
    
    routing->ProtectionDisjointYEN();
    this->CreatePDPPBitRateOptions();
}

void PartitioningDedicatedPathProtection::CreatePDPPBitRateOptions() {
    numSchProtRoutes = parameters->GetNumberPDPPprotectionRoutes();

    switch(resDevAlloc->options->GetProtectionOption()){
        case ProtectionPDPP:
            LoadPDPPBitRateOptions();
            break;
        case ProtectionPDPPBO_GA:
            break;
        default:
            std::cerr << "Invalid Protection Option" << std::endl;
            std::abort();
    }
    LoadPDPPBitRateNodePairDist();
}

void PartitioningDedicatedPathProtection::LoadPDPPBitRateOptions() {
    std::vector<double> VecTraffic = resDevAlloc->traffic->GetVecTraffic();
    std::vector<double> auxBitRateOption;
    double partialBitRate;
    double beta = parameters->GetBeta();

    for(auto it : VecTraffic){
        partialBitRate = ceil (((1 - beta) * it) / (numSchProtRoutes -1));
        for(unsigned int a = 0; a < numSchProtRoutes; a++){
            auxBitRateOption.push_back(partialBitRate);
        }
        PDPPBitRateDistOptions.push_back(auxBitRateOption);
        auxBitRateOption.clear();
    }
}

void PartitioningDedicatedPathProtection::LoadPDPPBitRateNodePairDist() {
    unsigned int NumNodes = topology->GetNumNodes();
    PDPPBitRateNodePairsDist.resize(NumNodes * NumNodes);

    switch(resDevAlloc->options->GetProtectionOption()){
        case ProtectionPDPP:
            for(int a = 0; a < PDPPBitRateNodePairsDist.size(); a++){
                PDPPBitRateNodePairsDist.at(a) = PDPPBitRateDistOptions;
            }
            break;
        case ProtectionPDPPBO_GA:
            this->SetPDPPBitRateNodePairDistGA();
            break;
        default:
            std::cerr << "Invalid Protection Option" << std::endl;
            std::abort();
    }
}

void PartitioningDedicatedPathProtection::ResourceAlloc(CallDevices* call) {

    switch(resDevAlloc->simulType->GetOptions()->GetOrderRSA()){
        case OrderRoutingSa:
            this->RoutingSpecPDPP(call);
            break;
        case OrderSaRouting:
            this->SpecRoutingPDPP(call);
            break;
        default:
            std::cout << "Invalid RSA Ordering" << std::endl;
    }
}

void PartitioningDedicatedPathProtection::RoutingSpecPDPP
(CallDevices* call) {      
    if(numSchProtRoutes == 3){
        this->routing->RoutingCall(call); //loading trialRoutes and trialprotRoutes
        unsigned int numRoutes = call->GetNumRoutes();

        this->CreateProtectionCalls(call); //loading transpsegments with protection calls

        //seting 3 protection calls to allocation
        std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
        std::shared_ptr<Call> callWork0 = callsVec.at(0);
        std::shared_ptr<Call> callWork1 = callsVec.at(1);
        std::shared_ptr<Call> callWork2 = callsVec.at(2);

        //try allocating with 3 routes
        for(unsigned int k = 0; k < numRoutes; k++){
            callWork0->SetRoute(call->GetRoute(k));
            callWork0->SetModulation(FixedModulation);
            std::deque<std::shared_ptr<Route>> ProtRoutes = call->GetProtRoutes(k);

            ProtRoutes.erase(std::remove(std::begin(ProtRoutes), std::end(ProtRoutes), nullptr),
                             std::end(ProtRoutes));
            unsigned int sizeProtRoutes = ProtRoutes.size();

            if(sizeProtRoutes >= 2){  //if to skip case which it is no routes enough
                for(unsigned int kd0 = 0; kd0 < sizeProtRoutes; kd0++) {

                    if(call->GetProtRoute(k , kd0)){  //if to avoid null route pointer
                        callWork1->SetRoute(call->GetProtRoute(k, kd0));
                        callWork1->SetModulation(FixedModulation);    

                        for(unsigned int kd1 = 0; kd1 < sizeProtRoutes; kd1++) {

                            if(kd0 == kd1)
                                continue;
                            callWork2->SetRoute(call->GetProtRoute(k, kd1));
                            callWork2->SetModulation(FixedModulation);

                            //calculate number of slots for the vector of calls
                            this->modulation->SetModulationParam(call);
                            //check if the number of slots are available in the 3 routes
                            this->resDevAlloc->specAlloc->SpecAllocation(call);

                            if(topology->IsValidLigthPath(call)){
                                call->SetRoute(call->GetRoute(k));
                                call->SetModulation(FixedModulation);
                                call->SetFirstSlot(callWork0->GetFirstSlot());
                                call->SetLastSlot(callWork0->GetLastSlot());
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
            }
        }

        //Delete one route, recalculate Bit rate and try allocating with 2 routes
        callsVec.pop_back();
        double callBitRate = call->GetBitRate();
        double beta = parameters->GetBeta();
        double partialBitRate = ceil (((1 - beta) * callBitRate) / (numSchProtRoutes-2));
        callWork0->SetBitRate(partialBitRate);
        callWork1->SetBitRate(partialBitRate);                
        call->SetTranspSegments(callsVec);        
        
        for(unsigned int k = 0; k < numRoutes; k++){
        callWork0->SetRoute(call->GetRoute(k));
        callWork0->SetModulation(FixedModulation);
        unsigned int sizeProtRoutes = call->GetProtRoutes(k).size();
        
            for(unsigned int kd = 0; kd < sizeProtRoutes; kd++) {

                if(call->GetProtRoute(k , kd)){  //if to avoid null route pointer
                    callWork1->SetRoute(call->GetProtRoute(k, kd));
                    callWork1->SetModulation(FixedModulation);

                    //calculate number of slots for the vector of calls (transpsegments)
                    this->modulation->SetModulationParam(call);

                    this->resDevAlloc->specAlloc->SpecAllocation(call);

                    if(topology->IsValidLigthPath(call)){
                        call->SetRoute(k);
                        call->SetModulation(FixedModulation);
                        call->SetFirstSlot(callWork0->GetFirstSlot());
                        call->SetLastSlot(callWork0->GetLastSlot());
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
        /*//Delete one route again, recalculate Bit rate and try allocating just 1
        //route (without protection)
        callsVec.pop_back();
        callWork0->SetBitRate(call->GetBitRate());
        call->SetTranspSegments(callsVec);    
     
        for(unsigned int k = 0; k < numRoutes; k++){
            callWork0->SetRoute(call->GetRoute(k));
            callWork0->SetModulation(FixedModulation);
            this->modulation->SetModulationParam(call);
            this->resDevAlloc->specAlloc->SpecAllocation(call);

            if(topology->IsValidLigthPath(call)){
                call->SetRoute(call->GetRoute(k));
                call->SetModulation(FixedModulation);
                call->SetFirstSlot(callWork0->GetFirstSlot());
                call->SetLastSlot(callWork0->GetLastSlot());
                call->ClearTrialRoutes();
                call->ClearTrialProtRoutes();
                call->SetStatus(Accepted);
                resDevAlloc->simulType->GetData()->SetNonProtectedCalls();
                return;
            }
        }*/
    }
    
    if(numSchProtRoutes == 2){
        this->routing->RoutingCall(call); //loading trialRoutes and trialprotRoutes
        unsigned int numRoutes = call->GetNumRoutes();

        this->CreateProtectionCalls(call); //loading transpsegments with calls

        //setting 2 protection calls to allocation
        std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
        std::shared_ptr<Call> callWork0 = callsVec.at(0);
        std::shared_ptr<Call> callWork1 = callsVec.at(1);

        //try allocating with 2 routes
        for(unsigned int k = 0; k < numRoutes; k++){
        callWork0->SetRoute(call->GetRoute(k));
        callWork0->SetModulation(FixedModulation);
        unsigned int sizeProtRoutes = call->GetProtRoutes(k).size();
        
            for(unsigned int kd = 0; kd < sizeProtRoutes; kd++) {

                if(call->GetProtRoute(k , kd)){  //if to avoid null route pointer
                    callWork1->SetRoute(call->GetProtRoute(k, kd));
                    callWork1->SetModulation(FixedModulation);

                    //calculate number of slots for the vector of calls (transpsegments)
                    this->modulation->SetModulationParam(call);

                    this->resDevAlloc->specAlloc->SpecAllocation(call);

                    if(topology->IsValidLigthPath(call)){
                        call->SetRoute(k);
                        call->SetModulation(FixedModulation);
                        call->SetFirstSlot(callWork0->GetFirstSlot());
                        call->SetLastSlot(callWork0->GetLastSlot());
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
        /*//Delete one route and try allocating just 1 route (without protection)
        callsVec.pop_back();
        callWork0->SetBitRate(call->GetBitRate());
        call->SetTranspSegments(callsVec);    
     
        for(unsigned int k = 0; k < numRoutes; k++){
            callWork0->SetRoute(call->GetRoute(k));
            callWork0->SetModulation(FixedModulation);
            this->modulation->SetModulationParam(call);
            this->resDevAlloc->specAlloc->SpecAllocation(call);

            if(topology->IsValidLigthPath(call)){
                call->SetRoute(call->GetRoute(k));
                call->SetModulation(FixedModulation);
                call->SetFirstSlot(callWork0->GetFirstSlot());
                call->SetLastSlot(callWork0->GetLastSlot());
                call->ClearTrialRoutes();
                call->ClearTrialProtRoutes();
                call->SetStatus(Accepted);
                resDevAlloc->simulType->GetData()->SetNonProtectedCalls();
                return;
            }
        } */
    }
}

void PartitioningDedicatedPathProtection::SpecRoutingPDPP(CallDevices* call) {
    if(numSchProtRoutes == 3){
        this->routing->RoutingCall(call); //loading trialRoutes and trialprotRoutes
        unsigned int numRoutes = call->GetNumRoutes();

        this->CreateProtectionCalls(call); //loading transpsegments with calls

        //seting 3 protection calls to allocation
        std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
        std::shared_ptr<Call> callWork0 = callsVec.at(0);
        std::shared_ptr<Call> callWork1 = callsVec.at(1);
        std::shared_ptr<Call> callWork2 = callsVec.at(2);

        //call->RepeatModulation();
        call->SetCore(0);
        bool allocCallWork0Found = false;
        bool allocCallWork1Found = false;
        bool allocCallWork2Found = false;
        unsigned int auxSlot0;
        unsigned int auxSlot1;
        unsigned int auxSlot2;
        const unsigned int topNumSlots = topology->GetNumSlots();
        std::vector<unsigned int> possibleSlots(0);
        possibleSlots = this->resDevAlloc->specAlloc->SpecAllocation();

        //slot loop for callWork0
        for (unsigned int s0 = 0; s0 < possibleSlots.size(); s0++) {
            auxSlot0 = possibleSlots.at(s0);

            //try allocating with 3 routes
            for (unsigned int k = 0; k < numRoutes; k++) {
                callWork0->SetRoute(call->GetRoute(k));
                callWork0->SetModulation(FixedModulation);

                //getting protection routes to use in next loop (FOR)
                std::deque<std::shared_ptr<Route>> ProtRoutes = call->GetProtRoutes(k);
                ProtRoutes.erase(std::remove(std::begin(ProtRoutes),
                                             std::end(ProtRoutes), nullptr), std::end(ProtRoutes));
                unsigned int sizeProtRoutes = ProtRoutes.size();

                //calculate number of slots for current of call
                this->modulation->SetModulationParam(callWork0.get());

                if (auxSlot0 + callWork0->GetNumberSlots() - 1 >= topNumSlots)
                    continue;
                //checking if callWork0 number of slots are available in its route
                if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(), auxSlot0,
                                     auxSlot0 + callWork0->GetNumberSlots() - 1)) {
                    callWork0->SetFirstSlot(auxSlot0);
                    callWork0->SetLastSlot(auxSlot0 + callWork0->GetNumberSlots() - 1);
                    callWork0->SetCore(0);

                    if (sizeProtRoutes >= 2) {  //if to skip case which it is no routes enough

                        //slot loop for callWork1
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
                                        callWork1->SetLastSlot(auxSlot1 + callWork1->GetNumberSlots() -1);
                                        callWork1->SetCore(0);

                                        //slot loop for callWork2
                                        for (unsigned int s2 = 0;
                                             s2 < possibleSlots.size(); s2++) {
                                            auxSlot2 = possibleSlots.at(s2);

                                            for (unsigned int kd1 = 0;
                                                 kd1 < sizeProtRoutes; kd1++) {
                                                if (kd0 == kd1)
                                                    continue;
                                                callWork2->SetRoute(
                                                        call->GetProtRoute(k, kd1));
                                                callWork2->SetModulation(FixedModulation);

                                                this->modulation->SetModulationParam(
                                                        callWork2.get());

                                                if (auxSlot2 +
                                                    callWork2->GetNumberSlots() - 1 >=
                                                    topNumSlots)
                                                    continue;
                                                //checking if callWork2 slots are available in its route
                                                if (this->resDevAlloc->CheckSlotsDisp(
                                                        callWork2->GetRoute(), auxSlot2,
                                                        auxSlot2 +
                                                        callWork2->GetNumberSlots() -
                                                        1)) {
                                                    callWork2->SetFirstSlot(auxSlot2)
                                                    ;
                                                    callWork2->SetLastSlot(auxSlot2 +
                                                                           callWork2->GetNumberSlots() -
                                                                           1);
                                                    callWork2->SetCore(0);

                                                    call->SetRoute(call->GetRoute(k));
                                                    call->SetModulation(FixedModulation);
                                                    call->SetFirstSlot(
                                                            callWork0->GetFirstSlot());
                                                    call->SetLastSlot(
                                                            callWork0->GetLastSlot());
                                                    call->ClearTrialRoutes();
                                                    call->ClearTrialProtRoutes();
                                                    call->SetStatus(Accepted);
                                                    resDevAlloc->simulType->GetData()->SetProtectedCalls(); //increment proCalls counter
                                                    CalcBetaAverage(call);
                                                    CalcAlpha(call);
                                                    allocCallWork0Found = true;
                                                    allocCallWork1Found = true;
                                                    allocCallWork2Found = true;
                                                    break;
                                                }
                                            }
                                            if(allocCallWork2Found)
                                                break;
                                        }
                                    }
                                }
                                if (allocCallWork1Found)
                                    break;
                            }
                            if (allocCallWork1Found)
                                break;
                        }
                    }
                }
                if(allocCallWork0Found)
                    break;
            }
            if(allocCallWork0Found)
                break;
        }

        if(allocCallWork2Found == false) {
            //Delete one route, recalculate Bit rate and try allocating with 2 routes
            callsVec.pop_back();
            double callBitRate = call->GetBitRate();
            double beta = parameters->GetBeta();
            double partialBitRate = ceil(
                    ((1 - beta) * callBitRate) / (numSchProtRoutes - 2));
            callWork0->SetBitRate(partialBitRate);
            callWork1->SetBitRate(partialBitRate);
            call->SetTranspSegments(callsVec);

            //slot loop for callWork0
            for (unsigned int s0 = 0; s0 < possibleSlots.size(); s0++) {
                auxSlot0 = possibleSlots.at(s0);

                for (unsigned int k = 0; k < numRoutes; k++) {
                    callWork0->SetRoute(call->GetRoute(k));
                    callWork0->SetModulation(FixedModulation);

                    //getting protection routes to use in next loop (FOR)
                    std::deque <std::shared_ptr<Route>> ProtRoutes = call->GetProtRoutes(
                            k);
                    ProtRoutes.erase(std::remove(std::begin(ProtRoutes),
                                                 std::end(ProtRoutes), nullptr),
                                     std::end(ProtRoutes));
                    unsigned int sizeProtRoutes = ProtRoutes.size();

                    //calculate number of slots for current of call
                    this->modulation->SetModulationParam(callWork0.get());

                    if (auxSlot0 + callWork0->GetNumberSlots() - 1 >= topNumSlots)
                        continue;
                    //checking if callWork0 number of slots are available in its route
                    if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(), auxSlot0,
                                                          auxSlot0 +
                                                          callWork0->GetNumberSlots() -
                                                          1)) {
                        callWork0->SetFirstSlot(auxSlot0);
                        callWork0->SetLastSlot(auxSlot0 + callWork0->GetNumberSlots() - 1);
                        callWork0->SetCore(0);

                        if (sizeProtRoutes >= 2) {  //if to skip case which it is no routes enough

                            //slot loop for callWork1
                            for (unsigned int s1 = 0; s1 < possibleSlots.size(); s1++) {
                                auxSlot1 = possibleSlots.at(s1);
                                for (unsigned int kd0 = 0; kd0 < sizeProtRoutes; kd0++) {
                                    if (call->GetProtRoute(k,
                                                           kd0)) {  //if to avoid null route pointer
                                        callWork1->SetRoute(call->GetProtRoute(k, kd0));
                                        callWork1->SetModulation(FixedModulation);

                                        this->modulation->SetModulationParam(
                                                callWork1.get());

                                        if (auxSlot1 + callWork1->GetNumberSlots() - 1 >=
                                            topNumSlots)
                                            continue;
                                        //checking if callWork1 slots are available in its route
                                        if (this->resDevAlloc->CheckSlotsDisp(
                                                callWork1->GetRoute(), auxSlot1,
                                                auxSlot1 + callWork1->GetNumberSlots() -
                                                1)) {
                                            callWork1->SetFirstSlot(auxSlot1);
                                            callWork1->SetLastSlot(
                                                    auxSlot1 +
                                                    callWork1->GetNumberSlots() - 1);
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
                                                      auxSlot +
                                                      callWork0->GetNumberSlots() -
                                                      1)) {
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

    if(numSchProtRoutes == 2){
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
                                                  auxSlot +
                                                  callWork0->GetNumberSlots() -
                                                  1)) {
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

void PartitioningDedicatedPathProtection::SpecRoutingSameSlotPDPP(CallDevices* call) {
    if(numSchProtRoutes == 3){
        this->routing->RoutingCall(call); //loading trialRoutes and trialprotRoutes
        unsigned int numRoutes = call->GetNumRoutes();

        this->CreateProtectionCalls(call); //loading transpsegments with calls

        //seting 3 protection calls to allocation
        std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
        std::shared_ptr<Call> callWork0 = callsVec.at(0);
        std::shared_ptr<Call> callWork1 = callsVec.at(1);
        std::shared_ptr<Call> callWork2 = callsVec.at(2);

        //call->RepeatModulation();
        call->SetCore(0);
        bool allocCallWork0Found = false;
        bool allocCallWork1Found = false;
        bool allocCallWork2Found = false;
        const unsigned int topNumSlots = topology->GetNumSlots();
        std::vector<unsigned int> possibleSlots(0);
        possibleSlots = this->resDevAlloc->specAlloc->SpecAllocation();
        unsigned int auxSlot;

        for (unsigned int s = 0; s < possibleSlots.size(); s++) {
            auxSlot = possibleSlots.at(s);

            //try allocating with 3 routes
            for (unsigned int k = 0; k < numRoutes; k++) {
                callWork0->SetRoute(call->GetRoute(k));
                callWork0->SetModulation(FixedModulation);

                //getting protection routes to use in next loop (FOR)
                std::deque<std::shared_ptr<Route>> ProtRoutes = call->GetProtRoutes(k);
                ProtRoutes.erase(std::remove(std::begin(ProtRoutes),
                       std::end(ProtRoutes), nullptr), std::end(ProtRoutes));
                unsigned int sizeProtRoutes = ProtRoutes.size();

                //calculate number of slots for current of call
                this->modulation->SetModulationParam(callWork0.get());

                if (auxSlot + callWork0->GetNumberSlots() - 1 >= topNumSlots)
                    continue;
                //checking if callWork0 number of slots are available in its route
                if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(), auxSlot,
                                         auxSlot + callWork0->GetNumberSlots() - 1)) {
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

                                    for (unsigned int kd1 = 0; kd1 < sizeProtRoutes; kd1++) {
                                        if (kd0 == kd1)
                                            continue;
                                        callWork2->SetRoute(call->GetProtRoute(k, kd1));
                                        callWork2->SetModulation(FixedModulation);

                                        this->modulation->SetModulationParam(callWork2.get());

                                        if (auxSlot + callWork2->GetNumberSlots() - 1 >= topNumSlots)
                                            continue;
                                        //checking if callWork2 slots are available in its route
                                        if (this->resDevAlloc->CheckSlotsDisp(callWork2->GetRoute(), auxSlot,
                                                    auxSlot + callWork2->GetNumberSlots() - 1)) {
                                            callWork2->SetFirstSlot(auxSlot);
                                            callWork2->SetLastSlot(auxSlot + callWork2->GetNumberSlots() - 1);
                                            callWork2->SetCore(0);

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
                                            allocCallWork2Found = true;
                                            break;
                                        }
                                    }
                                }
                            }
                            if(allocCallWork1Found)
                                break;
                        }
                    }
                }
                if(allocCallWork0Found)
                    break;
            }
            if(allocCallWork2Found)
                break;
        }

        if(allocCallWork2Found == false) {
            //Delete one route, recalculate Bit rate and try allocating with 2 routes
            callsVec.pop_back();
            double callBitRate = call->GetBitRate();
            double beta = parameters->GetBeta();
            double partialBitRate = ceil(
                    ((1 - beta) * callBitRate) / (numSchProtRoutes - 2));
            callWork0->SetBitRate(partialBitRate);
            callWork1->SetBitRate(partialBitRate);
            call->SetTranspSegments(callsVec);

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
                                                 std::end(ProtRoutes), nullptr),
                                     std::end(ProtRoutes));
                    unsigned int sizeProtRoutes = ProtRoutes.size();

                    //calculate number of slots for current of call
                    this->modulation->SetModulationParam(callWork0.get());

                    if (auxSlot + callWork0->GetNumberSlots() - 1 >= topNumSlots)
                        continue;
                    //checking if callWork0 number of slots are available in its route
                    if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(), auxSlot,
                                                          auxSlot +
                                                          callWork0->GetNumberSlots() -
                                                          1)) {
                        callWork0->SetFirstSlot(auxSlot);
                        callWork0->SetLastSlot(auxSlot + callWork0->GetNumberSlots() - 1);
                        callWork0->SetCore(0);

                        if (sizeProtRoutes >=
                            2) {  //if to skip case which it is no routes enough
                            for (unsigned int kd0 = 0; kd0 < sizeProtRoutes; kd0++) {
                                if (call->GetProtRoute(k,
                                                       kd0)) {  //if to avoid null route pointer
                                    callWork1->SetRoute(call->GetProtRoute(k, kd0));
                                    callWork1->SetModulation(FixedModulation);

                                    this->modulation->SetModulationParam(callWork1.get());

                                    if (auxSlot + callWork1->GetNumberSlots() - 1 >=
                                        topNumSlots)
                                        continue;
                                    //checking if callWork1 slots are available in its route
                                    if (this->resDevAlloc->CheckSlotsDisp(
                                            callWork1->GetRoute(), auxSlot,
                                            auxSlot + callWork1->GetNumberSlots() - 1)) {
                                        callWork1->SetFirstSlot(auxSlot);
                                        callWork1->SetLastSlot(
                                                auxSlot + callWork1->GetNumberSlots() - 1);
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
                                                      auxSlot +
                                                      callWork0->GetNumberSlots() -
                                                      1)) {
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

    if(numSchProtRoutes == 2){
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
                                                  auxSlot +
                                                  callWork0->GetNumberSlots() -
                                                  1)) {
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

void PartitioningDedicatedPathProtection::CreateProtectionCalls(CallDevices* call) {   
    unsigned int orN = call->GetOrNode()->GetNodeId();
    unsigned int deN = call->GetDeNode()->GetNodeId();
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int nodePairIndex = orN * numNodes + deN;
    call->GetTranspSegments().clear();    
    std::vector<double> VecTraffic = resDevAlloc->traffic->GetVecTraffic();
    double callBitRate = call->GetBitRate();
    unsigned int trafficIndex;
    
    for(unsigned int a = 0; a < VecTraffic.size(); a++){
        if(callBitRate == VecTraffic.at(a))
            trafficIndex = a;
    }  
    
    std::shared_ptr<Call> auxCall;
    std::vector<std::shared_ptr<Call>> auxVec(0);
        
    for(unsigned int a = 0; a < numSchProtRoutes; a++){
        auxCall = std::make_shared<Call>(call->GetOrNode(),                     
        call->GetDeNode(), PDPPBitRateNodePairsDist.at(nodePairIndex).at
        (trafficIndex).at(a), call->GetDeactivationTime());             
        auxVec.push_back(auxCall);
    }
    call->SetTranspSegments(auxVec); 
}

std::vector<std::vector<std::vector<double>>> PartitioningDedicatedPathProtection::
GetPDPPBitRateNodePairsDist() const {
    return PDPPBitRateNodePairsDist;

}

void PartitioningDedicatedPathProtection::SetPDPPBitRateNodePairsDist
(std::vector<std::vector<std::vector<double>>> PDPPBitRateNodePairsDist) {
    this->PDPPBitRateNodePairsDist = PDPPBitRateNodePairsDist;
}

void PartitioningDedicatedPathProtection::SetPDPPBitRateNodePairDistGA(){
    std::ifstream auxIfstream;
    double auxBR = 0;
    unsigned int numNodes = this->topology->GetNumNodes();
    this->resDevAlloc->GetSimulType()->GetInputOutput()->
    LoadPDPPBitRateNodePairsDistFirstSimul(auxIfstream);
    unsigned int numTraffic = resDevAlloc->GetSimulType()->GetTraffic()->
    GetVecTraffic().size();
    unsigned int numPDPPRoutes = resDevAlloc->GetSimulType()->GetParameters()->
    GetNumberPDPPprotectionRoutes();
    std::vector<std::vector<std::vector<double>>> auxPDPPBitRateNodePairsDist;
    auxPDPPBitRateNodePairsDist.resize(numNodes*numNodes);

    for(unsigned int a = 0; a < numNodes*numNodes; a++){
        auxPDPPBitRateNodePairsDist.at(a).resize(numTraffic);
        for(unsigned int trIndex = 0; trIndex < numTraffic; trIndex++) {
            for(unsigned int numRoutes = 0; numRoutes < numPDPPRoutes; numRoutes++){
                auxIfstream >> auxBR;
                auxPDPPBitRateNodePairsDist.at(a).at(trIndex).push_back(auxBR);
            }
        }
    }
    this->SetPDPPBitRateNodePairsDist(auxPDPPBitRateNodePairsDist);
}

