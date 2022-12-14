/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_SO.cpp
 * Author: BrunoVinicius
 * 
 * Created on March 25, 2019, 1:48 PM
 */

#include "../../../include/Algorithms/GA/GA_SO.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Data/Data.h"

bool GA_SO::IndividualCompare::operator()(
const std::shared_ptr<Individual>& indA, 
const std::shared_ptr<Individual>& indB) const {

    return (indA->GetFitness() < indB->GetFitness());
}

GA_SO::GA_SO(SimulationType* simul)
:GA(simul), numBestIndividuals(10), sumFitness(0.0), initialPopulation(0),
bestIndividuals(0), worstIndividuals(0), selectedPopulation(0),
totalPopulation(0) {
    
}

GA_SO::~GA_SO() {
    
}

void GA_SO::LoadFile() {
    GA::LoadFile();
    
    std::ifstream auxIfstream;
    unsigned int auxInt;
    this->GetSimul()->GetInputOutput()->LoadGA_SO(auxIfstream);
    
    auxIfstream >> auxInt;
    this->SetNumBestIndividuals(auxInt);
}

void GA_SO::Initialize() {
    GA::Initialize();
}

void GA_SO::KeepInitialPopulation() {
    
}

void GA_SO::SelectPopulation() {
    assert(this->selectedPopulation.empty());
    
    //Order all individuals, with best(smallest) Pb at the end of the vector.
    std::sort(this->totalPopulation.begin(), this->totalPopulation.end(),
              IndividualCompare());
    
    //Select numBestIndividuals best individuals (Block. Prob.)
    for(unsigned int a = 0; a < this->numBestIndividuals; a++){
        this->selectedPopulation.push_back(this->totalPopulation.back());
        this->totalPopulation.pop_back();
    }
    
    //Select randomly the others.
    std::shuffle(this->totalPopulation.begin(), 
                 this->totalPopulation.end(), this->random_generator);
    while(this->selectedPopulation.size() < this->GetNumberIndividuals()){
        this->selectedPopulation.push_back(this->totalPopulation.back());
        this->totalPopulation.pop_back();
    }
    
    //Sort the selected individuals, first worst last best.
    std::sort(this->selectedPopulation.begin(), this->selectedPopulation.end(), 
              IndividualCompare());
}

void GA_SO::SaveIndividuals() {

}

void GA_SO::SetSumFitness(double sumFitness) {
    assert(sumFitness > 0);
    this->sumFitness = sumFitness;
}

void GA_SO::SetSumFitnessSelectedPop() {
    double sum = 0.0;
    
    for(auto it: this->selectedPopulation){
        sum += (it->GetFitness());
    }
    
    this->SetSumFitness(sum);
}

void GA_SO::SetNumBestIndividuals(unsigned int numBestIndividuals) {
    assert(numBestIndividuals <= this->GetNumberIndividuals());
    this->numBestIndividuals = numBestIndividuals;
}

Individual* GA_SO::GetWorstIndividual() const {
    return this->worstIndividuals.at(this->GetActualGeneration()-1).get();
}

Individual* GA_SO::GetBestIndividual() const {
    return this->bestIndividuals.at(this->GetActualGeneration()-1).get();
}

Individual* GA_SO::GetIniIndividual(unsigned int index) {
    assert(index < this->initialPopulation.size());
    return this->initialPopulation.at(index).get();
}

Individual* GA_SO::RouletteIndividual() {
    double auxDouble = 0.0;
    unsigned int index = 0;
    
    this->fitnessDistribution = std::uniform_real_distribution<double>(0.0, 
                                this->sumFitness);
    double fitness = this->fitnessDistribution(this->random_generator);
    
    for(index = 0; index < this->selectedPopulation.size(); index++){
        auxDouble += this->selectedPopulation.at(index)->GetFitness();
        
        if(auxDouble >= fitness)
            break;
    }
    
    return this->selectedPopulation.at(index).get();
}

void GA_SO::RunSelectPop() {
    
    for(auto it: this->selectedPopulation){
        this->ApplyIndividual(it.get());
        this->GetSimul()->RunBase();
        this->SetIndParameters(it.get());
        this->GetSimul()->GetData()->Initialize();
    }
    this->SetSelectedPopFitness();
}

void GA_SO::RunTotalPop() {
    unsigned int maxNumSimulPerInd = this->GetMaxNumSimulation();
    
    for(auto it: this->totalPopulation){
        
        if(it->GetCount() < maxNumSimulPerInd){
            this->ApplyIndividual(it.get());
            this->GetSimul()->RunBase();
            this->SetIndParameters(it.get());
        }
        this->GetSimul()->GetData()->Initialize();
    }
    this->SetTotalPopFitness();
}

void GA_SO::CheckMinSimul() {
    unsigned int maxNumSimulPerInd = this->GetMaxNumSimulation();
    
    for(auto it: this->totalPopulation){
        
        while(it->GetCount() < maxNumSimulPerInd){
            this->ApplyIndividual(it.get());
            this->GetSimul()->RunBase();
            this->SetIndParameters(it.get());
            this->GetSimul()->GetData()->Initialize();
        }
    }
    this->SetTotalPopFitness();
}

void GA_SO::print(std::ostream& ostream) const {
    
    ostream << "Best individual: " << this->GetBestIndividual()
            << std::endl;
    ostream << "Worst individual: " << this->GetWorstIndividual()
            << std::endl;
}

std::ostream& GA_SO::printParameters(std::ostream& ostream) const {
    GA::printParameters(ostream);
    
    ostream << "GA SO PARAMETERS" << std::endl;
    ostream << "Number of best individuals to select: " 
            << this->numBestIndividuals << std::endl << std::endl;
    
    return ostream;
}
