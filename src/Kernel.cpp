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
#include "../include/SimulationType/IncNumRegSimulation.h"

const boost::unordered_map<TypeSimulation, std::string> 
Kernel::mapSimulationType = boost::assign::map_list_of
    (InvalidSimulation, "Invalid")
    (MultiLoadSimulationType, "Multiple Load Simulation")
    (IncNumRegType, "Increase Number of Regenerators")
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
    unsigned int auxSimulation;
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
        std::cin >> auxSimulation;
        typeSimul = (TypeSimulation) auxSimulation;
        
        switch(typeSimul){
            case MultiLoadSimulationType:
                simulations.push_back(std::make_shared<MultiLoadSimulation>(a, 
                                      typeSimul));
                break;
            case IncNumRegType:
                simulations.push_back(std::make_shared<IncNumRegSimulation>(a, 
                                      typeSimul));
                break;
            case GaSimulationType:
                simulations.push_back(std::make_shared<GA_SingleObjective>(a, 
                                      typeSimul));
                break;
            default:
                std::cerr << "Invalid simulation type" << std::endl;
        }
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