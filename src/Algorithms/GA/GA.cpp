/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA.cpp
 * Author: BrunoVinicius
 * 
 * Created on February 27, 2019, 10:07 PM
 */

#include "../../../include/Algorithms/GA/GA.h"
#include "../../../include/GeneralClasses/Def.h"
#include "../../../include/SimulationType/SimulationType.h"

std::default_random_engine GA::random_generator(Def::randomDevice());

std::ostream& operator<<(std::ostream& ostream, const GA* ga) {
    ostream << "Generation: " << ga->actualGeneration << std::endl;
    ga->print(ostream);
    
    return ostream;
}

GA::GA(SimulationType* simul)
:simul(simul), numberIndividuals(20), numberGenerations(10),
probCrossover(0.5), probMutation(0.1), actualGeneration(0), 
maxNumSimulation(3) {
    
}

GA::~GA() {
    
}

void GA::LoadFile() {
    std::ifstream auxIfstream;
    unsigned int auxInt;
    double auxDouble;
    
    this->simul->GetInputOutput()->LoadGA(auxIfstream);
    
    auxIfstream >> auxInt;
    this->SetNumberIndividuals(auxInt);
    auxIfstream >> auxInt;
    this->SetNumberGenerations(auxInt);
    auxIfstream >> auxDouble;
    this->SetProbCrossover(auxDouble);
    auxIfstream >> auxDouble;
    this->SetProbMutation(auxDouble);
    auxIfstream >> auxInt;
    this->SetMaxNumSimulation(auxInt);
}

void GA::Initialize() {
    this->probDistribution = std::uniform_real_distribution<double>(0, 1);
}

const unsigned int GA::GetNumberGenerations() const {
    return numberGenerations;
}

void GA::SetNumberGenerations(unsigned int numberGenerations) {
    this->numberGenerations = numberGenerations;
}

const unsigned int GA::GetNumberIndividuals() const {
    return numberIndividuals;
}

void GA::SetNumberIndividuals(unsigned int numberIndividuals) {
    this->numberIndividuals = numberIndividuals;
}

const double GA::GetProbCrossover() const {
    return probCrossover;
}

void GA::SetProbCrossover(double probCrossover) {
    assert(probCrossover >= 0 && probCrossover <= 1.0);
    this->probCrossover = probCrossover;
}

const double GA::GetProbMutation() const {
    return probMutation;
}

void GA::SetProbMutation(double probMutation) {
    assert(probMutation >= 0 && probMutation <= 1.0);
    this->probMutation = probMutation;
}

const unsigned int GA::GetMaxNumSimulation() const {
    return maxNumSimulation;
}

void GA::SetMaxNumSimulation(unsigned int maxNumSimulation) {
    this->maxNumSimulation = maxNumSimulation;
}

double GA::GetProbDistribution() {
    return probDistribution(random_generator);
}

SimulationType* GA::GetSimul() const {
    return simul;
}

unsigned int GA::GetActualGeneration() const {
    return actualGeneration;
}

void GA::SetActualGeneration(unsigned int actualGeneration) {
    assert(actualGeneration >= 0);
    this->actualGeneration = actualGeneration;
}

std::ostream& GA::printParameters(std::ostream& ostream) const {
    ostream << "GA PARAMETERS" << std::endl;
    ostream << "Number of individuals: " << this->GetNumberIndividuals()
            << std::endl;
    ostream << "Number of generations: " << this->GetNumberGenerations()
            << std::endl;
    ostream << "Crossover probability: " << this->GetProbCrossover()
            << std::endl;
    ostream << "Mutation probability: " << this->GetProbMutation()
            << std::endl;
    ostream << "Number of maximum simulation: " << this->GetMaxNumSimulation()
            << std::endl;
    
    return ostream;
}
