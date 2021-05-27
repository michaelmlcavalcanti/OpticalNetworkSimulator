/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationMultiLoad.cpp
 * Author: brunovacorreia
 * 
 * Created on July 31, 2019, 2:13 PM
 */

#include "../../include/SimulationType/SimulationMultiLoad.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/Data.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Calls/EventGenerator.h"
#include "../../include/Calls/Traffic.h"
#include "../../include/Structure/Topology.h"

SimulationMultiLoad::SimulationMultiLoad(unsigned int simulIndex, 
TypeSimulation typeSimulation)
:SimulationType(simulIndex, typeSimulation) {

}

SimulationMultiLoad::~SimulationMultiLoad() {

}

void SimulationMultiLoad::Run() {
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

void SimulationMultiLoad::RunBase() {
    SimulationType::Run();
}

void SimulationMultiLoad::Load() {
    SimulationType::Load();
}

void SimulationMultiLoad::LoadFile() {
    SimulationType::LoadFile();
}

void SimulationMultiLoad::Print() {
    this->Help(std::cout);
    SimulationType::Print();
}

void SimulationMultiLoad::Save() {
    SimulationType::Save();
    this->GetData()->SaveLog();
    this->GetData()->SaveBP();
    this->GetData()->SaveNetUtiliz();
    this->GetData()->SaveNetFrag();
    this->GetData()->SaveAccumNetFrag();
    this->GetData()->SaveFragTraffic();
    this->GetData()->SaveLinksUse();
    this->GetData()->SaveSlotsRelativeUse();
    this->GetData()->SaveNetProtRate();
    this->GetData()->SaveNetNonProtRate();
    this->GetData()->SaveNetBetaAverage();
    this->GetData()->SaveNetAlphaAverage();
    this->GetData()->SaveNumHopsRoutes();
}

std::ostream& SimulationMultiLoad::Help(std::ostream& ostream) {
    ostream << "MULTIPLE LOADS SIMULATION" << std::endl
            << "This type of simulation varies the network load "
            << "and analyzes the network performance. " 
            << std::endl << std::endl;
    
    return ostream;
}
