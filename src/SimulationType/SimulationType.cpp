/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationType.cpp
 * Author: bruno
 * 
 * Created on August 2, 2018, 3:50 PM
 */

#include <boost/make_unique.hpp>

#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/Options.h"
#include "../../include/Data/Data.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Calls/Traffic.h"
#include "../../include/Calls/EventGenerator.h"
#include "../../include/Calls/Event.h"
#include "../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../include/ResourceAllocation/ResourceDeviceAlloc.h"

SimulationType::SimulationType(SimulIndex simulIndex, 
TypeSimulation typeSimulation)
:typeSimulation(typeSimulation), simulationIndex(simulIndex),
parameters(std::make_shared<Parameters> (this)),
options(std::make_shared<Options> (this)), 
data(boost::make_unique<Data>(this)),
topology(std::make_shared<Topology>(this)),
inputOutput(boost::make_unique<InputOutput>(this)),
traffic(std::make_shared<Traffic>(this)),
callGenerator(std::make_shared<EventGenerator>(this)),
resourceAlloc(nullptr) {
    
}

SimulationType::~SimulationType() {
    this->parameters.reset();
    this->options.reset();
    this->data.release();
    this->topology.reset();
    this->inputOutput.release();
    this->traffic.reset();
    this->callGenerator.reset();
    this->resourceAlloc.reset();
}

void SimulationType::Run() {
    this->InitializeAll();
    this->Simulate();
    this->FinalizeAll();
}

void SimulationType::Load() {
    this->parameters->Load();
    this->options->Load();
    this->topology->LoadFile();
    this->traffic->LoadFile();
    this->GetData()->Initialize();
    this->CreateLoadResourceAlloc();
    this->callGenerator->Load();
}

void SimulationType::LoadFile() {
    this->parameters->LoadFile();
    this->options->LoadFile();
    this->topology->LoadFile();
    this->traffic->LoadFile();
    this->GetData()->Initialize();
    this->CreateLoadResourceAlloc();
    this->callGenerator->Load();
}

void SimulationType::Print() {
    std::cout << this->options << std::endl;
    std::cout << this->parameters << std::endl;
}

void SimulationType::Save() {
    this->options->Save();
    this->parameters->Save();
}

void SimulationType::AdditionalSettings() {
    this->topology->SetAditionalSettings();
    this->resourceAlloc->AdditionalSettings();
}

TypeSimulation SimulationType::GetTypeSimulation() const {
    return typeSimulation;
}

const SimulIndex SimulationType::GetSimulationIndex() const {
    return simulationIndex;
}

Parameters* SimulationType::GetParameters() const {
    return parameters.get();
}
Options* SimulationType::GetOptions() const {
    return options.get();
}

Data* SimulationType::GetData() const {
    return data.get();
}
Topology* SimulationType::GetTopology() const {
    return topology.get();
}

InputOutput* SimulationType::GetInputOutput() const {
    return inputOutput.get();
}
Traffic* SimulationType::GetTraffic() const {
    return traffic.get();
}

EventGenerator* SimulationType::GetCallGenerator() const {
    return callGenerator.get();
}
ResourceAlloc* SimulationType::GetResourceAlloc() const {
    return this->resourceAlloc.get();
}

void SimulationType::InitializeAll() {
    this->topology->Initialize();
    this->callGenerator->Initialize();
    this->numberRequests = 0.0;
}

void SimulationType::Simulate() {
    this->callGenerator->GenerateCall();
    
    switch(options->GetStopCriteria()){
        case NumCallRequestsMaximum:
            this->SimulateNumTotalReq();
            break;
        case NumCallRequestsBlocked:
            this->SimulateNumBlocReq();
            break;
        default:
            std::cerr << "Invalid stop criteria" << std::endl;
            std::abort();
    }
    
    this->GetData()->SetNumberReq(this->numberRequests);
    this->GetData()->SetSimulTime(this->callGenerator->GetSimulationTime());
}

void SimulationType::FinalizeAll() {
    this->callGenerator->Finalize();
}

void SimulationType::CreateLoadResourceAlloc() {
    if(this->options->GetDevicesOption() == DevicesDisabled)
        this->resourceAlloc = std::make_shared<ResourceAlloc>(this);
    else
        this->resourceAlloc = std::make_shared<ResourceDeviceAlloc>(this);
    
    this->resourceAlloc->Load();
}

void SimulationType::SimulateNumTotalReq() {
    double numReqMax = this->parameters->GetNumberReqMax();
    std::shared_ptr<Event> evt;
    
    while(this->numberRequests < numReqMax){
        evt = this->callGenerator->GetNextEvent();
        
        evt->ImplementEvent();
    }
}

void SimulationType::SimulateNumBlocReq() {
    double numBlocReqMax = this->parameters->GetNumberBloqMax();
    std::shared_ptr<Event> evt;
    
    while(this->GetData()->GetNumberBlocReq() < numBlocReqMax){
        evt = this->callGenerator->GetNextEvent();
        
        evt->ImplementEvent();
    }
}
