/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Parameters.cpp
 * Author: bruno
 * 
 * Created on August 8, 2018, 6:25 PM
 */

#include "../../include/Data/Parameters.h"

#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Data/Options.h"

std::ostream& operator<<(std::ostream& ostream, 
const Parameters* parameters) {
    
    ostream << "PARAMETERS" << std::endl;
    ostream << "Number of slots per fiber: " 
            << parameters->GetNumberSlots() << std::endl;
    ostream << "Number of cores per link: " 
            << parameters->GetNumberCores() << std::endl;
    ostream << "Mu: " << parameters->GetMu() << std::endl;
    ostream << "Minimum load point(erlang): " 
            << parameters->GetMinLoadPoint() << std::endl;
    ostream << "Maximum load point(erlang): " 
            << parameters->GetMaxLoadPoint() << std::endl;
    ostream << "Number of points: " << parameters->GetNumberLoadPoints() 
            << std::endl;
    ostream << "Total number of requisitions: " 
            << parameters->GetNumberReqMax() << std::endl;
    ostream << "Maximum blocked requests: "
            << parameters->GetNumberBloqMax() << std::endl;
    ostream << "Slot bandwidth (GHz): "
            << parameters->GetSlotBandwidth()/1E9 << std::endl;
    if(parameters->simulType->GetOptions()->GetRoutingOption() ==
    RoutingYEN)
        ostream << "K: " << parameters->GetNumberRoutes() << std::endl;
    ostream << "Maximum section length (km): "
            << parameters->GetMaxSectionLegnth()/1E3 << std::endl;
    ostream << "Number of polarizations: " 
            << parameters->GetNumberPolarizations() << std::endl;
    ostream << "Guard band size (slots): " << parameters->GetGuardBand()
            << std::endl;
    ostream << "Number of PDPP Routes: " << parameters->GetNumberPDPPprotectionRoutes()
            << std::endl;
    ostream << "Beta Squeezing: " << parameters->GetBeta()
            << std::endl;
    
    return ostream;
}

Parameters::Parameters(SimulationType* simulType)
:simulType(simulType), loadPoint(0), minLoadPoint(0.0), maxLoadPoint(0.0), 
loadPasso(0.0), numberLoadPoints(0), numberReqMax(0.0), mu(0.0), 
numberBloqMax(0), slotBandwidth(0.0), numberSlots(0), numberCores(0), 
maxSectionLegnth(0.0), numberPolarizations(0), guardBand(0), beta(0), 
   numberPDPPprotectionRoutes(2) {
    
}

Parameters::~Parameters() {
    
}

void Parameters::Load() {
    unsigned int auxUnsInt;
    double auxDouble;
    
    std::cout << "PARAMETERS INPUTS" << std::endl;
    std::cout << "Insert the number of slots per fiber: ";
    std::cin >> auxUnsInt;
    this->SetNumberSlots(auxUnsInt);
    std::cout << "Insert the number cores per link: ";
    std::cin >> auxUnsInt;
    this->SetNumberCores(auxUnsInt);
    std::cout << "Insert Connection Deactivation Rate: ";
    std::cin >> auxDouble;
    this->SetMu(auxDouble);
    std::cout << "Insert minimum load point: ";
    std::cin >> auxDouble;
    this->SetMinLoadPoint(auxDouble);
    std::cout << "Insert maximum load point: ";
    std::cin >> auxDouble;
    this->SetMaxLoadPoint(auxDouble);
    std::cout << "Insert number of load points: ";
    std::cin >> auxUnsInt;
    this->SetNumberLoadPoints(auxUnsInt);
    std::cout << "Insert total number of calls: ";
    std::cin >> auxDouble;
    this->SetNumberReqMax(auxDouble);
    std::cout << "Insert maximum number of blocked calls: ";
    std::cin >> auxDouble;
    this->SetNumberBloqMax(auxDouble);
    std::cout << "Insert the value for K: ";
    std::cin >> auxUnsInt;
    this->SetNumberRoutes(auxUnsInt);
    std::cout << "Insert the maximum section length (km): ";
    std::cin >> auxDouble;
    this->SetMaxSectionLegnth(auxDouble);
    std::cout << "Insert the number of polarizations: ";
    std::cin >> auxUnsInt;
    this->SetNumberPolarizations(auxUnsInt);
    std::cout << "Insert the guard band size (slots): ";
    std::cin >> auxUnsInt;
    this->SetGuardBand(auxUnsInt);
    std::cout << "Insert the Protection Squeezing Beta: ";
    std::cin >> auxUnsInt;
    this->SetBeta(auxUnsInt);
    std::cout << "Insert the value for K Protection Routes: ";
    std::cin >> auxUnsInt;
    this->SetNumberPDPPprotectionRoutes(auxUnsInt);
    
    this->SetLoadPointUniform();
    
    std::cout << std::endl;
}

void Parameters::LoadFile() {
    std::ifstream auxIfstream;
    unsigned int auxInt;
    double auxDouble;
    
    this->simulType->GetInputOutput()->LoadParameters(auxIfstream);
    auxIfstream >> auxInt;
    this->SetNumberSlots(auxInt);
    auxIfstream >> auxInt;
    this->SetNumberCores(auxInt);
    auxIfstream >> auxDouble;
    this->SetMu(auxDouble);
    auxIfstream >> auxDouble;
    this->SetMinLoadPoint(auxDouble);
    auxIfstream >> auxDouble;
    this->SetMaxLoadPoint(auxDouble);
    auxIfstream >> auxInt;
    this->SetNumberLoadPoints(auxInt);
    auxIfstream >> auxDouble;
    this->SetNumberReqMax(auxDouble);
    auxIfstream >> auxDouble;
    this->SetNumberBloqMax(auxDouble);
    auxIfstream >> auxInt;
    this->SetNumberRoutes(auxInt);
    auxIfstream >> auxDouble;
    this->SetMaxSectionLegnth(auxDouble);
    auxIfstream >> auxInt;
    this->SetNumberPolarizations(auxInt);
    auxIfstream >> auxInt;
    this->SetGuardBand(auxInt);
    auxIfstream >> auxDouble;
    this->SetBeta(auxDouble);
    auxIfstream >> auxInt;
    this->SetNumberPDPPprotectionRoutes(auxInt);
    
    this->SetLoadPointUniform();
}

