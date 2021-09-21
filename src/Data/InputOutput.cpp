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

#include "../../include/Data/InputOutput.h"
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Data/Options.h"

InputOutput::InputOutput(SimulationType* simulType)
:simulType(simulType) {
    this->LoadLog(logFile);
    this->LoadReqBP(reqBpFile);
    this->LoadBandBP(bandBpFile);
    this->LoadGaFiles(bestIndividuals, bestIndividual, 
                      worstIndividuals, initialPopulation);
    this->LoadPsoFiles(bestParticle, bestParticles);
    this->LoadNetUtiliz(netUtilizFile);
    this->LoadNetFrag(netFragFile);
    this->LoadAccumNetFrag(accumNetFragFile);
    this->LoadFragBand(fragBand);
    this->LoadLinksUse(linksUse);
    this->LoadSlotsRelativeUse(slotsRelativeUse);
    this->LoadNetProtRate(netProtRate);
    this->LoadNetNonProtRate(netNonProtRate);
    this->LoadNetNonProtRate(netNonProtRate);
    this->LoadNetBetaAverage(netBetaAverage);
    this->LoadNetAlphaAverage(netAlphaAverage);
    this->LoadNumHopsRoutes(numHopsRoutes);
}

InputOutput::~InputOutput() = default;

void InputOutput::LoadParameters(std::ifstream& parameters) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
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
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
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
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
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
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
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
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
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
        //orderRsa.open("Files/Inputs/GA/BestIndividual.txt");

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

void InputOutput::LoadPDPPBitRateNodePairsDistFirstSimul(std::ifstream& bitRateDist){

    do{
        bitRateDist.open("Files/Outputs/1/GA/BestIndividual.txt");
        //bitRateDist.open("Files/Inputs/GA/BestIndividual.txt");

        if(!bitRateDist.is_open()) {
            std::cerr << "Wrong Bit Rate Distribuition file." << std::endl;
            std::cerr << "The file required is: Files/Outputs/"
                         "1/GA/BestIndividual.txt" << std::endl;
            std::cerr << "Add/Fix the file then press 'Enter'"
                      << std::endl;

            std::cin.get();
        }
    }while(!bitRateDist.is_open());
}

void InputOutput::LoadDevicesFile(std::ifstream& devicesFile) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
    do{
        devicesFile.open("Files/Inputs/Devices/Devices_" + 
        std::to_string(auxInt) + ".txt");
        
        if(!devicesFile.is_open()) {
            std::cerr << "Wrong options file." << std::endl;
            std::cerr << "The file required is: Devices" 
                      << auxInt << ".txt";
            std::cerr << "Add/Fix the file then press 'Enter'" 
                      << std::endl;
            
            std::cin.get();
        }

    }while(!devicesFile.is_open());
}

void InputOutput::LoadPSO(std::ifstream& psoFile) {
    const SimulIndex auxInt = simulType->GetSimulationIndex();
    std::string auxString = std::to_string(auxInt);
    
    do{
        psoFile.open("Files/Inputs/PSO/PSO_" + auxString + ".txt");
        
        if(!psoFile.is_open()){
            std::cerr << "Wrong PSO file." << std::endl;
            std::cerr << "The file required is: PSO_" + auxString + ".txt";
            std::cerr << "Add/Fix the file then press 'Enter'" << std::endl;
            
            std::cin.get();
        }
    }while(!psoFile.is_open());
}

void InputOutput::LoadCoefficientsSCRA(std::ifstream& coeSCRA) {
    
    do{
        coeSCRA.open("Files/Outputs/1/PSO/BestParticle.txt");
        
        if(!coeSCRA.is_open()) {
            std::cerr << "Wrong PSO coefficients file." << std::endl;
            std::cerr << "The file required is: Files/Outputs/"
            "1/PSO/BestParticle.txt" << std::endl;
            std::cerr << "Add/Fix the file then press 'Enter'" 
                      << std::endl;
            
            std::cin.get();
        }
    }while(!coeSCRA.is_open());
}

