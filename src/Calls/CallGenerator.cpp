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

#include "../../include/Calls/CallGenerator.h"
#include "../../include/Calls/Traffic.h"
#include "../../include/Calls/Call.h"
#include "../../include/Calls/Event.h"
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/Data.h"

std::default_random_engine CallGenerator::random_generator(0);

bool CallGenerator::EventCompare::operator()(
const std::shared_ptr<Event> eventA,
const std::shared_ptr<Event> eventB) const {
    
    return (eventA->GetEventTime() > eventB->GetEventTime());
}

CallGenerator::CallGenerator(SimulationType* simulType) 
:simulType(simulType), topology(nullptr), data(nullptr), traffic(nullptr),
networkLoad(0.0), simulationTime(0.0) {
    
}

CallGenerator::~CallGenerator() {
    
}

void CallGenerator::Load() {
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
}

void CallGenerator::Initialize() {
    this->simulationTime = 0.0;
    this->exponencialHDistribution = std::exponential_distribution<TIME>
    (this->networkLoad);
    this->SetRealSimulationTime((TIME) std::clock() / CLOCKS_PER_SEC);
}

void CallGenerator::Finalize() {
    this->SetRealSimulationTime(((TIME) std::clock() / CLOCKS_PER_SEC) -
    this->GetRealSimulationTime());
    this->data->SetRealSimulTime(this->GetRealSimulationTime());
    
    while(!this->queueEvents.empty()){
        this->queueEvents.pop();
    }
}

void CallGenerator::GenerateCall() {
    this->simulType->numberRequests++;
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
    
    newCall = std::make_shared<Call>(this->topology->GetNode(auxIndexOrNode),
    this->topology->GetNode(auxIndexDeNode), this->traffic->
    GetTraffic(auxIndexTraffic), deactvationTime);
    
    //Event creation from the call created before
    std::shared_ptr<Event> newEvent = 
    std::make_shared<Event>(this, newCall, this->GetSimulationTime() + 
                            arrivalTime);
    
    this->PushEvent(newEvent);
}

double CallGenerator::GetNetworkLoad() const {
    return networkLoad;
}

void CallGenerator::SetNetworkLoad(const double networkLoad) {
    assert(networkLoad >= 0.0);
    this->networkLoad = networkLoad;
}

TIME CallGenerator::GetSimulationTime() const {
    return simulationTime;
}

void CallGenerator::SetSimulationTime(const TIME simulationTime) {
    assert(this->simulationTime <= simulationTime);
    this->simulationTime = simulationTime;
}

std::shared_ptr<Event> CallGenerator::GetNextEvent() {
    std::shared_ptr<Event> nextEvent = this->queueEvents.top();
    this->queueEvents.pop();
    this->SetSimulationTime(nextEvent->GetEventTime());
    
    return nextEvent;
}

void CallGenerator::PushEvent(std::shared_ptr<Event> evt) {
    this->queueEvents.push(evt);
}

SimulationType* CallGenerator::GetSimulType() const {
    return simulType;
}

void CallGenerator::SetSimulType(SimulationType* const simulType) {
    assert(simulType != nullptr);
    this->simulType = simulType;
}

Topology* CallGenerator::GetTopology() const {
    return topology;
}

void CallGenerator::SetTopology(Topology* topology) {
    this->topology = topology;
}

Data* CallGenerator::GetData() const {
    return data;
}

void CallGenerator::SetData(Data* data) {
    this->data = data;
}

ResourceAlloc* CallGenerator::GetResourceAlloc() const {
    return resourceAlloc;
}

void CallGenerator::SetResourceAlloc(ResourceAlloc* rsaAlgorithm) {
    this->resourceAlloc = rsaAlgorithm;
}

TIME CallGenerator::GetRealSimulationTime() const {
    return realSimulationTime;
}

void CallGenerator::SetRealSimulationTime(TIME realSimullationTime) {
    assert(realSimullationTime > 0.0);
    this->realSimulationTime = realSimullationTime;
}