void Parameters::Save() {
    std::ofstream& auxOfstream = this->simulType->GetInputOutput()
                                     ->GetLogFile();
    
    auxOfstream << this << std::endl;
}

std::vector<double> Parameters::GetLoadPoint() const {
    return loadPoint;
}

double Parameters::GetLoadPoint(unsigned int index) const {
    assert(index < this->loadPoint.size());
    return loadPoint.at(index);
}

void Parameters::SetLoadPoint(std::vector<double> loadPoint) {
    assert(loadPoint.size() > 0);
    this->loadPoint = loadPoint;
}

void Parameters::SetLoadPoint(double loadPoint, unsigned int index) {
    assert(index < this->loadPoint.size());
    this->loadPoint.at(index) = loadPoint;
}

double Parameters::GetMinLoadPoint() const {
    return minLoadPoint;
}

void Parameters::SetMinLoadPoint(double minLoadPoint) {
    assert(minLoadPoint > 0.0);
    this->minLoadPoint = minLoadPoint;
}

double Parameters::GetMaxLoadPoint() const {
    return maxLoadPoint;
}

void Parameters::SetMaxLoadPoint(double maxLoadPoint) {
    assert(maxLoadPoint > 0.0);
    this->maxLoadPoint = maxLoadPoint;
}

double Parameters::GetMidLoadPoint() const {
    return this->minLoadPoint + (this->maxLoadPoint - this->minLoadPoint)/2;
}

unsigned int Parameters::GetNumberLoadPoints() const {
    return numberLoadPoints;
}

void Parameters::SetNumberLoadPoints(unsigned int numberLoadPoints) {
    this->numberLoadPoints = numberLoadPoints;
}

double Parameters::GetNumberReqMax() const {
    return numberReqMax;
}

void Parameters::SetNumberReqMax(double numberReqMax) {
    assert(numberReqMax > 0.0);
    this->numberReqMax = numberReqMax;
}

double Parameters::GetMu() const {
    return mu;
}

void Parameters::SetMu(double mu) {
    assert(mu > 0.0);
    this->mu = mu;
}

void Parameters::SetLoadPasso() {
    int auxInt = this->numberLoadPoints;
    
    if(auxInt == 1)
        auxInt++;
   
    this->loadPasso = (this->maxLoadPoint - this->minLoadPoint) / (auxInt - 1);
}

void Parameters::SetLoadPointUniform() {
    this->SetLoadPasso();
    
    for(unsigned int a = 0; a < this->numberLoadPoints; ++a){
        loadPoint.push_back(this->minLoadPoint + a*this->loadPasso);
    }
}

double Parameters::GetNumberBloqMax() const {
    return numberBloqMax;
}

void Parameters::SetNumberBloqMax(double numberBloqMax) {
    assert(numberBloqMax > 0.0);
    this->numberBloqMax = numberBloqMax;
}

double Parameters::GetSlotBandwidth() const {
    return slotBandwidth;
}

void Parameters::SetSlotBandwidth(double slotBandwidth) {
    assert(slotBandwidth > 0.0);
    this->slotBandwidth = slotBandwidth*1E9;
}

unsigned int Parameters::GetNumberSlots() const {
    return numberSlots;
}

void Parameters::SetNumberSlots(unsigned int numberSlots) {
    this->numberSlots = numberSlots;
}

unsigned int Parameters::GetNumberCores() const {
    return numberCores;
}

void Parameters::SetNumberCores(unsigned int numberCores) {
    //assert(this->numberCores >= 1);
    this->numberCores = numberCores;
}

unsigned int Parameters::GetNumberRoutes() const {
    return numberRoutes;
}

void Parameters::SetNumberRoutes(unsigned int numberRoutes) {
    assert(numberRoutes > 0);
    
    this->numberRoutes = numberRoutes;
}

double Parameters::GetMaxSectionLegnth() const {
    return maxSectionLegnth;
}

void Parameters::SetMaxSectionLegnth(double maxSectionLegnth) {
    assert(maxSectionLegnth > 0.0);
    this->maxSectionLegnth = maxSectionLegnth*1E3;
}

unsigned int Parameters::GetNumberPolarizations() const {
    return numberPolarizations;
}

void Parameters::SetNumberPolarizations(unsigned int numberPolarizations) {
    assert(numberPolarizations > 0);
    this->numberPolarizations = numberPolarizations;
}

unsigned int Parameters::GetGuardBand() const {
    return guardBand;
}

void Parameters::SetGuardBand(unsigned int guardBand) {
    assert(guardBand < numberSlots);
    this->guardBand = guardBand;
}

double Parameters::GetBeta() const {
    return beta;
}

void Parameters::SetBeta(double beta) {
    assert(beta >= 0 && beta <=100);
    
    this->beta = beta;
}

unsigned int Parameters::GetNumberPDPPprotectionRoutes() const {
    return numberPDPPprotectionRoutes;
}

void Parameters::SetNumberPDPPprotectionRoutes(unsigned int numberPDPPprotectionRoutes) {
    assert(numberPDPPprotectionRoutes > 0);
    
    this->numberPDPPprotectionRoutes = numberPDPPprotectionRoutes;
}

