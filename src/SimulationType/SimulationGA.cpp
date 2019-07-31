/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationGA.cpp
 * Author: brunovacorreia
 * 
 * Created on July 31, 2019, 1:32 PM
 */

#include "../../include/SimulationType/SimulationGA.h"
#include "../../include/Algorithms/GA/GA_RsaOrder.h"
#include "../../include/Algorithms/GA/GA_NumInterRoutesMSCL.h"
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Calls/EventGenerator.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/Data.h"
#include "../../include/Data/Options.h"

SimulationGA::SimulationGA(unsigned int simulIndex, 
TypeSimulation typeSimulation)
:SimulationType(simulIndex, typeSimulation) {

}

SimulationGA::~SimulationGA() {

}

void SimulationGA::Run() {
    this->GetCallGenerator()->SetNetworkLoad(this->gaAlgorithm->GetLoadPoint());
    unsigned int numGenerations = this->gaAlgorithm->GetNumberGenerations();
    
    this->gaAlgorithm->InitializePopulation();
    this->GetInputOutput()->PrintProgressBar(0, numGenerations);
    this->gaAlgorithm->RunSelectPop();
    this->gaAlgorithm->KeepInitialPopulation();
    
    for(unsigned int a = 1; a <= numGenerations; a++){
        this->gaAlgorithm->SetActualGeneration(a);
        this->gaAlgorithm->CreateNewPopulation();
        this->gaAlgorithm->RunTotalPop();
        
        if(a == numGenerations)
            this->gaAlgorithm->CheckMinSimul();
        this->gaAlgorithm->SelectPopulation();
        this->gaAlgorithm->SaveIndividuals();
        std::cout << this->gaAlgorithm << std::endl;
        this->GetInputOutput()->PrintProgressBar(a, numGenerations);
    }
}

void SimulationGA::RunBase() {
    SimulationType::Run();
}

void SimulationGA::Load() {
    SimulationType::Load();
    this->CreateGA();
    this->gaAlgorithm->LoadFile();
}

void SimulationGA::LoadFile() {
    SimulationType::LoadFile();
    this->CreateGA();
    this->gaAlgorithm->LoadFile();
}

void SimulationGA::AdditionalSettings() {
    SimulationType::AdditionalSettings();
    this->gaAlgorithm->Initialize();
}

void SimulationGA::Print() {
    this->Help();
    SimulationType::Print();
    this->gaAlgorithm->printParameters(std::cout);
}

void SimulationGA::Save() {
    SimulationType::Save();
    this->GetData()->SaveGaFiles();
}

void SimulationGA::Help() {
    std::cout << "GA SIMULATION" << std::endl
              << "This type of simulation applies the genetic algorithm." 
              << std::endl;
    std::cout << "The specific GA is determined by the option chosen."
              << std::endl << std::endl;
}

GA* SimulationGA::GetGA() const {
    return this->gaAlgorithm.get();
}

void SimulationGA::CreateGA() {
    assert(this->GetOptions()->GetGaOption() != GaOptionDisabled);
    
    switch(this->GetOptions()->GetGaOption()){
        case GaRsaOrder:
            this->gaAlgorithm = std::make_shared<GA_RsaOrder>(this);
            break;
        case GaNumRoutesCheckMSCL:
            this->gaAlgorithm = std::make_shared<GA_NumInterRoutesMSCL>(this);
            break;
        default:
            std::cerr << "Invalid GA option" << std::endl;
            std::abort();
    }
}