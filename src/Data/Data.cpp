/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Data.cpp
 * Author: bruno
 * 
 * Created on August 8, 2018, 6:25 PM
 */

#include "../../include/Data/Data.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/Options.h"
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/SimulationType/SimulationGA.h"
#include "../../include/SimulationType/SimulationPSO.h"
#include "../../include/Calls/Call.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/Algorithms/Algorithms.h"
#include "../../include/Structure/Topology.h"

std::ostream& operator<<(std::ostream& ostream, 
const Data* data) {
    ostream << "Simulation time:" << data->GetSimulTime() 
            << "  Real simul time: " << data->GetRealSimulTime()
            << "  Number of requests:" << data->GetNumberReq() << std::endl;
    
    TypeSimulation type = data->simulType->GetTypeSimulation();
    switch(type){
        case MultiLoadSimulationType:
            ostream << "Load point:" << data->simulType->GetParameters()->
            GetLoadPoint(data->GetActualIndex());
            break;
        case IncNumDevType:
            ostream << "Load point:" << data->simulType->GetParameters()->
            GetMaxLoadPoint();
            break;
        default:
            ostream <<  "Invalid Type of simulation" << std::endl;
            std::abort();
    }
    
    ostream << "  ReqBP:" << data->GetReqBP() 
            << "  SlotsBP:" << data->GetSlotsBP() 
            << "  HopsMed:" << data->GetMeanNumHops() << std::endl;
    ostream << "  NetOcc:" << data->GetNetOccupancy() 
            << "  NetUti:" << data->GetNetUtilization() << std::endl;
    
    ostream << std::endl;
    
    return ostream;
}

Data::Data(SimulationType* simulType) 
:simulType(simulType), numberReq(0), numberBlocReq(0), numberAccReq(0), 
numberSlotsReq(0), numberBlocSlots(0), numberAccSlots(0),
numHopsPerRoute(0), netOccupancy(0), simulTime(0), realSimulTime(0), 
actualIndex(0) {
    
}

Data::~Data() {
    
}

void Data::Initialize() {
    int size = this->simulType->GetParameters()->GetNumberLoadPoints();
    this->numberReq.assign(size, 0.0);
    this->numberBlocReq.assign(size, 0.0);
    this->numberAccReq.assign(size, 0.0);
    this->numberSlotsReq.assign(size, 0.0);
    this->numberBlocSlots.assign(size, 0.0);
    this->numberAccSlots.assign(size, 0.0);
    this->numHopsPerRoute.assign(size, 0.0);
    this->netOccupancy.assign(size, 0.0);
    this->netUtilization.assign(size, 0.0);
    this->simulTime.assign(size, 0.0);
    this->realSimulTime.assign(size, 0.0);
}

void Data::Initialize(unsigned int numPos) {
    
    this->numberReq.assign(numPos, 0.0);
    this->numberBlocReq.assign(numPos, 0.0);
    this->numberAccReq.assign(numPos, 0.0);
    this->numberSlotsReq.assign(numPos, 0.0);
    this->numberBlocSlots.assign(numPos, 0.0);
    this->numberAccSlots.assign(numPos, 0.0);
    this->numHopsPerRoute.assign(numPos, 0.0);
    this->netOccupancy.assign(numPos, 0.0);
    this->netUtilization.assign(numPos, 0.0);
    this->simulTime.assign(numPos, 0.0);
    this->realSimulTime.assign(numPos, 0.0);
}

void Data::StorageCall(Call* call) {
    double bitRate = call->GetBitRate();
    
    switch(call->GetStatus()){
        case Accepted:
            numberAccReq.at(actualIndex)++;
            numberAccSlots.at(actualIndex) += bitRate;
            numHopsPerRoute.at(actualIndex) += (double) 
            call->GetRoute()->GetNumHops();
            netOccupancy.at(actualIndex) += 
            (double) call->GetTotalNumSlots();
            netUtilization.at(actualIndex) += ((double) call->GetTotalNumSlots())
            * call->GetDeactivationTime();
            break;
        case Blocked:
            numberBlocReq.at(actualIndex)++;
            numberBlocSlots.at(actualIndex) += bitRate;
            break;
        case NotEvaluated:
            std::cerr << "Not evaluated call" <<  std::endl;
            std::abort();
    }
    numberSlotsReq.at(actualIndex) += bitRate;
}

