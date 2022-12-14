/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PSO.cpp
 * Author: brunovacorreia
 * 
 * Created on September 10, 2019, 3:18 PM
 */

#include "../../../include/Algorithms/PSO/PSO.h"
#include "../../../include/Algorithms/PSO/ParticlePSO_SCRA.h"
#include "../../../include/Algorithms/PSO/ParticlePSO_SCRA2.h"
#include "../../../include/GeneralClasses/Def.h"
#include "../../../include/Data/InputOutput.h"
#include "../../../include/Data/Options.h"
#include "../../../include/SimulationType/SimulationType.h"

std::default_random_engine PSO::random_engine(Def::randomDevice());

std::ostream& operator<<(std::ostream& ostream, const PSO* pso) {
    ostream << "Iteration: " << pso->actualIteration << std::endl;
    ostream << "Best particle: " << pso->GetBestParticle() << std::endl;
    
    return ostream;
}

PSO::PSO(SimulationType* simul) 
:simul(simul), options(nullptr), loadPoint(0.0), actualIteration(0), 
numberIterations(0), numberParticles(0), numberDimensions(0), minPosition(0.0), 
maxPosition(0.0), minVelocity(0.0), maxVelocity(0.0), particles(0), 
bestParticles(0) {
    
}

PSO::~PSO() {

}

bool PSO::ParticleCompare::operator()(const std::shared_ptr<ParticlePSO>& partA,
const std::shared_ptr<ParticlePSO>& partB) const {
    
    return (partA->GetBestFitness() < partB->GetBestFitness());
}

void PSO::LoadFile() {
    unsigned int auxInt;
    double auxDouble;
    std::ifstream auxIfstream;
    
    simul->GetInputOutput()->LoadPSO(auxIfstream);
    
    auxIfstream >> auxDouble;
    this->SetLoadPoint(auxDouble);
    auxIfstream >> auxInt;
    this->SetNumberIterations(auxInt);
    auxIfstream >> auxInt;
    this->SetNumberParticles(auxInt);
    auxIfstream >> auxInt;
    this->SetNumberDimensions(auxInt);
    auxIfstream >> auxDouble;
    this->SetMinPosition(auxDouble);
    auxIfstream >> auxDouble;
    this->SetMaxPosition(auxDouble);
    auxIfstream >> auxDouble;
    this->SetMinVelocity(auxDouble);
    auxIfstream >> auxDouble;
    this->SetMaxVelocity(auxDouble);
    
    options = simul->GetOptions();
}

void PSO::RunIteration() {
    
    for(auto particle: particles){
        particle->CalcNewVelocity();
        particle->CalcNewPosition();
        particle->CalculateFitness();
        particle->UpdateBestPosition();
        particle->UpdateNeighborBestPosition();
    }
    
    std::sort(particles.begin(), particles.end(), ParticleCompare());
}

void PSO::Initialize() {
    constDistibution = std::uniform_real_distribution<double>(0,1);
    posDistribution = std::uniform_real_distribution<double>(minPosition,
                                                             maxPosition);
    velDistribution = std::uniform_real_distribution<double>(minVelocity,
                                                             maxVelocity);
}

void PSO::InitializePopulation() {
    
    this->CreateParticles();
    this->SetParticlesNeighbors();
    
    for(auto it: particles)
        it->CalculateFitness();
    
    for(auto it: particles)
        it->UpdateBestPosition();
    
    for(auto it: particles)
        it->UpdateNeighborBestPosition();
}

void PSO::SaveBestParticle() {
    std::shared_ptr<ParticlePSO> auxParticle;
    
    switch(options->GetRegAssOption()){
        case RegAssSCRA1:
        case RegAssSCRA2:
        case RegAssSCRA3:
        case RegAssSCRA4:
        case RegAssSCRA5:
            auxParticle = std::make_shared<ParticlePSO_SCRA>(
            std::dynamic_pointer_cast<ParticlePSO_SCRA>(particles.back()));
            break;
        case RegAssSCRA_Mod:
            auxParticle = std::make_shared<ParticlePSO_SCRA2>(
            std::dynamic_pointer_cast<ParticlePSO_SCRA2>(particles.back()));
            break;
        default:
            std::cerr << "Invalid SCRA option" << std::endl;
            std::abort();
    }
    bestParticles.push_back(auxParticle);
    auxParticle.reset();
}

const double PSO::GetC1() const {
    return c1;
}

const double PSO::GetC2() const {
    return c2;
}

const double PSO::GetConstrictionFactor() const {
    return constrictionFactor;
}

double PSO::GetRandomWeighting() {
    return constDistibution(random_engine);
}

double PSO::GetRandomPosition() {
    return posDistribution(random_engine);
}

double PSO::GetRandomVelocity() {
    return velDistribution(random_engine);
}

double PSO::GetLoadPoint() const {
    return loadPoint;
}

unsigned int PSO::GetNumberIterations() const {
    return numberIterations;
}

unsigned int PSO::GetNumberParticles() const {
    return numberParticles;
}

