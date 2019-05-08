/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_SingleObjective.cpp
 * Author: BrunoVinicius
 * 
 * Created on February 11, 2019, 10:53 PM
 */

#include "../../include/SimulationType/GA_SingleObjective.h"
#include "../../include/Algorithms/GA/GA_RsaOrder.h"
#include "../../include/Algorithms/GA/GA_NumInterRoutesMSCL.h"
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Calls/CallGenerator.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/Data.h"
#include "../../include/Data/Options.h"

GA_SingleObjective::GA_SingleObjective(unsigned int simulIndex, 
TypeSimulation typeSimulation) 
:SimulationType(simulIndex, typeSimulation) {
    
}

GA_SingleObjective::~GA_SingleObjective() {
    
}

void GA_SingleObjective::Run() {
    this->SetLoadPoint();
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

void GA_SingleObjective::RunBase() {
    SimulationType::Run();
}

void GA_SingleObjective::Load() {
    SimulationType::Load();
    this->CreateGA();
    this->gaAlgorithm->Initialize();
}

void GA_SingleObjective::LoadFile() {
    SimulationType::LoadFile();
    this->CreateGA();
    this->gaAlgorithm->Initialize();
}

void GA_SingleObjective::Print() {
    this->Help();
    SimulationType::Print();
}

void GA_SingleObjective::Save() {
    SimulationType::Save();
    this->GetData()->SaveGaFiles();
}

void GA_SingleObjective::Help() {
    std::cout << "GA SIMULATION" << std::endl
              << "This type of simulation applies the genetic algorithm." 
              << std::endl;
    std::cout << "The specific GA is determined by the option chosen."
              << std::endl << std::endl;
}

GA* GA_SingleObjective::GetGA() const {
    return this->gaAlgorithm.get();
}

void GA_SingleObjective::CreateGA() {
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
    }
}

void GA_SingleObjective::SetLoadPoint() {
    unsigned int point = 0;
    
    switch(point){
        case 0:
            this->GetCallGenerator()->SetNetworkLoad(this->
            GetParameters()->GetMinLoadPoint());
            break;
        case 1:
            this->GetCallGenerator()->SetNetworkLoad(this->
            GetParameters()->GetMidLoadPoint());
            break;
        case 2:
            this->GetCallGenerator()->SetNetworkLoad(this->
            GetParameters()->GetMaxLoadPoint());
            break;
        default:
            std::cerr << "Invalid load point" << std::endl;
            abort();
    }
}