void Data::SaveMultiloadLog() {
    std::ofstream &logOfstream = this->simulType->GetInputOutput()
                                     ->GetLogFile();
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    logOfstream << "DATA" << std::endl;
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        logOfstream << this << std::endl;
    }
}

void Data::SaveLog(std::vector<unsigned> vecParam) {
    std::ofstream &logOfstream = this->simulType->GetInputOutput()
                                     ->GetLogFile();
    unsigned int numPoints = vecParam.size();
    
    logOfstream << "DATA" << std::endl;
    
    for(unsigned int a = 0; a < numPoints; a++){
        this->SetActualIndex(a);
        logOfstream << "Number of devices: " << vecParam.at(a) << std::endl;
        logOfstream << this << std::endl;
    }
}

void Data::SaveBP() {
    std::ofstream &callReqBP = this->simulType->GetInputOutput()
                                        ->GetResultFile();
    std::ofstream &bandwidthBP = simulType->GetInputOutput()
                                          ->GetBandBpFile();
    
    this->SaveCallReqBP(callReqBP);
    this->SaveBandwidthBP(bandwidthBP);
}

void Data::SaveBP(std::vector<unsigned> vecParam) {
    std::ofstream &callReqBP = this->simulType->GetInputOutput()
                                        ->GetResultFile();
    std::ofstream &bandwidthBP = simulType->GetInputOutput()
                                          ->GetBandBpFile();
    
    this->SaveCallReqBP(callReqBP, vecParam);
    this->SaveBandwidthBP(bandwidthBP, vecParam);
}

void Data::SaveGaFiles() {
    std::ofstream& logOfstream = this->simulType->GetInputOutput()
                                     ->GetLogFile();
    std::ofstream& initPop = this->simulType->GetInputOutput()
                                        ->GetIniPopulationFile();
    std::ofstream& bestInds = this->simulType->GetInputOutput()
                                 ->GetBestIndividualsFile();
    std::ofstream& bestInd = this->simulType->GetInputOutput()
                                ->GetBestIndividualFile();
    std::ofstream& worstInds = this->simulType->GetInputOutput()
                                 ->GetWorstIndividualsFile();
    
    if(this->simulType->GetOptions()->IsGA_SO())
        this->SaveGaSoFiles(logOfstream, initPop, bestInds, worstInds, bestInd);
    else if(this->simulType->GetOptions()->IsGA_MO())
        this->SaveGaMoFiles(logOfstream, bestInds, bestInd);
}

void Data::SaveFilesPSO() {
    std::ofstream& logOfstream = simulType->GetInputOutput()->GetLogFile();
    std::ofstream& bestParticle = simulType->GetInputOutput()
                                           ->GetBestParticle();
    std::ofstream& bestParticles = simulType->GetInputOutput()
                                            ->GetBestParticles();
    PSO* pso = dynamic_cast<SimulationPSO*>(simulType)->GetPSO();
    unsigned int numIterations = pso->GetNumberIterations();
    
    pso->PrintParameters(logOfstream);
    
    //Save the Log.txt and the best particle of each iteration.
    for(unsigned int a = 1; a <= numIterations; a++){
        pso->SetActualIteration(a);
        logOfstream << pso << std::endl;
        bestParticles << a << "\t" << pso->GetBestParticle()->GetMainParameter()
                      << std::endl;
    }
    
    //Save the best particle coefficients.
    std::vector<double> coefficients = pso->GetBestParticle()
                                          ->GetBestPosition();
    
    for(unsigned int a = 0; a < coefficients.size(); a++)
        bestParticle << coefficients.at(a) << std::endl;
}

