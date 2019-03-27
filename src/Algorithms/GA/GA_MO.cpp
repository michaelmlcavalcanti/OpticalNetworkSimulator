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

#include <vector>

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

void GA_MO::KeepInitialPopulation() {
    this->initialPopulation = this->paretoFronts.front();
}

void GA_MO::SelectPopulation() {
    
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
