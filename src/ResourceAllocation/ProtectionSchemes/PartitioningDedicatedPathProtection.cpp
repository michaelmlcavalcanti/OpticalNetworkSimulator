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
    numSchProtRoutes = 3;

    switch(resDevAlloc->options->GetProtectionOption()){
        case ProtectionPDPP:
            Function();
            break;
        case ProtectionEPDPP_GA:
            break;
        default:
            std::cerr << "Invalid Protection Option" << std::endl;
            std::abort();
    }
    LoadPDPPBitRateNodePairDist();
}

void PartitioningDedicatedPathProtection::Function() {
    std::vector<double> VecTraffic = resDevAlloc->traffic->GetVecTraffic();
    std::vector<double> auxBitRateOption;
    double partialBitRate;
    double beta = parameters->GetBeta();

    for(auto it : VecTraffic){
        partialBitRate = ceil (((it)/(numSchProtRoutes -1)) -
                               (((beta) * (it)) / (numSchProtRoutes -1)));
        for(unsigned int a = 0; a < numSchProtRoutes;a++){
            auxBitRateOption.push_back(partialBitRate);
        }
        PDPPBitRateDistOptions.push_back(auxBitRateOption);
        auxBitRateOption.clear();
    }
}

void PartitioningDedicatedPathProtection::LoadPDPPBitRateOptions(int PDPPType) {
    std::vector<double> VecTraffic;
    VecTraffic = resDevAlloc->traffic->GetVecTraffic();
    std::vector<double> auxBitRateOption;
    double partialBitRate;
    double beta = parameters->GetBeta();

    if(PDPPType == 0){
        for(auto it : VecTraffic){
            partialBitRate = ceil (((it)/(numSchProtRoutes -1)) -
            (((beta) * (it)) / (numSchProtRoutes -1)));
            for(unsigned int a = 0; a < numSchProtRoutes;a++){
                auxBitRateOption.push_back(partialBitRate);                        
            }
        PDPPBitRateDistOptions.push_back(auxBitRateOption);
        auxBitRateOption.clear();
        }
    }
    else if(PDPPType == 1){
        if(beta != 0){
            for(auto it : VecTraffic){
                double BRdown = ((it/2) - (beta*it));
                double BRup = ((it/2) + (beta*it));
                double BRmin = ((it*(1 - beta)));
                   
                for(double a = BRdown; a <= BRup; a = a+5e9){
                    for(double b = BRdown; b <= BRup; b = b+5e9){
                        for(double c = BRdown; c <= BRup; c = c+5e9){
                            if(a + b >= BRmin && b + c >= BRmin && a + c >= BRmin){
                                auxBitRateOption.push_back(a);
                                auxBitRateOption.push_back(b);
                                auxBitRateOption.push_back(c);
                            }
                        }
                    }
                }
                PDPPBitRateDistOptions.push_back(auxBitRateOption);
            }
        }
        else{
            for(auto it : VecTraffic){
                partialBitRate = ceil (((it)/(numSchProtRoutes -1)) -
                                       (((beta) * (it)) / (numSchProtRoutes -1)));
                for(unsigned int a = 0; a < numSchProtRoutes;a++){
                    auxBitRateOption.push_back(partialBitRate);
                }
                PDPPBitRateDistOptions.push_back(auxBitRateOption);
                auxBitRateOption.clear();
            }
        }
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
        case ProtectionEPDPP_GA:
            //Criar função de load do indivíduo do GA
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

    this->routing->RoutingCall(call); //loading trialRoutes and trialprotRoutes
    
    this->CreateProtectionCalls(call); //loading transpsegments with calls
    
    std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
    std::shared_ptr<Call> callWork0 = callsVec.at(0);
    std::shared_ptr<Call> callWork1 = callsVec.at(1);
    std::shared_ptr<Call> callWork2 = callsVec.at(2);
    unsigned int numRoutes = call->GetNumRoutes();

    for(unsigned int k = 0; k < numRoutes; k++){
        callWork0->SetRoute(call->GetRoute(k));
        callWork0->SetModulation(FixedModulation);
//        unsigned int sizeProtRoutes = call->GetProtRoutes(k).size();
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
                            call->SetFirstSlot(callWork0->GetFirstSlot());
                            call->SetLastSlot(callWork0->GetLastSlot());
                            call->ClearTrialRoutes();
                            call->ClearTrialProtRoutes();
                            call->SetStatus(Accepted);
                            IncrementNumProtectedCalls();
                            resDevAlloc->simulType->GetData()->SetProtectedCalls
                            (this->numProtectedCalls);
                            return;           
                        }
                    }
                }
            }
        }
    }

    //Delete protection route (Corrigir)
    for(int a = 0; a < numSchProtRoutes - 1; a++){
        callsVec.pop_back();
    }
    call->SetTranspSegments(callsVec);
    
    //Try only work connection
    for(unsigned int k = 0; k < numRoutes; k++){
        callWork0->SetRoute(call->GetRoute(k));
        callWork0->SetModulation(FixedModulation);
        this->modulation->SetModulationParam(call);
        this->resDevAlloc->specAlloc->SpecAllocation(call);
        
        if(topology->IsValidLigthPath(call)){
            call->SetRoute(call->GetRoute(k));
            call->SetFirstSlot(callWork0->GetFirstSlot());
            call->SetLastSlot(callWork0->GetLastSlot());
            call->ClearTrialRoutes();
            call->ClearTrialProtRoutes();
            call->SetStatus(Accepted);
            IncrementNumNonProtectedCalls();
            resDevAlloc->simulType->GetData()->SetNonProtectedCalls
            (this->numNonProtectedCalls);
            return;
        }
    } 
}

void PartitioningDedicatedPathProtection::CreateProtectionCalls(CallDevices* call) {
    unsigned int orN = call->GetOrNode()->GetNodeId();
    unsigned int deN = call->GetDeNode()->GetNodeId();
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int nodePairIndex = orN * numNodes + deN;
    call->GetTranspSegments().clear();
    numSchProtRoutes = 3;
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


