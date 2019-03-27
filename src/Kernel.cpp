/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Kernel.cpp
 * Author: bruno
 * 
 * Created on August 2, 2018, 3:42 PM
 */

#include "../include/Kernel.h"

#include "../include/SimulationType/SimulationType.h"
#include "../include/SimulationType/MultiLoadSimulation.h"
#include "../include/SimulationType/GA_SingleObjective.h"

Kernel::Kernel(int numSimulations)
:numberSimulations(numSimulations),
simulations(0) {
    
}

Kernel::~Kernel() {
    
    for(auto it : simulations)
        it.reset();
}

void Kernel::Run() {
    
    this->CreateSimulations();
    
    for(auto it: this->simulations){
        this->Pre_Simulation(it.get());
        this->Simulation(it.get());
        this->Pos_Simulation(it.get());
    }
}

void Kernel::CreateSimulations() {
    
    for(unsigned int a = 1; a <= this->numberSimulations; ++a){
    //    if(a == 1)
    //        simulations.push_back(std::make_shared<GA_SingleObjective>(a));
    //    else
            simulations.push_back(std::make_shared<MultiLoadSimulation>(a));
    }
}

void Kernel::Pre_Simulation(SimulationType* simul) {
    
    simul->LoadFile();
    simul->AdditionalSettings();
}

void Kernel::Simulation(SimulationType* simul) {
    
    simul->Print();
    simul->Run();
}

void Kernel::Pos_Simulation(SimulationType* simul) {
    
    simul->Save();
}