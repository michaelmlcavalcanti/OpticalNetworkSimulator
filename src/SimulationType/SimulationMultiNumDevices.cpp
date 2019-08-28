/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationMultiNumDevices.cpp
 * Author: brunovacorreia
 * 
 * Created on July 31, 2019, 2:37 PM
 */

#include "../../include/SimulationType/SimulationMultiNumDevices.h"
#include "../../include/Calls/EventGenerator.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/Data.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Structure/Devices/Device.h"

SimulationMultiNumDevices::SimulationMultiNumDevices(unsigned int simulIndex, 
TypeSimulation typeSimulation)
:SimulationType(simulIndex, typeSimulation), vecNumDevices(0) {

}

SimulationMultiNumDevices::~SimulationMultiNumDevices() {

}

void SimulationMultiNumDevices::Run() {
    unsigned int numNodes = this->GetTopology()->GetNumNodes();
    unsigned int minNumReg = 0 * numNodes;
    unsigned int maxNumReg = 1500 * numNodes;
    unsigned int stepNumReg = 25 * numNodes;
    unsigned int numPoints = ((maxNumReg - minNumReg) / stepNumReg) + 1;
    unsigned int actualPoint = 0;
    
    this->GetCallGenerator()->SetNetworkLoad(this->GetParameters()
                                                 ->GetMaxLoadPoint());
    this->GetData()->Initialize(numPoints);
    this->GetInputOutput()->PrintProgressBar(actualPoint, numPoints);
    
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

void SimulationMultiNumDevices::RunBase() {
    SimulationType::Run();
}

void SimulationMultiNumDevices::Load() {
    SimulationType::Load();
}

void SimulationMultiNumDevices::LoadFile() {
    SimulationType::LoadFile();
}

void SimulationMultiNumDevices::Print() {
    this->Help(std::cout);
    SimulationType::Print();
}

void SimulationMultiNumDevices::Save() {
    SimulationType::Save();
    this->GetData()->SaveLog(vecNumDevices);
    this->GetData()->SaveBP(vecNumDevices);
}

std::ostream& SimulationMultiNumDevices::Help(std::ostream& ostream) {
    ostream << "INCREASE NUMBER OF DEVICES SIMULATION" << std::endl
            << "This type of simulation increase the total number of "
            << "regenerators in the network and analyzes its performance. " 
            << std::endl << std::endl;
    
    return ostream;
}

void SimulationMultiNumDevices::SetNumberOfDevices(unsigned int numDevices) {
    //Switch for variate the type of device.
    DeviceType type = DeviceRegenerator;
    vecNumDevices.push_back(numDevices);
    
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