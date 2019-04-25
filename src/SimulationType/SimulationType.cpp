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
#include "../../include/Calls/CallGenerator.h"
#include "../../include/ResourceAllocation/ResourceAlloc.h"

SimulationType::SimulationType(unsigned int simulIndex)
:simulationIndex(simulIndex),
parameters(std::make_shared<Parameters> (this)),
options(std::make_shared<Options> (this)), 
data(boost::make_unique<Data>(this)),
topology(std::make_shared<Topology> (this)),
inputOutput(boost::make_unique<InputOutput>(this)),
traffic(std::make_shared<Traffic>(this)),
callGenerator(std::make_shared<CallGenerator>(this)),
resourceAlloc(std::make_shared<ResourceAlloc>(this)) {
    
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
    this->callGenerator->Load();
    this->resourceAlloc->Load();
}

void SimulationType::LoadFile() {
    this->parameters->LoadFile();
    this->options->LoadFile();
    this->topology->LoadFile();
    this->traffic->LoadFile();
    this->GetData()->Initialize();
    this->callGenerator->Load();
    this->resourceAlloc->Load();
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
    
    if(this->resourceAlloc->IsOfflineRouting()){
        this->resourceAlloc->RoutingOffline();
        this->resourceAlloc->UpdateRoutesCosts();
        
        if(this->resourceAlloc->CheckInterRouting()){
            this->resourceAlloc->SetInterferingRoutes2();
            this->resourceAlloc->SetNumInterRoutesToCheck();
            this->resourceAlloc->SetNumSlotsTraffic();
        }
    }
}

const unsigned int SimulationType::GetSimulationIndex() const {
    return simulationIndex;
}

Parameters* SimulationType::GetParameters() const {
    return parameters.get();
}

void SimulationType::SetParameters(std::shared_ptr<Parameters> 
parameters) {
    this->parameters = parameters;
}

Options* SimulationType::GetOptions() const {
    return options.get();
}

void SimulationType::SetOptions(std::shared_ptr<Options> options) {
    this->options = options;
}

Data* SimulationType::GetData() const {
    return data.get();
}

void SimulationType::SetData(std::unique_ptr<Data> data) {
    std::swap(this->data, data);
}

Topology* SimulationType::GetTopology() const {
    return topology.get();
}

void SimulationType::SetTopology(std::shared_ptr<Topology> 
topology) {
    this->topology = topology;
}

InputOutput* SimulationType::GetInputOutput() const {
    return inputOutput.get();
}

void SimulationType::SetInputOutput(std::unique_ptr<InputOutput> 
inputOutput) {
    this->inputOutput = std::move(inputOutput);
}

Traffic* SimulationType::GetTraffic() const {
    return traffic.get();
}

void SimulationType::SetTraffic(std::shared_ptr<Traffic> 
traffic) {
    this->traffic = traffic;
}

CallGenerator* SimulationType::GetCallGenerator() const {
    return callGenerator.get();
}

void SimulationType::SetCallGenerator(std::shared_ptr<CallGenerator> 
callGenerator) {
    this->callGenerator = callGenerator;
}

ResourceAlloc* SimulationType::GetResourceAlloc() const {
    return this->resourceAlloc.get();
}

void SimulationType::SetResourceAlloc(std::shared_ptr<ResourceAlloc> rsaAlgorithm) {
    this->resourceAlloc = rsaAlgorithm;
}

void SimulationType::InitializeAll() {
    this->topology->Initialize();
    this->callGenerator->Initialize();
    this->numberRequests = 0.0;
}

void SimulationType::Simulate() {
    double numReqMax = this->parameters->GetNumberReqMax();
    this->callGenerator->GenerateCall();
    
    while(this->numberRequests <= numReqMax){
        std::shared_ptr<Event> evt = this->callGenerator->GetNextEvent();
        
        switch(evt->GetEventType()){
            case CallRequest:
                evt->ImplementCallRequest();
                this->callGenerator->GenerateCall();
                break;
            case CallEnd:
                evt->ImplementCallEnd();
                break;
            default:
                std::cerr << "Invalid event" << std::endl;
        }
    }
    
    this->GetData()->SetNumberReq(this->numberRequests-1);
    this->GetData()->SetSimulTime(this->callGenerator->GetSimulationTime());
}

void SimulationType::FinalizeAll() {
    this->callGenerator->Finalize();
}
