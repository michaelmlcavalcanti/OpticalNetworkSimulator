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

void NewPathProtection::ResourceAlloc(CallDevices* call){

    bool callProtect = reinterpret_cast<bool *>(call->isProtected());

    if(callProtect == true){
        bool allocatedCallProtectedWithThreeRoutes = this->RoutingCallProtectedWithThreeRoutes(call);
        if(allocatedCallProtectedWithThreeRoutes == false){
            this->RoutingCallWithTwoRoutes(call);
        }
    }else{
        bool allocatedCallUnprotectedWithOneRoute = this->RoutingCallUnprotectedWithOneRoute(call);
        if(allocatedCallUnprotectedWithOneRoute == false){
            this->RoutingCallWithTwoRoutes(call);
        }
    }
}

bool NewPathProtection::RoutingCallProtectedWithThreeRoutes(CallDevices* call){
    this->CreateProtectionCalls(call); //loading transpsegments with calls

    //setting 3 protection calls to allocation
    std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
    std::shared_ptr<Call> callWork0 = callsVec.at(0);
    std::shared_ptr<Call> callWork1 = callsVec.at(1);
    std::shared_ptr<Call> callWork2 = callsVec.at(2);

    call->SetCore(0);
    unsigned int auxSlot;
    unsigned int sumFirstSlots = 0;
    const unsigned int topNumSlots = topology->GetNumSlots();
    std::vector<unsigned int> possibleSlots(0);
    std::vector<int> firstSlotIndexesSum(0);
    std::vector<std::vector<int>> firstSlotIndexes(0);
    possibleSlots = this->resDevAlloc->specAlloc->SpecAllocation();
    unsigned int orN = call->GetOrNode()->GetNodeId();
    unsigned int deN = call->GetDeNode()->GetNodeId();
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int nodePairIndex = orN * numNodes + deN;
    bool callAllocated = false;
    unsigned int groupIndex = 0;

    //trying allocate call with 3 routes
    if (!resources->protectionAllRoutesGroups.at(nodePairIndex).front().empty()) {
        unsigned int numGroups = resources->protectionAllRoutesGroups.at(
                nodePairIndex).front().size();
        firstSlotIndexes.resize(numGroups);
        firstSlotIndexesSum.resize(numGroups);
        //computing the first slot indexes available of each group for current call and its sum
        for (auto &group3: resources->protectionAllRoutesGroups.at(
                nodePairIndex).front()) {
//                if(groupIndex == parameters->GetNumberRoutes())
//                    break;
            bool allocCallWork0Found = false;
            bool allocCallWork1Found = false;
            bool allocCallWork2Found = false;
            sumFirstSlots = 0;
            callWork0->SetRoute(group3.at(0));
            callWork0->SetModulation(FixedModulation);
            this->modulation->SetModulationParam(callWork0.get());
            for (unsigned int s = 0; s < possibleSlots.size(); s++) {
                auxSlot = possibleSlots.at(s);
                if (auxSlot + callWork0->GetNumberSlots() - 1 >= topNumSlots)
                    break;
                if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(), auxSlot,
                                                      auxSlot + callWork0->GetNumberSlots() - 1, occupied)) {
                    firstSlotIndexes.at(groupIndex).push_back(auxSlot);
                    sumFirstSlots = auxSlot;
                    allocCallWork0Found = true;
                    break;
                }
            }
            if (allocCallWork0Found == false) {
                firstSlotIndexesSum.at(groupIndex) = Def::Max_Int;
            }
            if (allocCallWork0Found == true) {
                callWork1->SetRoute(group3.at(1));
                callWork1->SetModulation(FixedModulation);
                this->modulation->SetModulationParam(callWork1.get());
                for (unsigned int s = 0; s < possibleSlots.size(); s++) {
                    auxSlot = possibleSlots.at(s);
                    if (auxSlot + callWork1->GetNumberSlots() - 1 >= topNumSlots)
                        break;
                    if (this->resDevAlloc->CheckSlotsDisp(callWork1->GetRoute(),
                                                          auxSlot, auxSlot + callWork1->GetNumberSlots() - 1, occupied)) {
                        firstSlotIndexes.at(groupIndex).push_back(auxSlot);
                        sumFirstSlots += auxSlot;
                        allocCallWork1Found = true;
                        break;
                    }
                }
            }
            if (allocCallWork1Found == false) {
                firstSlotIndexesSum.at(groupIndex) = Def::Max_Int;
            }
            if (allocCallWork1Found == true) {
                callWork2->SetRoute(group3.at(2));
                callWork2->SetModulation(FixedModulation);
                this->modulation->SetModulationParam(callWork2.get());
                for (unsigned int s = 0; s < possibleSlots.size(); s++) {
                    auxSlot = possibleSlots.at(s);
                    if (auxSlot + callWork2->GetNumberSlots() - 1 >= topNumSlots)
                        break;
                    if (this->resDevAlloc->CheckSlotsDisp(callWork2->GetRoute(),
                                                          auxSlot, auxSlot + callWork2->GetNumberSlots() - 1, reserved)) {
                        firstSlotIndexes.at(groupIndex).push_back(auxSlot);
                        sumFirstSlots += auxSlot;
                        firstSlotIndexesSum.at(groupIndex) = sumFirstSlots;
                        allocCallWork2Found = true;
                        break;
                    }
                }
            }
            if (allocCallWork2Found == false) {
                firstSlotIndexesSum.at(groupIndex) = Def::Max_Int;
            }
            groupIndex++;
        }

        int minSlotIndexSum = *std::min_element(firstSlotIndexesSum.begin(),
                                                firstSlotIndexesSum.end());
        unsigned int counterIndex = 0;
        for (auto index: firstSlotIndexesSum) {
            if (index == minSlotIndexSum && index != Def::Max_Int) {
                callWork0->SetRoute(resources->protectionAllRoutesGroups.at(
                        nodePairIndex).front().at(counterIndex).at(0));
                callWork0->SetModulation(FixedModulation);
                this->modulation->SetModulationParam(callWork0.get());
                if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(),
                                                      firstSlotIndexes.at(counterIndex).at(0),
                                                      firstSlotIndexes.at(counterIndex).at(0) +
                                                      callWork0->GetNumberSlots() - 1, occupied)) {
                    callWork0->SetFirstSlot(firstSlotIndexes.at(counterIndex).at(0));
                    callWork0->SetLastSlot(firstSlotIndexes.at(counterIndex).at(0) +
                                           callWork0->GetNumberSlots() - 1);
                    callWork0->SetCore(0);
                }
                callWork1->SetRoute(resources->protectionAllRoutesGroups.at(
                        nodePairIndex).front().at(counterIndex).at(1));
                callWork1->SetModulation(FixedModulation);
                this->modulation->SetModulationParam(callWork1.get());
                if (this->resDevAlloc->CheckSlotsDisp(callWork1->GetRoute(),
                                                      firstSlotIndexes.at(counterIndex).at(1),
                                                      firstSlotIndexes.at(counterIndex).at(1) +
                                                      callWork1->GetNumberSlots() - 1, occupied)) {
                    callWork1->SetFirstSlot(firstSlotIndexes.at(counterIndex).at(1));
                    callWork1->SetLastSlot(firstSlotIndexes.at(counterIndex).at(1) +
                                           callWork1->GetNumberSlots() - 1);
                    callWork1->SetCore(0);
                }
                callWork2->SetRoute(resources->protectionAllRoutesGroups.at(
                        nodePairIndex).front().at(counterIndex).at(2));
                callWork2->SetModulation(FixedModulation);
                this->modulation->SetModulationParam(callWork2.get());
                if (this->resDevAlloc->CheckSlotsDisp(callWork2->GetRoute(),
                                                      firstSlotIndexes.at(counterIndex).at(2),
                                                      firstSlotIndexes.at(counterIndex).at(2) +
                                                      callWork2->GetNumberSlots() - 1, reserved)) {
                    callWork2->SetFirstSlot(firstSlotIndexes.at(counterIndex).at(2));
                    callWork2->SetLastSlot(firstSlotIndexes.at(counterIndex).at(2) +
                                           callWork2->GetNumberSlots() - 1);
                    callWork2->SetCore(0);
                }

                call->SetRoute(resources->protectionAllRoutesGroups.at(
                        nodePairIndex).front().at(counterIndex).at(0));
                call->SetModulation(FixedModulation);
                call->SetFirstSlot(callWork0->GetFirstSlot());
                call->SetLastSlot(callWork0->GetLastSlot());
                call->SetStatus(Accepted);
                call->setCallAllocatedType(reservada);
                //increment proCalls counter
                resDevAlloc->simulType->GetData()->SetProtectedCalls();
                CalcBetaAverage(call);
                CalcAlpha(call);
                callAllocated = true;
                break;
            }
            counterIndex++;
        }
    }
    return callAllocated;
}