void InputOutput::LoadReqBP(std::ofstream& pBvLoad) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
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

void InputOutput::LoadBandBP(std::ofstream& bandBP) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
    do{
        bandBP.open("Files/Outputs/" + std::to_string(auxInt) 
                     + "/bandBP.txt");
        
        if(!bandBP.is_open()){
            std::cerr << "Wrong result file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!bandBP.is_open());
}

void InputOutput::LoadNetUtiliz(std::ofstream& netUtiliz) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
    do{
        netUtiliz.open("Files/Outputs/" + std::to_string(auxInt) 
                     + "/NetUtiliz.txt");
        
        if(!netUtiliz.is_open()){
            std::cerr << "Wrong result file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!netUtiliz.is_open());
}

void InputOutput::LoadNetFrag(std::ofstream& netFrag) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
    do{
        netFrag.open("Files/Outputs/" + std::to_string(auxInt) 
                     + "/NetFrag.txt");
        
        if(!netFrag.is_open()){
            std::cerr << "Wrong result file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!netFrag.is_open());
}

void InputOutput::LoadAccumNetFrag(std::ofstream& accumNetFrag) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
    do{
        accumNetFrag.open("Files/Outputs/" + std::to_string(auxInt) 
                     + "/AccumNetFrag.txt");
        
        if(!accumNetFrag.is_open()){
            std::cerr << "Wrong result file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!accumNetFrag.is_open());
}

void InputOutput::LoadFragBand(std::ofstream& fragBand) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
    do{
        fragBand.open("Files/Outputs/" + std::to_string(auxInt)
                     + "/FragBand.txt");
        
        if(!fragBand.is_open()){
            std::cerr << "Wrong log file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!fragBand.is_open());
}

void InputOutput::LoadLog(std::ofstream& log) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
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

void InputOutput::LoadLinksUse(std::ofstream& linksUse) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
    do{
        linksUse.open("Files/Outputs/" + std::to_string(auxInt) 
                     + "/LinksUse.txt");
        
        if(!linksUse.is_open()){
            std::cerr << "Wrong log file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!linksUse.is_open());
}

void InputOutput::LoadSlotsRelativeUse(std::ofstream& slotsRelativeUse) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
    do{
        slotsRelativeUse.open("Files/Outputs/" + std::to_string(auxInt) 
                     + "/SlotsRelativeUse.txt");
        
        if(!slotsRelativeUse.is_open()){
            std::cerr << "Wrong log file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!slotsRelativeUse.is_open());
}

void InputOutput::LoadNetProtRate(std::ofstream& netProtRate) {
    unsigned int auxInt = this->simulType->GetSimulationIndex();
    
    do{
        netProtRate.open("Files/Outputs/" + std::to_string(auxInt) 
                     + "/netProtRate.txt");
        
        if(!netProtRate.is_open()){
            std::cerr << "Wrong log file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!netProtRate.is_open());
}

void InputOutput::LoadNetNonProtRate(std::ofstream& netNonProtRate) {
    unsigned int auxInt = this->simulType->GetSimulationIndex();
    
    do{
        netNonProtRate.open("Files/Outputs/" + std::to_string(auxInt) 
                     + "/netNonProtRate.txt");
        
        if(!netNonProtRate.is_open()){
            std::cerr << "Wrong log file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!netNonProtRate.is_open());
}

void InputOutput::LoadNetBetaAverage(std::ofstream& netBetaAverage) {
    unsigned int auxInt = this->simulType->GetSimulationIndex();
    
    do{
        netBetaAverage.open("Files/Outputs/" + std::to_string(auxInt)
                            + "/netBetaAverage.txt");
        
        if(!netBetaAverage.is_open()){
            std::cerr << "Wrong log file." << std::endl;
            std::cerr << "The folder required is: " << auxInt 
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;
            
            std::cin.get();
        }
    }while(!netBetaAverage.is_open());
}

void InputOutput::LoadNetAlphaAverage(std::ofstream &netAlphaAverage) {
    unsigned int auxInt = this->simulType->GetSimulationIndex();

    do{
        netAlphaAverage.open("Files/Outputs/" + std::to_string(auxInt)
                            + "/netAlphaAverage.txt");

        if(!netAlphaAverage.is_open()){
            std::cerr << "Wrong log file." << std::endl;
            std::cerr << "The folder required is: " << auxInt
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;

            std::cin.get();
        }
    }while(!netAlphaAverage.is_open());
}

void InputOutput::LoadNumHopsRoutes(std::ofstream &numHopsRoutes) {
    unsigned int auxInt = this->simulType->GetSimulationIndex();

    do{
        numHopsRoutes.open("Files/Outputs/" + std::to_string(auxInt)
                            + "/numHopsRoutes.txt");

        if(!numHopsRoutes.is_open()){
            std::cerr << "Wrong log file." << std::endl;
            std::cerr << "The folder required is: " << auxInt
                      << "/" << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'"
                      << std::endl;

            std::cin.get();
        }
    }while(!numHopsRoutes.is_open());
}


void InputOutput::LoadGaFiles(std::ofstream& bests, std::ofstream& best, 
std::ofstream& worst, std::ofstream& iniPop) {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
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

void InputOutput::LoadPsoFiles(std::ofstream& best, std::ofstream& bests) {
    const SimulIndex index = simulType->GetSimulationIndex();
    std::string string = std::to_string(index);
    
    do{
        best.open("Files/Outputs/" + string + "/PSO/BestParticle.txt");
        
        if(!best.is_open()){
            std::cerr << "Wrong best particle file." << std::endl;
            std::cerr << "The folder required is: " << string << "/PSO/" 
                      << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'" << std::endl;
            
            std::cin.get();
        }
    }while(!best.is_open());
    
    do{
        bests.open("Files/Outputs/" + string + "/PSO/BestParticles.txt");
        
        if(!bests.is_open()){
            std::cerr << "Wrong best particles file." << std::endl;
            std::cerr << "The folder required is: " << string << "/PSO/" 
                      << std::endl;
            std::cerr << "Add/Fix the folder, then press 'Enter'" << std::endl;
            
            std::cin.get();
        }
    }while(!bests.is_open());
}

std::ofstream& InputOutput::GetLogFile() {
    return this->logFile;
}

std::ofstream& InputOutput::GetReqBpFile() {
    return this->reqBpFile;
}

std::ofstream& InputOutput::GetBandBpFile() {
    return bandBpFile;
}

std::ofstream& InputOutput::GetNetUtilizFile() {
    return netUtilizFile; 
}

std::ofstream& InputOutput::GetNetFragFile() {
    return netFragFile;
}

std::ofstream& InputOutput::GetAccumNetFragFile() {
    return accumNetFragFile;
}

std::ofstream& InputOutput::GetFragBandFile() {
    return fragBand;
}

std::ofstream& InputOutput::GetLinksUse() {
    return linksUse;
}

std::ofstream& InputOutput::GetSlotsRelativeUse() {
    return slotsRelativeUse;
}

std::ofstream& InputOutput::GetNetProtRate() {
    return netProtRate;
}

std::ofstream& InputOutput::GetNetNonProtRate() {
    return netNonProtRate;
}

std::ofstream& InputOutput::GetNetBetaAverage() {
    return netBetaAverage;
}

std::ofstream &InputOutput::GetNetAlphaAverage() {
    return netAlphaAverage;
}

std::ofstream &InputOutput::GetNumHopsRoutes() {
    return numHopsRoutes;
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

std::ofstream& InputOutput::GetBestParticle() {
    return bestParticle;
}

std::ofstream& InputOutput::GetBestParticles() {
    return bestParticles;
}

std::ofstream& InputOutput::LoadTable() {
    const SimulIndex auxInt = this->simulType->GetSimulationIndex();
    
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








