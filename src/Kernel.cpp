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
    (IncNumDevType, "Increase Number of Devices")
    (GaSimulationType, "GA Simulation");

Kernel::Kernel()
:numberSimulations(0), simulations(0) {
    
}

Kernel::~Kernel() {
    
    simulations.clear();
}

void Kernel::Run() {
    
    this->CreateSimulations();
    
    for(unsigned int a = 0; a < simulations.size(); a++){
        this->Pre_Simulation(simulations.at(a).get());
        this->Simulation(simulations.at(a).get());
        this->Pos_Simulation(simulations.at(a).get());
        simulations.at(a).reset();
    }
}

void Kernel::CreateSimulations() {
    unsigned int auxIndex;
    TypeSimulation typeSimul;
    std::shared_ptr<SimulationType> auxSimulation;
    
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
        
        switch(typeSimul){
            case MultiLoadSimulationType:
                auxSimulation = std::make_shared<SimulationMultiLoad>(a, 
                                      typeSimul);
                break;
            case IncNumDevType:
                auxSimulation = std::make_shared<SimulationMultiNumDevices>(a, 
                                      typeSimul);
                break;
            case GaSimulationType:
                auxSimulation = std::make_shared<SimulationGA>(a, 
                                      typeSimul);
                break;
            default:
                std::cerr << "Invalid simulation type" << std::endl;
                std::abort();
        }
        simulations.push_back(auxSimulation);
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
