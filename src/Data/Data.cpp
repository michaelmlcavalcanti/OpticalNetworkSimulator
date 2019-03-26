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

#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Calls/Call.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/SimulationType/GA_SingleObjective.h"
#include "../../include/Algorithms/GA/GA.h"
#include "../../include/Algorithms/GA/GA_SO.h"
#include "../../include/Algorithms/GA/IndividualBool.h"

std::ostream& operator<<(std::ostream& ostream, 
const Data* data) {    
    ostream << "Simulation time:" << data->GetSimulTime() 
            << "  Number of requests:" << data->GetNumberReq() << std::endl;
    
    ostream << "Load point:" << data->simulType->GetParameters()->
            GetLoadPoint(data->GetActualIndex()) << "  PbReq:" << 
            data->GetNumberBlocReq()/data->GetNumberReq() 
            << "  PbSlots:" << data->GetNumberBlocSlots()/
            data->GetNumberSlotsReq() << "  HopsMed:" 
            << data->GetNumHopsPerRoute()/data->GetNumberAccReq() 
            << "  NetOcc:" << data->GetNetOccupancy() << std::endl;
    
    ostream << std::endl;
    
    return ostream;
}

Data::Data(SimulationType* simulType) 
:simulType(simulType), numberReq(0), numberBlocReq(0), numberAccReq(0), 
numberSlotsReq(0), numberBlocSlots(0), numberAccSlots(0),
numHopsPerRoute(0), netOccupancy(0), simulTime(0), actualIndex(0) {
    
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
    this->simulTime.assign(size, 0.0);
}

void Data::StorageCall(Call* call) {
    unsigned int numHops = call->GetRoute()->GetNumHops();
    unsigned int numSlots = call->GetNumberSlots();
    
    switch(call->GetStatus()){
        case Accepted:
            this->numberAccReq.at(this->actualIndex)++;
            this->numHopsPerRoute.at(this->actualIndex) += (double) numHops;
            this->netOccupancy.at(this->actualIndex) += 
            (double) numSlots * numHops;
            break;
        case Blocked:
            this->numberBlocReq.at(this->actualIndex)++;
            break;
        case NotEvaluated:
            std::cerr << "Not evaluated call" <<  std::endl;
    }   
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

void Data::SavePBvLoad() {
    std::ofstream &resultOfstream = this->simulType->GetInputOutput()
                                        ->GetResultFile();
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        this->SavePBvLoad(resultOfstream);
    }
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
    
    //Make function to check the cast for each possible GA SimulationType
    GA_SO* ga = dynamic_cast<GA_SO*>( dynamic_cast<GA_SingleObjective*>
                                      (this->simulType)->GetGA() );
    
    this->SaveBestWorstIndividuals(ga, logOfstream, bestInds, worstInds);
    this->SaveBestIndividual(ga, bestInd);
    this->SaveInitPopulation(ga, initPop);
}

void Data::SetNumberReq(double numReq) {
    assert(numReq >= 0);
    this->numberReq.at(this->actualIndex) = numReq;
}

double Data::GetNumberReq(unsigned int index) const {
    return this->numberReq.at(index);
}

double Data::GetNumberReq() const {
    return this->numberReq.at(this->actualIndex);
}

double Data::GetNumberBlocReq(unsigned int index) const {
    return this->numberBlocReq.at(index);
}

double Data::GetNumberBlocReq() const {
    return this->numberBlocReq.at(this->actualIndex);
}

double Data::GetNumberAccReq(unsigned int index) const {
    return this->numberAccReq.at(index);
}

double Data::GetNumberAccReq() const {
    return this->numberAccReq.at(this->actualIndex);
}

double Data::GetPbReq() const {
    return this->GetNumberBlocReq()/this->GetNumberReq();
}

double Data::GetNumberSlotsReq() const {
    return this->numberSlotsReq.at(this->actualIndex);
}

double Data::GetNumberBlocSlots(unsigned int index) const {
    return this->numberBlocSlots.at(index);
}

double Data::GetNumberBlocSlots() const {
    return this->numberBlocSlots.at(this->actualIndex);
}

double Data::GetNumberAccSlots(unsigned int index) const {
    return this->numberAccSlots.at(index);
}

double Data::GetNumberAccSlots() const {
    return this->numberAccSlots.at(this->actualIndex);
}

double Data::GetPbSlots() const {
    return this->GetNumberBlocSlots()/this->GetNumberSlotsReq();
}

double Data::GetNumHopsPerRoute(unsigned int index) const {
    return this->numHopsPerRoute.at(index);
}

double Data::GetNumHopsPerRoute() const {
    return this->numHopsPerRoute.at(this->actualIndex);
}

double Data::GetNetOccupancy(unsigned int index) const {
    return this->netOccupancy.at(index);
}

double Data::GetNetOccupancy() const {
    return this->netOccupancy.at(this->actualIndex);
}

TIME Data::GetSimulTime() const {
    return this->simulTime.at(this->actualIndex);
}

void Data::SetSimulTime(const TIME simulTime) {
    assert(simulTime > 0);
    this->simulTime.at(this->actualIndex) = simulTime;
}

unsigned int Data::GetActualIndex() const {
    return actualIndex;
}

void Data::SetActualIndex(unsigned int actualIndex) {
    this->actualIndex = actualIndex;
}

void Data::SavePBvLoad(std::ostream& ostream) {   
    ostream << this->simulType->GetParameters()->GetLoadPoint(this
                   ->GetActualIndex()) << "\t" << this->GetNumberBlocReq()/
                   this->GetNumberReq() << std::endl;
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
