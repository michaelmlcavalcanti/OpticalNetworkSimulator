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

    this->RoutingOffNoSameSlotProtPDPPSpecAlloc(call);
}

void PartitioningDedicatedPathProtection::RoutingOffNoSameSlotProtPDPPSpecAlloc
(CallDevices* call) {      
    if(numSchProtRoutes == 3){
        this->routing->RoutingCall(call); //loading trialRoutes and trialprotRoutes

        this->CreateProtectionCalls(call); //loading transpsegments with calls

        //try allocating with 3 routes
        std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
        std::shared_ptr<Call> callWork0 = callsVec.at(0);
        std::shared_ptr<Call> callWork1 = callsVec.at(1);
        std::shared_ptr<Call> callWork2 = callsVec.at(2);
        unsigned int numRoutes = call->GetNumRoutes();

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

                            //calculate number of slots for the vector of calls (transpsegments)
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
                                resDevAlloc->simulType->GetData()->SetProtectedCalls();
                                CalcBetaAverage(call);
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

        this->CreateProtectionCalls(call); //loading transpsegments with calls

        //try allocating with 2 routes
        std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
        std::shared_ptr<Call> callWork0 = callsVec.at(0);
        std::shared_ptr<Call> callWork1 = callsVec.at(1);
        unsigned int numRoutes = call->GetNumRoutes();
        
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
                        return;
                    }
                }
            }
        }
        /*//Delete one route, recalculate Bit rate and try allocating just 1
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
        } */
    }
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

