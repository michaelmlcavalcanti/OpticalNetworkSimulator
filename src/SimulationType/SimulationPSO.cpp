/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationPSO.cpp
 * Author: brunovacorreia
 * 
 * Created on September 16, 2019, 9:33 PM
 */

#include "../../include/SimulationType/SimulationPSO.h"
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Algorithms/PSO/PSO.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Calls/EventGenerator.h"
#include "../../include/Data/Data.h"

SimulationPSO::SimulationPSO(unsigned int simulIndex, 
TypeSimulation typeSimulation)
:SimulationType(simulIndex, typeSimulation) {

}

SimulationPSO::~SimulationPSO() {

}

void SimulationPSO::Run() {
    this->GetCallGenerator()->SetNetworkLoad(psoAlgorithm->GetLoadPoint());
    unsigned int numIterations = psoAlgorithm->GetNumberIterations();
    
    psoAlgorithm->InitializePopulation();
    this->GetInputOutput()->PrintProgressBar(0, numIterations);
    
    for(unsigned int a = 1; a <= numIterations; a++){
        psoAlgorithm->SetActualIteration(a);
        psoAlgorithm->RunIteration();
        psoAlgorithm->SaveBestParticle();
        std::cout << psoAlgorithm << std::endl;
        this->GetInputOutput()->PrintProgressBar(a, numIterations);
    }
}

void SimulationPSO::RunBase() {
    SimulationType::Run();
}

void SimulationPSO::Load() {
    SimulationType::Load();
    this->CreatePSO();
    psoAlgorithm->LoadFile();
}

void SimulationPSO::LoadFile() {
    SimulationType::LoadFile();
    this->CreatePSO();
    psoAlgorithm->LoadFile();
}

void SimulationPSO::AdditionalSettings() {
    SimulationType::AdditionalSettings();
    psoAlgorithm->Initialize();
}

void SimulationPSO::Print() {
    this->Help(std::cout);
    SimulationType::Print();
    psoAlgorithm->PrintParameters(std::cout);
}

void SimulationPSO::Save() {
    SimulationType::Save();
    this->GetData()->SaveFilesPSO();
}

std::ostream& SimulationPSO::Help(std::ostream& ostream) {
    ostream << "PSO SIMULATION" << std::endl
            << "This type of simulation runs the particle swarm algorithm"
            << std::endl << std::endl;
    
    return ostream;
}

PSO* SimulationPSO::GetPSO() {
    return psoAlgorithm.get();
}

void SimulationPSO::CreatePSO() {
    
    psoAlgorithm = std::make_shared<PSO>(this);
}
