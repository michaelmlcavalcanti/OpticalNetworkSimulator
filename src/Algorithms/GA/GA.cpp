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

std::default_random_engine GA::random_generator(Def::randomDevice());

std::ostream& operator<<(std::ostream& ostream, const GA* ga) {
    ostream << "Generation: " << ga->actualGeneration << std::endl;
    ga->print(ostream);
    
    return ostream;
}

GA::GA(SimulationType* simul)
:simul(simul), numberIndividuals(50), numberGenerations(300),
probCrossover(0.5), probMutation(0.1), actualGeneration(0), 
maxNumSimulation(5) {
    
}

GA::~GA() {
    
}

void GA::Initialize() {
    this->probDistribution = std::uniform_real_distribution<double>(0, 1);
}

const unsigned int GA::GetNumberGenerations() const {
    return numberGenerations;
}

const unsigned int GA::GetNumberIndividuals() const {
    return numberIndividuals;
}

const double GA::GetProbCrossover() const {
    return probCrossover;
}

const double GA::GetProbMutation() const {
    return probMutation;
}

const unsigned int GA::GetMaxNumSimulation() const {
    return maxNumSimulation;
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
