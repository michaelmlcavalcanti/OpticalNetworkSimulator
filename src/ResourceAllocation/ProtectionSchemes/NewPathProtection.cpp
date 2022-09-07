//
// Created by michaelcavalcanti on 07/09/22.
//

#include <vector>
#include <memory>

#include "../../../include/ResourceAllocation/ProtectionSchemes/NewPathProtection.h"
#include "../../../include/ResourceAllocation/SA.h"
#include "../../../include/Calls/Traffic.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Data/Data.h"
#include "../../../include/Data/InputOutput.h"
#include "math.h"

NewPathProtection::NewPathProtection
        (ResourceDeviceAlloc* rsa): ProtectionScheme(rsa), BitRateDistOptions(0), BitRateNodePairsDist(0) {

}

NewPathProtection::~NewPathProtection() {

}

void NewPathProtection::CreateProtectionRoutes(){
    this->routing->MultiPathRouting();
    this->CreateBitRateOptions();
}

void NewPathProtection::CreateBitRateOptions(){
    int numSchProtoRoutes = parameters->GetNumberPDPPprotectionRoutes();
    LoadBitRateOptions(numSchProtoRoutes);
    LoadBitRateNodePairDist();
}

void NewPathProtection::LoadBitRateOptions(int numSchProtoRoutes){
    std::vector<double> VecTraffic = resDevAlloc->traffic->GetVecTraffic();
    std::vector<double> auxBitRateOption;
    double partialBitRate;
    double beta = parameters->GetBeta();

    for(auto it : VecTraffic){
        partialBitRate = ceil (((1 - beta) * it) / (numSchProtoRoutes -1));
        for(unsigned int a = 0; a < numSchProtoRoutes; a++){
            auxBitRateOption.push_back(partialBitRate);
        }
        BitRateDistOptions.push_back(auxBitRateOption);
        auxBitRateOption.clear();
    }
}

void NewPathProtection::LoadBitRateNodePairDist(){
    unsigned int NumNodes = topology->GetNumNodes();
    BitRateNodePairsDist.resize(NumNodes * NumNodes);
    for(int a = 0; a < BitRateNodePairsDist.size(); a++){
        BitRateNodePairsDist.at(a) = BitRateDistOptions;
    }
}

void NewPathProtection::CreateProtectionCalls(CallDevices* call){
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
                                         call->GetDeNode(), BitRateNodePairsDist.at(nodePairIndex).at
                        (trafficIndex).at(a), call->GetDeactivationTime());
        auxVec.push_back(auxCall);
    }
    call->SetTranspSegments(auxVec);
}

void NewPathProtection::ResourceAlloc(CallDevices* call){
    this->RoutingSpec(call);

}

void NewPathProtection::RoutingSpec(CallDevices* call){


}

std::vector<std::vector<std::vector<double>>> NewPathProtection::
GetBitRateNodePairsDist() const {
    return BitRateNodePairsDist;

}

void NewPathProtection::SetBitRateNodePairsDist
        (std::vector<std::vector<std::vector<double>>> BitRateNodePairsDist) {
    this->BitRateNodePairsDist = BitRateNodePairsDist;
}