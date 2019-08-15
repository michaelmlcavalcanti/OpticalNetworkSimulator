/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_MO.cpp
 * Author: BrunoVinicius
 * 
 * Created on March 27, 2019, 3:28 PM
 */

#include "../../../include/Algorithms/GA/GA_MO.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Data/Data.h"

GA_MO::GA_MO(SimulationType* simul)
:GA(simul), initialPopulation(0), firstParetoFronts(0), saveStep(5), 
actualParetoFronts(0), totalPopulation(0) {
    
}

GA_MO::~GA_MO() {

}

void GA_MO::LoadFile() {
    GA::LoadFile();
    
    std::ifstream auxIfstream;
    unsigned int auxInt;
    this->GetSimul()->GetInputOutput()->LoadGA_SO(auxIfstream);
    
    auxIfstream >> auxInt;
    this->SetSaveStep(auxInt);
}

void GA_MO::Initialize() {
    GA::Initialize();
}

bool GA_MO::IndividualDominated::operator()(const std::shared_ptr<Individual>& 
indA, const std::shared_ptr<Individual>& indB) const {
    return ((indA->GetMainParameter() > indB->GetMainParameter() &&
            indA->GetSecondParameter() >= indB->GetSecondParameter()) ||
            (indA->GetMainParameter() >= indB->GetMainParameter() &&
            indA->GetSecondParameter() > indB->GetSecondParameter()));
}

bool GA_MO::OrderIndividuals::operator()(const std::shared_ptr<Individual>& 
indA, const std::shared_ptr<Individual>& indB) const {
    
    return indA->GetMainParameter() < indB->GetMainParameter();
}

void GA_MO::KeepInitialPopulation() {
    this->initialPopulation = this->actualParetoFronts.front();
    std::sort(this->initialPopulation.begin(), this->initialPopulation.end(), 
              OrderIndividuals());
}

void GA_MO::SelectPopulation() {
    assert(this->actualParetoFronts.empty());
    std::vector<std::shared_ptr<Individual>> vecDominated(0);
    std::vector<std::shared_ptr<Individual>> auxFront(0);
    std::shared_ptr<Individual> auxInd;
    std::vector<unsigned int> dominatedIndexes(0);
    unsigned int numInd = this->GetNumberIndividuals();
    
    while(this->GetNumIndParetoFronts() < numInd){
        
        //Return the indexes of the dominated solutions.
        for(unsigned int a = 0; a < this->totalPopulation.size(); a++){
            auxInd = this->totalPopulation.at(a);
            
            for(auto it: this->totalPopulation){
            
                if(IndividualDominated()(auxInd, it) && auxInd != it){
                    dominatedIndexes.push_back(a);
                    break;
                }
            }
        }
        
        //Put the dominated solutions in a auxiliary vector and keeps the 
        //non-dominated solutions in the totalPopulation vector.
        while(!dominatedIndexes.empty()){
            auxInd = this->totalPopulation.at(dominatedIndexes.back());
            vecDominated.push_back(auxInd);
            this->totalPopulation.erase(this->totalPopulation.begin() + 
            dominatedIndexes.back());
            dominatedIndexes.pop_back();
        }
        
        std::shuffle(this->totalPopulation.begin(), 
                     this->totalPopulation.end(), this->random_generator);
        
        //Fill an auxiliary vector with the right amount of 
        //non-dominated solutions.
        unsigned int added = 0;
        while((!this->totalPopulation.empty()) &&
        ((this->GetNumIndParetoFronts() + added) < numInd)){
            auxFront.push_back(this->totalPopulation.back());
            this->totalPopulation.pop_back();
            added++;
        }
        
        //Add the Pareto front to the vector of fronts, in ascending order.
        std::sort(auxFront.begin(), auxFront.end(), OrderIndividuals());
        this->actualParetoFronts.push_back(auxFront);
        this->totalPopulation.insert(this->totalPopulation.end(),
        vecDominated.begin(), vecDominated.end());
        vecDominated.clear();
        auxFront.clear();
    }
    
    this->totalPopulation.clear();
    assert(this->GetNumIndParetoFronts() == numInd);
}

void GA_MO::SaveIndividuals() {
    this->firstParetoFronts.push_back(this->actualParetoFronts.front());
}

void GA_MO::RunSelectPop() {
    
    for(auto it: this->actualParetoFronts.front()){
        this->ApplyIndividual(it.get());
        this->GetSimul()->RunBase();
        this->SetIndParameters(it.get());
        this->GetSimul()->GetData()->Initialize();
    }
}

void GA_MO::RunTotalPop() {
    unsigned int maxNumSimulPerInd = this->GetMaxNumSimulation();
    
    for(auto it: this->totalPopulation){
        
        if(it->GetCount() < maxNumSimulPerInd){
            this->ApplyIndividual(it.get());
            this->GetSimul()->RunBase();
            this->SetIndParameters(it.get());
        }
        this->GetSimul()->GetData()->Initialize();
    }
}

void GA_MO::CheckMinSimul() {
    unsigned int maxNumSimulPerInd = this->GetMaxNumSimulation();
    
    for(auto it: this->totalPopulation){
        
        while(it->GetCount() < maxNumSimulPerInd){
            this->ApplyIndividual(it.get());
            this->GetSimul()->RunBase();
            this->SetIndParameters(it.get());
            this->GetSimul()->GetData()->Initialize();
        }
    }
}

unsigned int GA_MO::GetNumIndParetoFronts() const {
    unsigned int numInd = 0;
    
    for(auto it: this->actualParetoFronts){
        numInd += it.size();
    }

    return numInd;
}

std::vector<Individual*> GA_MO::GetParetoFront() const {
    std::vector<Individual*> auxVecInd(0);
    
    for(auto it: this->firstParetoFronts.at(this->GetActualGeneration()-1)){
        auxVecInd.push_back(it.get());
    }
    
    return auxVecInd;
}

std::vector<Individual*> GA_MO::GetIniPopulation() const {
    std::vector<Individual*> auxVecInd(0);
    
    for(auto it: this->initialPopulation){
        auxVecInd.push_back(it.get());
    }
    
    return auxVecInd;
}

void GA_MO::SetSaveStep(unsigned int saveStep) {
    this->saveStep = saveStep;
}

unsigned int GA_MO::GetSaveStep() const {
    return saveStep;
}

void GA_MO::print(std::ostream& ostream) const {
    
    ostream << "Number of individuals of the first Pareto front: " 
            << this->firstParetoFronts.at(this->GetActualGeneration()-1).size() 
            << std::endl;
}

std::ostream& GA_MO::printParameters(std::ostream& ostream) const {
    GA::printParameters(ostream);
    
    ostream << "GA MO PARAMETERS" << std::endl;
    ostream << "Step of generation to save: " << this->GetSaveStep()
            << std::endl << std::endl;
    
    return ostream;
}