void Data::SetNumberReq(double numReq) {
    assert(numReq >= 0);
    this->numberReq.at(this->actualIndex) = numReq;
}

double Data::GetNumberReq() const {
    return this->numberReq.at(this->actualIndex);
}

double Data::GetNumberBlocReq() const {
    return this->numberBlocReq.at(this->actualIndex);
}

double Data::GetNumberAccReq() const {
    return this->numberAccReq.at(this->actualIndex);
}

double Data::GetReqBP() const {
    return this->GetNumberBlocReq()/this->GetNumberReq();
}

double Data::GetNumberSlotsReq() const {
    return this->numberSlotsReq.at(this->actualIndex);
}

double Data::GetNumberBlocSlots() const {
    return this->numberBlocSlots.at(this->actualIndex);
}

double Data::GetNumberAccSlots() const {
    return this->numberAccSlots.at(this->actualIndex);
}

double Data::GetSlotsBP() const {
    return this->GetNumberBlocSlots()/this->GetNumberSlotsReq();
}

double Data::GetNumHopsPerRoute() const {
    return this->numHopsPerRoute.at(this->actualIndex);
}

double Data::GetMeanNumHops() const {
    return (this->GetNumHopsPerRoute() / this->GetNumberAccReq());
}

double Data::GetNetOccupancy() const {
    return this->netOccupancy.at(this->actualIndex);
}

double Data::GetNetUtilization() const {
    double total = (double) simulType->GetTopology()->GetNumLinks() * 
    (double) simulType->GetTopology()->GetNumSlots() * simulTime.at(actualIndex);
    
    return netUtilization.at(actualIndex) / total;
}

TIME Data::GetSimulTime() const {
    return this->simulTime.at(this->actualIndex);
}

void Data::SetSimulTime(const TIME simulTime) {
    assert(simulTime > 0);
    this->simulTime.at(this->actualIndex) = simulTime;
}

TIME Data::GetRealSimulTime() const {
    return this->realSimulTime.at(this->actualIndex);
}

void Data::SetRealSimulTime(const TIME simulTime) {
    this->realSimulTime.at(this->actualIndex) = simulTime;
}

unsigned int Data::GetActualIndex() const {
    return actualIndex;
}

void Data::SetActualIndex(unsigned int actualIndex) {
    this->actualIndex = actualIndex;
}

void Data::SaveCallReqBP(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        ostream << this->simulType->GetParameters()->GetLoadPoint(
                   this->GetActualIndex()) << "\t" << this->GetReqBP() 
                << std::endl;
    }
}

void Data::SaveBandwidthBP(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        ostream << this->simulType->GetParameters()->GetLoadPoint(
                   this->GetActualIndex()) << "\t" << this->GetSlotsBP() 
                << std::endl;
    }
}

void Data::SaveCallReqBP(std::ostream& ostream, std::vector<unsigned> vec) {
    assert(vec.size() == this->numberReq.size());
    
    for(unsigned int a = 0; a < vec.size(); a++){
        this->SetActualIndex(a);
        ostream << vec.at(a) << "\t" << this->GetReqBP() << std::endl;
    }
}

void Data::SaveBandwidthBP(std::ostream& ostream, std::vector<unsigned> vec) {
    assert(vec.size() == this->numberReq.size());
    
    for(unsigned int a = 0; a < vec.size(); a++){
        this->SetActualIndex(a);
        ostream << vec.at(a) << "\t" << this->GetSlotsBP() << std::endl;
    }
}

void Data::SaveGaSoFiles(std::ostream& logOfstream, std::ostream& initPop, 
std::ostream& bestInds, std::ostream& worstInds, std::ostream& bestInd) {
    GA_SO* ga = dynamic_cast<GA_SO*>( dynamic_cast<SimulationGA*>
                                      (this->simulType)->GetGA() );
    
    ga->printParameters(logOfstream);
    this->SaveBestWorstIndividuals(ga, logOfstream, bestInds, worstInds);
    this->SaveBestIndividual(ga, bestInd);
    this->SaveInitPopulation(ga, initPop);
}

