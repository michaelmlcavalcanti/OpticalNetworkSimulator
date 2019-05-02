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

IncNumRegSimulation::IncNumRegSimulation(unsigned int simulIndex, 
TypeSimulation typeSimulation)
:SimulationType(simulIndex, typeSimulation), vecNumReg(0) {

}

IncNumRegSimulation::~IncNumRegSimulation() {

}

void IncNumRegSimulation::Run() {
    unsigned int numNodes = this->GetTopology()->GetNumNodes();
    unsigned int minNumReg = 5 * numNodes;
    unsigned int maxNumReg = 50 * numNodes;
    unsigned int stepNumReg = 5 * numNodes;
    unsigned int numPoints = (maxNumReg - minNumReg) / stepNumReg;
    unsigned int actualPoint = 0;
    this->GetCallGenerator()->SetNetworkLoad(this->
    GetParameters()->GetMidLoadPoint());
    this->GetData()->Initialize(numPoints);
    
    for(unsigned a = minNumReg; a <= maxNumReg; a += stepNumReg){
        this->GetData()->SetActualIndex(actualPoint);
        this->GetInputOutput()->PrintProgressBar(actualPoint, numPoints);
        
        vecNumReg.push_back(a);
        this->GetTopology()->SetNumRegenerators(a);
        
        this->RunBase();
        std::cout << this->GetData() << std::endl;
        actualPoint++;
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
    this->GetData()->SaveMainResults(vecNumReg);
}

void IncNumRegSimulation::Help() {
    std::cout << "INCREASE NUMBER OF TRANSPONDERS SIMULATION" << std::endl
              << "This type of simulation increase the total number of "
              << "regenerators in the network and analyzes the network "
              << "performance. " << std::endl << std::endl;
}
