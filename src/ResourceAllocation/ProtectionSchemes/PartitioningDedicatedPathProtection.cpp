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
(ResourceDeviceAlloc* rsa): ProtectionScheme(rsa), NodePairPDPPBitRateDist(0) {

}

PartitioningDedicatedPathProtection::~PartitioningDedicatedPathProtection() {

}

void PartitioningDedicatedPathProtection::CreateProtectionRoutes() {
    
    routing->ProtectionDisjointYEN();
    this->CreatePDPPBitRateOptions();
}

void PartitioningDedicatedPathProtection::CreatePDPPBitRateOptions() {
    unsigned int numNodes = topology->GetNumNodes();
    PDPPBitRateDistribution.resize(numNodes * numNodes);

    switch(resDevAlloc->options->GetProtectionOption()){
        case ProtectionPDPP:
            PDPPBitRateDistribution.assign(numNodes*numNodes, 0);
            break;
        case ProtectionEPDPP_GA:
            PDPPBitRateDistribution.assign(numNodes*numNodes, 1);
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
        unsigned int sizeProtRoutes = call->GetProtRoutes(k).size();
        
        if(sizeProtRoutes >= 2){
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
                            return;           
                        }
                    }
                }
            }
        }
    }
    
    //Delete protection route
    
    for(int a = 0; a < 2; a++){
        call->GetTranspSegmentsVec().pop_back();
    }
    
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
            return;
        }
    } 
}

void PartitioningDedicatedPathProtection::CreateProtectionCalls(CallDevices* call) {
    call->GetTranspSegments().clear();
    numSchProtRoutes = 3;

    this->CalcPDPPBitRate(call);
        
    std::shared_ptr<Call> auxCall;
    std::vector<std::shared_ptr<Call>> auxVec(0);
        
    for(unsigned a = 0; a < numSchProtRoutes; a++){
        auxCall = std::make_shared<Call>(call->GetOrNode(), 
        call->GetDeNode(), NodePairPDPPBitRateDist.at(a), call->GetDeactivationTime());        
        auxVec.push_back(auxCall);
    }
    
    call->SetTranspSegments(auxVec);
    this->NodePairPDPPBitRateDist.clear();
}

void PartitioningDedicatedPathProtection::CalcPDPPBitRate(CallDevices* call) {
    unsigned int orN = call->GetOrNode()->GetNodeId();
    unsigned int deN = call->GetDeNode()->GetNodeId();
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int nodePairIndex = orN * numNodes + deN;
    unsigned int NodePairBitRateOption = this->PDPPBitRateDistribution.at(nodePairIndex);
    double partialBitRate;
    double callBitRate = call->GetBitRate();
    double beta = parameters->GetBeta();

   /* switch(NodePairBitRateOption){
        case 0:
            partialBitRate = ceil (((callBitRate)/(numSchProtRoutes -1)) -
            (((beta) * (callBitRate)) / (numSchProtRoutes -1)));

            for(int a = 0; a < numSchProtRoutes; a++){
                NodePairPDPPBitRateDist.push_back(partialBitRate);
            }
            break;
        case != 0:
            if(beta != 0){
                double BRdown = ((callBitRate/2) - (beta*callBitRate));
                double BRup = ((callBitRate/2) + (beta*callBitRate));
                double BRmin = ((callBitRate*(1 - beta)));
                std::vector<std::vector<double>> auxVecBR;
                std::vector<double> auxVecBR1;

                for(double a = BRdown; a <= BRup; a = a+5e9){
                    for(double b = BRdown; b <= BRup; b = b+5e9){
                        for(double c = BRdown; c <= BRup; c = c+5e9){
                            if(a + b >= BRmin && b + c >= BRmin && a + c >= BRmin){
                                auxVecBR1.push_back(a);
                                auxVecBR1.push_back(b);
                                auxVecBR1.push_back(c);
                                auxVecBR.push_back(auxVecBR1);
                                auxVecBR1.clear();
                            }
                        }
                    }
                }
            }
            else{
                partialBitRate = ceil (((callBitRate)/(numSchProtRoutes -1)) -
                (((beta) * (callBitRate)) / (numSchProtRoutes -1)));

                for(int a = 0; a < numSchProtRoutes; a++){
                    NodePairPDPPBitRateDist.push_back(partialBitRate);
                }
            }
        default:
            std::cerr << "Invalid Bit Rate Option" << std::endl;
            std::abort();
    }*/

    if(NodePairBitRateOption == 0){
        partialBitRate = ceil (((callBitRate)/(numSchProtRoutes -1)) -
        (((beta) * (callBitRate)) / (numSchProtRoutes -1)));

        for(int a = 0; a < numSchProtRoutes; a++)
            NodePairPDPPBitRateDist.push_back(partialBitRate);
    }
    else{
        if(beta != 0){
            double BRdown = ((callBitRate/2) - (beta*callBitRate));
            double BRup = ((callBitRate/2) + (beta*callBitRate));
            double BRmin = ((callBitRate*(1 - beta)));
            std::vector<std::vector<double>> auxVecBR;
            std::vector<double> auxVecBR1;

            for(double a = BRdown; a <= BRup; a = a+5e9){
                for(double b = BRdown; b <= BRup; b = b+5e9){
                    for(double c = BRdown; c <= BRup; c = c+5e9){
                        if(a + b >= BRmin && b + c >= BRmin && a + c >= BRmin){
                            auxVecBR1.push_back(a);
                            auxVecBR1.push_back(b);
                            auxVecBR1.push_back(c);
                            auxVecBR.push_back(auxVecBR1);
                            auxVecBR1.clear();
                        }
                    }
                }
            }
            for(int a = 0; a < numSchProtRoutes; a++){
                partialBitRate = auxVecBR.at(NodePairBitRateOption).at(a);
                NodePairPDPPBitRateDist.push_back(partialBitRate);
            }
        }
        else{
            partialBitRate = ceil (((callBitRate)/(numSchProtRoutes -1)) -
            (((beta) * (callBitRate)) / (numSchProtRoutes -1)));

            for(int a = 0; a < numSchProtRoutes; a++)
                NodePairPDPPBitRateDist.push_back(partialBitRate);
        }
    }
}