void NewPathProtection::RoutingCallWithTwoRoutes(CallDevices *call) {

    this->CreateProtectionCalls(call); //loading transpsegments with calls

    //setting 3 protection calls to allocation
    std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
    std::shared_ptr<Call> callWork0 = callsVec.at(0);
    std::shared_ptr<Call> callWork1 = callsVec.at(1);
    std::shared_ptr<Call> callWork2 = callsVec.at(2);

    call->SetCore(0);
    unsigned int auxSlot;
    unsigned int sumFirstSlots = 0;
    const unsigned int topNumSlots = topology->GetNumSlots();
    std::vector<unsigned int> possibleSlots(0);
    std::vector<int> firstSlotIndexesSum(0);
    std::vector<std::vector<int>> firstSlotIndexes(0);
    possibleSlots = this->resDevAlloc->specAlloc->SpecAllocation();
    unsigned int orN = call->GetOrNode()->GetNodeId();
    unsigned int deN = call->GetDeNode()->GetNodeId();
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int nodePairIndex = orN * numNodes + deN;
    bool callAllocated = false;
    unsigned int groupIndex = 0;

    //Delete one route, recalculate Bit rate and try allocating with 2 routes
    callsVec.pop_back();
    double callBitRate = call->GetBitRate();
    double beta = parameters->GetBeta();
    if(call->isProtected() == true){
        double partialBitRate = ceil(
                (((1 - beta)*2) * callBitRate) / (numSchProtRoutes - 1));
        callWork0->SetBitRate(partialBitRate);
        callWork1->SetBitRate(partialBitRate);
    }else{
        double partialBitRate = ceil(
                (callBitRate) / (numSchProtRoutes - 1));
        callWork0->SetBitRate(partialBitRate);
        callWork1->SetBitRate(partialBitRate);
    }

    call->SetTranspSegments(callsVec);
    groupIndex = 0;

    //trying allocate call with 2 routes
    if (!resources->protectionAllRoutesGroups.at(nodePairIndex).back().empty()) {
        unsigned int numGroups = resources->protectionAllRoutesGroups.at(
                nodePairIndex).back().size();
        firstSlotIndexesSum.clear();
        firstSlotIndexes.clear();
        firstSlotIndexes.resize(numGroups);
        firstSlotIndexesSum.resize(numGroups);
        //computing the first slot indexes available of each group for current call and its sum
        for (auto &group2: resources->protectionAllRoutesGroups.at(
                nodePairIndex).back()) {

            bool allocCallWork0Found = false;
            bool allocCallWork1Found = false;
            sumFirstSlots = 0;
            callWork0->SetRoute(group2.at(0));
            callWork0->SetModulation(FixedModulation);
            this->modulation->SetModulationParam(callWork0.get());
            for (unsigned int s = 0; s < possibleSlots.size(); s++) {
                auxSlot = possibleSlots.at(s);
                if (auxSlot + callWork0->GetNumberSlots() - 1 >= topNumSlots)
                    break;
                if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(),
                                                      auxSlot,auxSlot +callWork0->GetNumberSlots() -1, occupied)) {
                    firstSlotIndexes.at(groupIndex).push_back(auxSlot);
                    sumFirstSlots = auxSlot;
                    allocCallWork0Found = true;
                    break;
                }
            }
            if(allocCallWork0Found == false){
                firstSlotIndexesSum.at(groupIndex) = Def::Max_Int;
            }
            if (allocCallWork0Found == true) {
                callWork1->SetRoute(group2.at(1));
                callWork1->SetModulation(FixedModulation);
                this->modulation->SetModulationParam(callWork1.get());
                for (unsigned int s = 0; s < possibleSlots.size(); s++) {
                    auxSlot = possibleSlots.at(s);
                    if (auxSlot + callWork1->GetNumberSlots() - 1 >= topNumSlots)
                        break;
                    if (this->resDevAlloc->CheckSlotsDisp(callWork1->GetRoute(),
                                                          auxSlot,auxSlot +callWork1->GetNumberSlots() -1, occupied)) {
                        firstSlotIndexes.at(groupIndex).push_back(auxSlot);
                        sumFirstSlots += auxSlot;
                        firstSlotIndexesSum.at(groupIndex) = sumFirstSlots;
                        allocCallWork1Found = true;
                        break;
                    }
                }
            }
            if (allocCallWork1Found == false) {
                firstSlotIndexesSum.at(groupIndex) = Def::Max_Int;
            }
            groupIndex++;
        }

        int minSlotIndexSum = *std::min_element(firstSlotIndexesSum.begin(),
                                                firstSlotIndexesSum.end());
        unsigned int counterIndex = 0;
        for (auto index: firstSlotIndexesSum) {
            if (index == minSlotIndexSum && index != Def::Max_Int) {
                callWork0->SetRoute(resources->protectionAllRoutesGroups.at(
                        nodePairIndex).back().at(counterIndex).at(0));
                callWork0->SetModulation(FixedModulation);
                this->modulation->SetModulationParam(callWork0.get());
                if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(),
                                                      firstSlotIndexes.at(counterIndex).at(0),
                                                      firstSlotIndexes.at(counterIndex).at(0)+callWork0->GetNumberSlots() -1, occupied)) {
                    callWork0->SetFirstSlot(
                            firstSlotIndexes.at(counterIndex).at(0));
                    callWork0->SetLastSlot(
                            firstSlotIndexes.at(counterIndex).at(0) +
                            callWork0->GetNumberSlots() - 1);
                    callWork0->SetCore(0);
                }
                callWork1->SetRoute(resources->protectionAllRoutesGroups.at(
                        nodePairIndex).back().at(counterIndex).at(1));
                callWork1->SetModulation(FixedModulation);
                this->modulation->SetModulationParam(callWork1.get());
                if (this->resDevAlloc->CheckSlotsDisp(callWork1->GetRoute(),
                                                      firstSlotIndexes.at(counterIndex).at(1),
                                                      firstSlotIndexes.at(counterIndex).at(1) +callWork1->GetNumberSlots() -1, occupied)) {
                    callWork1->SetFirstSlot(
                            firstSlotIndexes.at(counterIndex).at(1));
                    callWork1->SetLastSlot(
                            firstSlotIndexes.at(counterIndex).at(1) +
                            callWork1->GetNumberSlots() - 1);
                    callWork1->SetCore(0);
                }
                call->SetRoute(resources->protectionAllRoutesGroups.at(
                        nodePairIndex).back().at(counterIndex).at(0));
                call->SetModulation(FixedModulation);
                call->SetFirstSlot(callWork0->GetFirstSlot());
                call->SetLastSlot(callWork0->GetLastSlot());
                call->SetStatus(Accepted);
                call->setCallAllocatedType(ocupada);
                //increment proCalls counter
                resDevAlloc->simulType->GetData()->SetProtectedCalls();
                CalcBetaAverage(call);
                CalcAlpha(call);
                callAllocated = true;
                break;
            }
            counterIndex++;
        }
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

