/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CallGenerator.cpp
 * Author: bruno
 * 
 * Created on August 17, 2018, 10:53 PM
 */

#include "../../include/Calls/EventGenerator.h"
#include "../../include/Calls/Traffic.h"
#include "../../include/Calls/Call.h"
#include "../../include/Calls/CallDevices.h"
#include "../../include/Calls/Event.h"
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/Data.h"
#include "../../include/Data/Options.h"

std::default_random_engine EventGenerator::random_generator(0);

bool EventGenerator::EventCompare::operator()(
        const std::shared_ptr<Event> eventA,
        const std::shared_ptr<Event> eventB) const {

    return (eventA->GetEventTime() > eventB->GetEventTime());
}

EventGenerator::EventGenerator(SimulationType* simulType)
        :simulType(simulType), topology(nullptr), data(nullptr), traffic(nullptr),
         networkLoad(0.0), simulationTime(0.0) {

}

EventGenerator::~EventGenerator() {

}

void EventGenerator::Load() {
    this->topology = this->GetSimulType()->GetTopology();
    this->data = this->GetSimulType()->GetData();
    this->traffic = this->GetSimulType()->GetTraffic();
    this->resourceAlloc = this->GetSimulType()->GetResourceAlloc();

    this->uniformNodeDistribution = std::uniform_int_distribution<int>
            (0, this->topology->GetNumNodes() - 1);
    this->uniformTrafficDistribution = std::uniform_int_distribution<int>
            (0, this->traffic->GetVecTraffic().size() - 1);
    this->exponencialMuDistribution = std::exponential_distribution<TIME>
            (1.0L / this->simulType->GetParameters()->GetMu());

    this->LoadRandomGenerator();
}

void EventGenerator::Initialize() {
    this->InitializeGenerator();
    this->simulationTime = 0.0;
    this->exponencialHDistribution = std::exponential_distribution<TIME>
            (this->networkLoad);
    this->SetRealSimulationTime((TIME) std::clock() / CLOCKS_PER_SEC);
}

void EventGenerator::Finalize() {
    this->SetRealSimulationTime(((TIME) std::clock() / CLOCKS_PER_SEC) -
                                this->GetRealSimulationTime());
    this->data->SetRealSimulTime(this->GetRealSimulationTime());

    while(!this->queueEvents.empty()){
        this->queueEvents.pop();
    }
}

void EventGenerator::GenerateCall() {
    std::shared_ptr<Call> newCall;
    unsigned int auxIndexOrNode = uniformNodeDistribution(random_generator);
    unsigned int auxIndexDeNode;

    do{
        auxIndexDeNode = uniformNodeDistribution(random_generator);
    }while(auxIndexOrNode == auxIndexDeNode);

    unsigned int auxIndexTraffic =
            uniformTrafficDistribution(random_generator);

    TIME arrivalTime = exponencialHDistribution(random_generator);
    TIME deactvationTime = exponencialMuDistribution(random_generator);

    // generates number in the range 0..1
    // if 0 call is non-protected, if 1 call is protected
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,1);
    int type = distribution(generator);
    bool typeCall;
    if(type == 0){
        typeCall = false;
    }else{
        typeCall = true;
    };

    newCall = this->CreateCall(auxIndexOrNode, auxIndexDeNode, auxIndexTraffic,
                               deactvationTime, typeCall);

    //Event creation from the call created before
    std::shared_ptr<Event> newEvent =
            std::make_shared<Event>(this, newCall, this->GetSimulationTime() +
                                                   arrivalTime);

    this->PushEvent(newEvent);
}

double EventGenerator::GetNetworkLoad() const {
    return networkLoad;
}

void EventGenerator::SetNetworkLoad(const double networkLoad) {
    assert(networkLoad >= 0.0);
    this->networkLoad = networkLoad;
}

TIME EventGenerator::GetSimulationTime() const {
    return simulationTime;
}

void EventGenerator::SetSimulationTime(const TIME simulationTime) {
    assert(this->simulationTime <= simulationTime);
    this->simulationTime = simulationTime;
}

std::shared_ptr<Event> EventGenerator::GetNextEvent() {
    std::shared_ptr<Event> nextEvent = this->queueEvents.top();
    this->queueEvents.pop();
    this->SetSimulationTime(nextEvent->GetEventTime());

    return nextEvent;
}

void EventGenerator::PushEvent(std::shared_ptr<Event> evt) {
    this->queueEvents.push(evt);
}

SimulationType* EventGenerator::GetSimulType() const {
    return simulType;
}

Topology* EventGenerator::GetTopology() const {
    return topology;
}

Data* EventGenerator::GetData() const {
    return data;
}

ResourceAlloc* EventGenerator::GetResourceAlloc() const {
    return resourceAlloc;
}

TIME EventGenerator::GetRealSimulationTime() const {
    return realSimulationTime;
}

void EventGenerator::SetRealSimulationTime(TIME realSimullationTime) {
    assert(realSimullationTime > 0.0);
    this->realSimulationTime = realSimullationTime;
}

std::shared_ptr<Call> EventGenerator::CreateCall(unsigned orNodeIndex,
                                                 unsigned deNodeIndex, unsigned trafficIndex, TIME deactTime, bool protectionCall) {
    std::shared_ptr<Call> newCall;
    Node* orNode = this->topology->GetNode(orNodeIndex);
    Node* deNode = this->topology->GetNode(deNodeIndex);
    double traffic = this->traffic->GetTraffic(trafficIndex);

    switch(this->simulType->GetOptions()->GetDevicesOption()){
        case DevicesDisabled:
            newCall = std::make_shared<Call>(orNode, deNode, traffic,
                                             deactTime, protectionCall);
            break;
        case DevicesEnabled:
            newCall = std::make_shared<CallDevices>(orNode, deNode, traffic,
                                                    deactTime, protectionCall);
            break;
        default:
            std::cerr << "Invalid device option" << std::endl;
            std::abort();
    }

    return newCall;
}

void EventGenerator::LoadRandomGenerator() {

    switch(simulType->GetOptions()->GetGenerationOption()){
        case GenerationSame:
        case GenerationPseudoRandom:
            EventGenerator::random_generator = std::default_random_engine{0};
            break;
        case GenerationRandom:
            EventGenerator::random_generator =
                    std::default_random_engine{Def::randomDevice()};
            break;
        default:
            std::cerr << "Invalid random generation option" << std::endl;
            std::abort();
    }
}

void EventGenerator::InitializeGenerator() {

    if(simulType->GetOptions()->GetGenerationOption() == GenerationSame)
        EventGenerator::random_generator = std::default_random_engine{0};
}