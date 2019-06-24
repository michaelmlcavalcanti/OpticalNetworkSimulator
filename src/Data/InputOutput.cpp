/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InputOutput.cpp
 * Author: bruno
 * 
 * Created on August 8, 2018, 8:14 PM
 */

#include <iostream>

#include "../../include/Data/InputOutput.h"

#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Data/Options.h"

const int InputOutput::barWidth = 30;

InputOutput::InputOutput(SimulationType* simulType)
:simulType(simulType) {
    this->LoadLog(this->logFile);
    this->LoadResults(this->resultFile);
    this->LoadGaFiles(this->bestIndividuals, this->bestIndividual, 
                      this->worstIndividuals, this->initialPopulation);
}

InputOutput::~InputOutput() {
    
}

void InputOutput::LoadParameters(std::ifstream& parameters) {
    int auxInt = this->simulType->GetSimulationIndex();
    
    do{
        parameters.open("Files/Inputs/Parameters_" + 
        std::to_string(auxInt) + ".txt");
        
        if(!parameters.is_open()) {       
            std::cerr << "Wrong parameter file." << std::endl;
            std::cerr << "The file required is: Parameters_" 
                      << auxInt << ".txt" << std::endl;
            std::cerr << "Add/Fix the file then press 'Enter'" 
                      << std::endl;
            
            std::cin.get();
        }

    }while(!parameters.is_open());
}

void InputOutput::LoadOptions(std::ifstream& options) {
    int auxInt = this->simulType->GetSimulationIndex();
    
    do{
        options.open("Files/Inputs/Options_" + 
        std::to_string(auxInt) + ".txt");
        
        if(!options.is_open()) {       
            std::cerr << "Wrong options file." << std::endl;
            std::cerr << "The file required is: Options_" 
                      << auxInt << ".txt";
            std::cerr << "Add/Fix the file then press 'Enter'" 
                      << std::endl;
            
            std::cin.get();
        }

    }while(!options.is_open());
}

void InputOutput::LoadTopology(std::ifstream& topology) {
    std::string topologyName = 
    this->simulType->GetOptions()->GetTopologyName();
    
    do{
        topology.open("Files/Inputs/Topologies/" + 
        topologyName + ".txt");
        
        if(!topology.is_open()) {       
            std::cerr << "Wrong topology file." << std::endl;
            std::cerr << "The file required is: " 
                      << topologyName << ".txt";
            std::cerr << "Add/Fix the file then press 'Enter'" 
                      << std::endl;
            
            std::cin.get();
        }
    }while(!topology.is_open());
}

void InputOutput::LoadTraffic(std::ifstream& traffic) {
    std::string trafficName = 
    this->simulType->GetOptions()->GetTrafficName();
    
    do{
        traffic.open("Files/Inputs/Traffics/Traffic_" + 
        trafficName + ".txt");
        
        if(!traffic.is_open()) {       
            std::cerr << "Wrong traffic file." << std::endl;
            std::cerr << "The file required is: Traffic_" 
                      << trafficName << ".txt" << std::endl;
            std::cerr << "Add/Fix the file then press 'Enter'" 
                      << std::endl;
            
            std::cin.get();
        }
    }while(!traffic.is_open());
}

void InputOutput::LoadGA(std::ifstream& gaParam) {
    int auxInt = this->simulType->GetSimulationIndex();
    
    do{
        gaParam.open("Files/Inputs/GA/GA_" + 
        std::to_string(auxInt) + ".txt");
        
        if(!gaParam.is_open()) {       
            std::cerr << "Wrong options file." << std::endl;
            std::cerr << "The file required is: GA_" 
                      << auxInt << ".txt";
            std::cerr << "Add/Fix the file then press 'Enter'" 
                      << std::endl;
            
            std::cin.get();
        }

    }while(!gaParam.is_open());
}

void InputOutput::LoadGA_SO(std::ifstream& gaSoParam) {
    int auxInt = this->simulType->GetSimulationIndex();
    
    do{
        gaSoParam.open("Files/Inputs/GA/GA_SO_" + 
        std::to_string(auxInt) + ".txt");
        
        if(!gaSoParam.is_open()) {       
            std::cerr << "Wrong options file." << std::endl;
            std::cerr << "The file required is: GA_SO_" 
                      << auxInt << ".txt";
            std::cerr << "Add/Fix the file then press 'Enter'" 
                      << std::endl;
            
            std::cin.get();
        }

    }while(!gaSoParam.is_open());
}