bool NewPathProtection::RoutingCallUnprotectedWithOneRoute(CallDevices *call) {

    this->CreateUnprotectionCalls(call);
    //setting 3 protection calls to allocation
    std::vector<std::shared_ptr<Call>> callsVec = call->GetTranspSegmentsVec();
    std::shared_ptr<Call> callWork0 = callsVec.at(0);

    call->SetCore(0);
    unsigned int auxSlot;
    unsigned int sumFirstSlots = 0;
    const unsigned int topNumSlots = topology->GetNumSlots();
    std::vector<unsigned int> possibleSlots(0);
    std::vector<int> firstSlotIndexesSum(0);
    std::vector<std::vector<int>> firstSlotIndexes(0);
    possibleSlots = this->resDevAlloc->specAlloc->SpecAllocation();
    unsigned int orN = call->GetOrNode()->GetNodeId();
    unsigned int deN = call->GetDeNode()->GetNodeId();
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int nodePairIndex = orN * numNodes + deN;
    bool callAllocated = false;
    unsigned int groupIndex = 0;
    bool allocCallWork0Found = false;


    if (!resources->protectionAllRoutesGroups.at(nodePairIndex).front().empty()) {
        unsigned int numGroups = resources->protectionAllRoutesGroups.at(
                nodePairIndex).front().size();
        firstSlotIndexes.resize(numGroups);
        firstSlotIndexesSum.resize(numGroups);

        //computing the first slot indexes available of each group for current call and its sum
        for (auto &group3: resources->protectionAllRoutesGroups.at(
                nodePairIndex).front()) {
//                if(groupIndex == parameters->GetNumberRoutes())
//                    break;
            sumFirstSlots = 0;
            callWork0->SetRoute(group3.at(2));
            callWork0->SetModulation(FixedModulation);
            this->modulation->SetModulationParam(callWork0.get());
            for (unsigned int s = 0; s < possibleSlots.size(); s++) {
                auxSlot = possibleSlots.at(s);
                if (auxSlot + callWork0->GetNumberSlots() - 1 >= topNumSlots)
                    break;
                if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(), auxSlot,
                                                      auxSlot + callWork0->GetNumberSlots() - 1, reutilized)) {
                    firstSlotIndexes.at(groupIndex).push_back(auxSlot);
                    sumFirstSlots = auxSlot;
                    allocCallWork0Found = true;
                    break;
                }
            }
            if (allocCallWork0Found == false) {
                firstSlotIndexesSum.at(groupIndex) = Def::Max_Int;
            }
            groupIndex++;
        }


        int minSlotIndexSum = *std::min_element(firstSlotIndexesSum.begin(),
                                                firstSlotIndexesSum.end());
        unsigned int counterIndex = 0;
        for (auto index: firstSlotIndexesSum) {
            if (index == minSlotIndexSum && index != Def::Max_Int) {
                callWork0->SetRoute(resources->protectionAllRoutesGroups.at(
                        nodePairIndex).front().at(counterIndex).at(2));
                callWork0->SetModulation(FixedModulation);
                this->modulation->SetModulationParam(callWork0.get());
                if (this->resDevAlloc->CheckSlotsDisp(callWork0->GetRoute(),
                                                      firstSlotIndexes.at(counterIndex).at(0),
                                                      firstSlotIndexes.at(counterIndex).at(0) +
                                                      callWork0->GetNumberSlots() - 1, reutilized)) {
                    callWork0->SetFirstSlot(firstSlotIndexes.at(counterIndex).at(0));
                    callWork0->SetLastSlot(firstSlotIndexes.at(counterIndex).at(0) +
                                           callWork0->GetNumberSlots() - 1);
                    callWork0->SetCore(0);
                }
                call->SetRoute(resources->protectionAllRoutesGroups.at(
                        nodePairIndex).front().at(counterIndex).at(0));
                call->SetModulation(FixedModulation);
                call->SetFirstSlot(callWork0->GetFirstSlot());
                call->SetLastSlot(callWork0->GetLastSlot());
                call->SetStatus(Accepted);
                call->setCallAllocatedType(reutilizada);
                CalcBetaAverage(call);
                CalcAlpha(call);
                callAllocated = true;
                break;
            }
            counterIndex++;
        }
    }
    return callAllocated;
}

void NewPathProtection::CreateUnprotectionCalls(CallDevices* call){
    call->GetTranspSegments().clear();
    std::vector<double> VecTraffic = resDevAlloc->traffic->GetVecTraffic();
    double callBitRate = call->GetBitRate();
    std::shared_ptr<Call> auxCall;
    std::vector<std::shared_ptr<Call>> auxVec(0);

    auxCall = std::make_shared<Call>(call->GetOrNode(), call->GetDeNode(), callBitRate, call->GetDeactivationTime());
    auxVec.push_back(auxCall);
    call->SetTranspSegments(auxVec);
}
