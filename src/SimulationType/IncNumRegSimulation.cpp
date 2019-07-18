/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IncNumRegSimulation.cpp
 * Author: BrunoVinicius
 * 
 * Created on April 30, 2019, 4:47 PM
 */

#include "../../include/SimulationType/IncNumRegSimulation.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Calls/CallGenerator.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/Data.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Structure/Devices/Device.h"

IncNumRegSimulation::IncNumRegSimulation(unsigned int simulIndex, 
TypeSimulation typeSimulation)
:SimulationType(simulIndex, typeSimulation), vecNumReg(0) {

}

IncNumRegSimulation::~IncNumRegSimulation() {

}

void IncNumRegSimulation::Run() {
    unsigned int numNodes = this->GetTopology()->GetNumNodes();
    unsigned int minNumReg = 0 * numNodes;
    unsigned int maxNumReg = 1000 * numNodes;
    unsigned int stepNumReg = 10 * numNodes;
    unsigned int numPoints = ((maxNumReg - minNumReg) / stepNumReg) + 1;
    unsigned int actualPoint = 0;
    this->GetCallGenerator()->SetNetworkLoad(this->
    GetParameters()->GetMaxLoadPoint());
    this->GetData()->Initialize(numPoints);
    
    this->GetInputOutput()->PrintProgressBar(0, numPoints);
    for(unsigned a = minNumReg; a <= maxNumReg; a += stepNumReg){
        this->GetData()->SetActualIndex(actualPoint);
        
        this->SetNumberOfDevices(a);
        
        this->RunBase();
        std::cout << "Number of regenerators: " << a << std::endl;
        std::cout << this->GetData() << std::endl;
        actualPoint++;
        this->GetInputOutput()->PrintProgressBar(actualPoint, numPoints);
    }
}

void IncNumRegSimulation::RunBase() {
    SimulationType::Run();
}

void IncNumRegSimulation::Load() {
    SimulationType::Load();
}

void IncNumRegSimulation::LoadFile() {
    SimulationType::LoadFile();
}

void IncNumRegSimulation::Print() {
    this->Help();
    SimulationType::Print();
}

void IncNumRegSimulation::Save() {
    SimulationType::Save();
    this->GetData()->SaveCallReqBP(vecNumReg);
}

void IncNumRegSimulation::Help() {
    std::cout << "INCREASE NUMBER OF REGENERATORS SIMULATION" << std::endl
              << "This type of simulation increase the total number of "
              << "regenerators in the network and analyzes its performance. " 
              << std::endl << std::endl;
}

void IncNumRegSimulation::SetNumberOfDevices(unsigned int numDevices) {
    //Switch for variate the type of device.
    DeviceType type = DeviceRegenerator;
    vecNumReg.push_back(numDevices);
    
    switch(type){
        case DeviceRegenerator:
            this->GetTopology()->SetNumDevices(numDevices, DeviceRegenerator);
            break;
        case DeviceTransponder:
            this->GetTopology()->SetNumDevices(numDevices, DeviceTransponder);
            break;
        default:
            std::cerr << "Invalid device type" << std::endl;
            std::abort();
    }
}
