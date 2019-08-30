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
#include "../include/SimulationType/SimulationMultiLoad.h"
#include "../include/SimulationType/SimulationGA.h"
#include "../include/SimulationType/SimulationMultiNumDevices.h"

const boost::unordered_map<TypeSimulation, std::string> 
Kernel::mapSimulationType = boost::assign::map_list_of
    (InvalidSimulation, "Invalid")
    (MultiLoadSimulationType, "Multiple Load Simulation")
    (IncNumDevType, "Increase Number of Regenerators")
    (GaSimulationType, "GA Simulation");

Kernel::Kernel()
:numberSimulations(0), simulations(0) {
    
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
    unsigned int auxIndex;
    TypeSimulation typeSimul;
    
    std::cout << "Type the number of simulations to perform: ";
    std::cin >> this->numberSimulations;
    std::cout << std::endl;
    
    std::cout << "List of simulations types:" << std::endl;
    for(TypeSimulation a = FirstSimulation; a <= LastSimulation; 
    a = TypeSimulation(a+1)){
        std::cout << a << "-" << this->mapSimulationType.at(a) << std::endl;
    }
    
    for(unsigned a = 1; a <= this->numberSimulations; a++){
        std::cout << "Enter the simulation type " << a << ":";
        std::cin >> auxIndex;
        typeSimul = (TypeSimulation) auxIndex;
        
        this->CreateSimulation(a, typeSimul);
    }
    std::cout << std::endl;
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

void Kernel::CreateSimulation(SimulIndex index, TypeSimulation type) {
    
    switch(type){
        case MultiLoadSimulationType:
            simulations.push_back(std::make_shared<SimulationMultiLoad>(
            index, type));
            break;
        case IncNumDevType:
            simulations.push_back(std::make_shared<SimulationMultiNumDevices>(
            index, type));
            break;
        case GaSimulationType:
            simulations.push_back(std::make_shared<SimulationGA>(index, type));
            break;
        default:
            std::cerr << "Invalid simulation type" << std::endl;
            std::abort();
    }
}