void Data::SaveBestWorstIndividuals(GA_SO* ga, std::ostream& logOfstream, 
std::ostream& bestInds, std::ostream& worstInds) {
    unsigned int numGen = ga->GetNumberGenerations();
    
    for(unsigned int a = 1; a <= numGen; a++){
        ga->SetActualGeneration(a);
        logOfstream << ga << std::endl;
        bestInds << a << "\t" << ga->GetBestIndividual()->GetMainParameter() 
                << std::endl;
        worstInds << a << "\t" << ga->GetWorstIndividual()->GetMainParameter()
                 << std::endl;
    }
}

void Data::SaveBestIndividual(GA_SO* ga, std::ostream& bestInd) {
    //Make function to check the cast for the best individual
    //and a switch function for casting according to the individual. 
    IndividualBool* ind = dynamic_cast<IndividualBool*>
                          (ga->GetBestIndividual());
    
    std::vector<bool> gene = ind->GetGenes();
    for(unsigned int a = 0; a < gene.size(); a++){
        bestInd << gene.at(a) << std::endl;
    }
}

void Data::SaveInitPopulation(GA_SO* ga, std::ostream& initPop) {
    unsigned int numIniPop = ga->GetNumberIndividuals();
    
    for(unsigned int a = 0; a < numIniPop; a++){
        initPop << 0 << "\t" << ga->GetIniIndividual(a)->GetMainParameter()
                << std::endl;
    }
}

void Data::SaveGaMoFiles(std::ostream& logOfstream, std::ostream& bestInds, 
std::ostream& bestInd) {
    GA_MO* ga = dynamic_cast<GA_MO*>( dynamic_cast<SimulationGA*>
                                      (this->simulType)->GetGA() );
    
    ga->printParameters(logOfstream);
    this->SaveParetoFronts(ga, logOfstream, bestInds);
    this->SaveLastIndividuals(ga, bestInd);
}

void Data::SaveParetoFronts(GA_MO* ga, std::ostream& logOfstream, 
std::ostream& bestInds) {
    std::vector<Individual*> auxVecInd(0);
    unsigned int numGen = ga->GetNumberGenerations();
    unsigned int passo = ga->GetSaveStep();
    
    for(unsigned int a = 0; a <= numGen; a++){  
        ga->SetActualGeneration(a);
        auxVecInd.clear();
        
        if(a == 0)
            auxVecInd = ga->GetIniPopulation();
        else if(a % passo == 0){
            logOfstream << ga << std::endl;
            auxVecInd = ga->GetParetoFront();
        }
        
        for(auto it: auxVecInd){
            bestInds << it->GetMainParameter() << "\t" 
                     << it->GetSecondParameter() << std::endl;
        }
        
        if(!auxVecInd.empty())
            bestInds << std::endl;
    }
}

void Data::SaveLastIndividuals(GA_MO* ga, std::ostream& bestInd) {
    //Make function to check the cast for the best individual
    //and a switch function for casting according to the individual.
    IndividualNumRoutesMSCL* ind;
    ga->SetActualGeneration(ga->GetNumberGenerations());
    std::vector<Individual*> auxVecInd = ga->GetParetoFront();
    std::vector<std::vector<unsigned int>> auxGenes(0);
    
    for(auto it1: auxVecInd){
        ind = dynamic_cast<IndividualNumRoutesMSCL*>(it1);
        auxGenes = ind->GetGenes();
        
        for(unsigned int a = 0; a < auxGenes.size(); a++){
            if(auxGenes.at(a).empty()){
                bestInd << 0 << "\t";
            }
            for(unsigned int b = 0; b < auxGenes.at(a).size(); b++){
                bestInd << auxGenes.at(a).at(b) << "\t";
            }
        }
        bestInd << std::endl;
    }
}