void InputOutput::LoadGA_MO(std::ifstream& gaMoParam) {
    int auxInt = this->simulType->GetSimulationIndex();
    
    do{
        gaMoParam.open("Files/Inputs/GA/GA_MO_" + 
        std::to_string(auxInt) + ".txt");
        
        if(!gaMoParam.is_open()) {       
            std::cerr << "Wrong options file." << std::endl;
            std::cerr << "The file required is: GA_MO_" 
                      << auxInt << ".txt";
            std::cerr << "Add/Fix the file then press 'Enter'" 
                      << std::endl;
            
            std::cin.get();
        }

    }while(!gaMoParam.is_open());
}

void InputOutput::LoadRsaOrderFirstSimul(std::ifstream& orderRsa) {
    
    do{
        orderRsa.open("Files/Outputs/1/GA/BestIndividual.txt");
        
        if(!orderRsa.is_open()) {
            std::cerr << "Wrong RSA order file." << std::endl;
            std::cerr << "The file required is: Files/Outputs/"
            "1/GA/BestIndividual.txt" << std::endl;
            std::cerr << "Add/Fix the file then press 'Enter'" 
                      << std::endl;
            
            std::cin.get();
        }
    }while(!orderRsa.is_open());
}

void InputOutput::LoadResults(std::ofstream& pBvLoad) {
    unsigned int auxInt = this->simulType->GetSimulationIndex();
    
    do{
        pBvLoad.open("Files/Outputs/" + std::to_string(auxInt) 
                     + "/PBvLoad.txt");
        
        if(!pBvLoad.is_open()){
            std::cerr << "Wrong result file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!pBvLoad.is_open());
}

void InputOutput::LoadLog(std::ofstream& log) {
    unsigned int auxInt = this->simulType->GetSimulationIndex();
    
    do{
        log.open("Files/Outputs/" + std::to_string(auxInt) 
                     + "/Log.txt");
        
        if(!log.is_open()){
            std::cerr << "Wrong log file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!log.is_open());
}

void InputOutput::LoadGaFiles(std::ofstream& bests, std::ofstream& best, 
std::ofstream& worst, std::ofstream& iniPop) {
    unsigned int auxInt = this->simulType->GetSimulationIndex();
    
    do{
        bests.open("Files/Outputs/" + std::to_string(auxInt)
                  + "/GA/BestIndividuals.txt");
        
        if(!bests.is_open()){
            std::cerr << "Wrong best individuals file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/GA/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!bests.is_open());
    
    do{
        best.open("Files/Outputs/" + std::to_string(auxInt)
                  + "/GA/BestIndividual.txt");
        
        if(!bests.is_open()){
            std::cerr << "Wrong best individuals file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/GA/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!best.is_open());
    
    do{
        worst.open("Files/Outputs/" + std::to_string(auxInt)
                  + "/GA/WorstIndividuals.txt");
        
        if(!worst.is_open()){
            std::cerr << "Wrong worst individuals file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/GA/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!worst.is_open());
    
    do{
        iniPop.open("Files/Outputs/" + std::to_string(auxInt)
                  + "/GA/InitialPopulation.txt");
        
        if(!iniPop.is_open()){
            std::cerr << "Wrong initial population file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/GA/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!iniPop.is_open());
}

std::ofstream& InputOutput::GetLogFile() {
    return this->logFile;
}

std::ofstream& InputOutput::GetResultFile() {
    return this->resultFile;
}

std::ofstream& InputOutput::GetBestIndividualsFile() {
    return this->bestIndividuals;
}

std::ofstream& InputOutput::GetBestIndividualFile() {
    return this->bestIndividual;
}

std::ofstream& InputOutput::GetWorstIndividualsFile() {
    return this->worstIndividuals;
}

std::ofstream& InputOutput::GetIniPopulationFile() {
    return this->initialPopulation;
}

std::ofstream& InputOutput::LoadTable() {
    unsigned int auxInt = this->simulType->GetSimulationIndex();
    
    table.open("Files/Outputs/" + std::to_string(auxInt) 
                     + "/Table.txt");
    
    return table;
}

void InputOutput::PrintProgressBar(unsigned int actual, unsigned int max) {
    double division = (double) actual / max;
    int pos = barWidth * division;
    
    std::cout << "[";
    
    for(int a = 0; a < barWidth; a++){
        if(a < pos)
            std::cout << "=";
        else if(a == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    
    std::cout << "]" << int(division * 100.0) << "%" << std::endl;
    std::cout.flush();
}

void InputOutput::PrintProgressBar(unsigned actual, unsigned min, unsigned max) {
    double division = (double) actual / (max - min);
    int pos = barWidth * division;
    
    std::cout << "[";
    
    for(int a = 0; a < barWidth; a++){
        if(a < pos)
            std::cout << "=";
        else if(a == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    
    std::cout << "]" << int(division * 100.0) << "%" << std::endl;
    std::cout.flush();
}