unsigned int PSO::GetNumberDimensions() const {
    return numberDimensions;
}

double PSO::GetMinPosition() const {
    return minPosition;
}

double PSO::GetMaxPosition() const {
    return maxPosition;
}

double PSO::GetMinVelocity() const {
    return minVelocity;
}

unsigned int PSO::GetActualIteration() const {
    return actualIteration;
}

void PSO::SetActualIteration(unsigned int actualIteration) {
    assert(actualIteration > 0 && actualIteration <= numberIterations);
    this->actualIteration = actualIteration;
}

SimulationType* PSO::GetSimul() const {
    return simul;
}

ParticlePSO* PSO::GetBestParticle() const {
    return bestParticles.at(actualIteration-1).get();
}

std::ostream& PSO::PrintParameters(std::ostream& ostream) const {
    ostream << "PSO PARAMETERS" << std::endl;
    ostream << "Network load(erlang): " << this->GetLoadPoint() << std::endl;
    ostream << "Number of iterations: " << this->GetNumberIterations() 
            << std::endl;
    ostream << "Number of particles: " << this->GetNumberParticles() 
            << std::endl;
    ostream << "Number of dimensions: " << this->GetNumberDimensions() 
            << std::endl;
    ostream << "Minimum position: " << this->GetMinPosition() << std::endl;
    ostream << "Maximum position: " << this->GetMaxPosition() << std::endl;
    ostream << "Minimum velocity: " << this->GetMinVelocity() << std::endl;
    ostream << "Maximum velocity: " << this->GetMaxVelocity() << std::endl
            << std::endl;
    
    return ostream;
}

void PSO::SetLoadPoint(double loadPoint) {
    assert(loadPoint > 0.0);
    this->loadPoint = loadPoint;
}

double PSO::GetMaxVelocity() const {
    return maxVelocity;
}

void PSO::SetNumberIterations(unsigned int numberIterations) {
    assert(numberIterations > 0);
    this->numberIterations = numberIterations;
}

void PSO::SetNumberParticles(unsigned int numberParticles) {
    assert(numberParticles > 0);
    this->numberParticles = numberParticles;
}

void PSO::SetNumberDimensions(unsigned int numberDimensions) {
    assert(numberDimensions > 0);
    this->numberDimensions = numberDimensions;
}

void PSO::SetMinPosition(double minPosition) {
    this->minPosition = minPosition;
}

void PSO::SetMaxPosition(double maxPosition) {
    this->maxPosition = maxPosition;
}

void PSO::SetMinVelocity(double minVelocity) {
    this->minVelocity = minVelocity;
}

void PSO::SetMaxVelocity(double maxVelocity) {
    this->maxVelocity = maxVelocity;
}

void PSO::CreateParticles() {
    assert(options->GetDevicesOption() == DevicesEnabled);
    assert(options->GetRegenerationOption() != RegenerationDisabled);
    RegAssignmentOption regAssOpt = options->GetRegAssOption();
    assert(regAssOpt == RegAssSCRA1 || regAssOpt == RegAssSCRA2 ||
           regAssOpt == RegAssSCRA3 || regAssOpt == RegAssSCRA4 ||
           regAssOpt == RegAssSCRA5 || regAssOpt == RegAssSCRA_Mod);
    Data* data = simul->GetData();
    ResourceAlloc* resAlloc = simul->GetResourceAlloc();
    
    while(particles.size() < numberParticles){
        
        switch(regAssOpt){
            case RegAssSCRA1:
            case RegAssSCRA2:
            case RegAssSCRA3:
            case RegAssSCRA4:
            case RegAssSCRA5:
                particles.push_back(std::make_shared<ParticlePSO_SCRA>(this, 
                data, resAlloc));
                break;
            case RegAssSCRA_Mod:
                particles.push_back(std::make_shared<ParticlePSO_SCRA2>(this, 
                data, resAlloc));
                break;
            default:
                std::cerr << "Invalid SCRA option" << std::endl;
                std::abort();
        }
    }
}

void PSO::SetParticlesNeighbors() {
    ParticlePSO* auxParticle;
    
    for(unsigned int ind = 0; ind < numberParticles; ind++){
        
        if((ind != 0) && (ind != numberParticles - 1)){
            auxParticle = particles.at(ind-1).get();
            particles.at(ind)->AddNeighborParticle(auxParticle);
            auxParticle = particles.at(ind+1).get();
            particles.at(ind)->AddNeighborParticle(auxParticle);
        }
        else if(ind == 0){
            auxParticle = particles.at(numberParticles-1).get();
            particles.at(ind)->AddNeighborParticle(auxParticle);
            auxParticle = particles.at(ind+1).get();
            particles.at(ind)->AddNeighborParticle(auxParticle);
        }
        else{
            auxParticle = particles.at(ind-1).get();
            particles.at(ind)->AddNeighborParticle(auxParticle);
            auxParticle = particles.at(0).get();
            particles.at(ind)->AddNeighborParticle(auxParticle);
        }
    }
}
