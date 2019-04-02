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
:GA(simul), initialPopulation(0), firstParetoFronts(0), paretoFronts(0), 
totalPopulation(0) {
    
}

GA_MO::~GA_MO() {

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
    
    return indA->GetMainParameter() > indB->GetMainParameter();
}

void GA_MO::KeepInitialPopulation() {
    this->initialPopulation = this->paretoFronts.front();
}

void GA_MO::SelectPopulation() {
    assert(this->paretoFronts.empty());
    std::vector<std::shared_ptr<Individual>> vecDominated(0);
    std::vector<std::shared_ptr<Individual>> auxFront(0);
    std::shared_ptr<Individual> auxInd;
    unsigned int numInd = this->GetNumberIndividuals();
    std::vector<unsigned int> dominatedIndexes(0);
    
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
        while(!this->totalPopulation.empty() && 
        (this->GetNumIndParetoFronts() < numInd)){
            auxFront.push_back(this->totalPopulation.back());
            this->totalPopulation.pop_back();
        }
        
        //Add the Pareto front to the vector of fronts, in ascending order.
        std::make_heap(auxFront.begin(), auxFront.end(), OrderIndividuals());
        this->paretoFronts.push_back(auxFront);
        this->totalPopulation.insert(this->totalPopulation.end(),
        vecDominated.begin(), vecDominated.end());
        vecDominated.clear();
        auxFront.clear();
    }
    
    this->totalPopulation.clear();
    assert(this->GetNumIndParetoFronts() == numInd);
}

void GA_MO::SaveIndividuals() {
    this->firstParetoFronts.push_back(this->paretoFronts.front());
}

void GA_MO::RunSelectPop() {
    
    for(auto it: this->paretoFronts.front()){
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
    
    for(auto it: this->paretoFronts){
        numInd += it.size();
    }

    return numInd;
}
