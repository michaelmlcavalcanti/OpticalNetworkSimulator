/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MultiLoadSimulation.cpp
 * Author: bruno
 * 
 * Created on August 8, 2018, 7:18 PM
 */

#include "../../include/SimulationType/MultiLoadSimulation.h"

#include "../../include/Data/Parameters.h"
#include "../../include/Calls/CallGenerator.h"
#include "../../include/Calls/Traffic.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Data/Data.h"
#include "../../include/Data/InputOutput.h"

MultiLoadSimulation::MultiLoadSimulation(unsigned int simulIndex, 
TypeSimulation typeSimulation)
:SimulationType(simulIndex, typeSimulation){
    
}

MultiLoadSimulation::~MultiLoadSimulation() {
    
}

void MultiLoadSimulation::Run() {
    unsigned int numLoadPoints = this->GetParameters()->
    GetNumberLoadPoints();
    
    for(unsigned int a = 0; a < numLoadPoints; ++a){
        double loadPoint = this->GetParameters()->GetLoadPoint(a);
        this->GetData()->SetActualIndex(a);
        this->GetCallGenerator()->SetNetworkLoad(loadPoint);
        this->GetInputOutput()->PrintProgressBar(a, numLoadPoints);
        
        this->RunBase();
        
        std::cout << this->GetData() << std::endl;
    }
    
    this->GetInputOutput()->PrintProgressBar(numLoadPoints, numLoadPoints);
}

void MultiLoadSimulation::RunBase() {
    SimulationType::Run();
}

void MultiLoadSimulation::Load() {
    SimulationType::Load();
}

void MultiLoadSimulation::LoadFile() {
    SimulationType::LoadFile();
}

void MultiLoadSimulation::Print() {
    this->Help();
    SimulationType::Print();
}

void MultiLoadSimulation::Save() {
    SimulationType::Save();
    this->GetData()->SaveMultiloadLog();
    this->GetData()->SavePBvLoad();
}

void MultiLoadSimulation::Help() {
    std::cout << "MULTIPLE LOADS SIMULATION" << std::endl
              << "This type of simulation varies the network load "
              << "and analyzes the network performance. "
              << std::endl << std::endl;
}
