/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_RsaOrder.cpp
 * Author: BrunoVinicius
 * 
 * Created on February 11, 2019, 11:02 AM
 */

#include "../../../include/Algorithms/GA/GA_RsaOrder.h"
#include "../../../include/Structure/Topology.h"
#include "../../../include/Algorithms/GA/IndividualBool.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../../include/Data/Data.h"
#include "../../../include/GeneralClasses/Def.h"

GA_RsaOrder::GA_RsaOrder(SimulationType* simul)
:GA_SO(simul), numNodes(0) {
    
}

GA_RsaOrder::~GA_RsaOrder() {
    
}

void GA_RsaOrder::Initialize() {
    GA_SO::Initialize();
    this->SetNumNodes(this->GetSimul()->GetTopology()->GetNumNodes());
    this->boolDistribution = std::uniform_int_distribution<int>(r_sa, sa_r);
}

void GA_RsaOrder::InitializePopulation() {
    assert(this->selectedPopulation.empty() && this->totalPopulation.empty());
    
    for(unsigned int a = 0; a < this->GetNumberIndividuals(); a++){
        
        if(a == 0){
            this->selectedPopulation.push_back(std::make_shared
                                               <IndividualBool>(this, r_sa));
        }
        else if(a == 1){
            this->selectedPopulation.push_back(std::make_shared
                                               <IndividualBool>(this, sa_r));
        }
        else{
            this->selectedPopulation.push_back(std::make_shared
                                               <IndividualBool>(this));
        }
    }
}

void GA_RsaOrder::CreateNewPopulation() {
    this->totalPopulation.clear();
    this->SetSumFitnessSelectedPop();
    this->Crossover();
    this->Mutation();
}

void GA_RsaOrder::KeepInitialPopulation() {
    GA_SO::KeepInitialPopulation();
    std::shared_ptr<IndividualBool> auxInd;
    
    for(auto it: this->selectedPopulation){
        auxInd = std::dynamic_pointer_cast<IndividualBool>(it);
        this->initialPopulation.push_back(
        std::make_shared<IndividualBool>(auxInd));
    }
    std::sort(this->initialPopulation.begin(), this->initialPopulation.end(),
              IndividualCompare());
}

void GA_RsaOrder::SaveIndividuals() {
    GA_SO::SaveIndividuals();
    std::shared_ptr<IndividualBool> auxInd;
    
    auxInd = std::dynamic_pointer_cast<IndividualBool>
             (this->selectedPopulation.back());
    this->bestIndividuals.push_back(std::make_shared<IndividualBool>(auxInd));
    auxInd = std::dynamic_pointer_cast<IndividualBool>
             (this->selectedPopulation.front());
    this->worstIndividuals.push_back(std::make_shared<IndividualBool>(auxInd));
}

unsigned int GA_RsaOrder::GetNumNodes() const {
    return numNodes;
}

void GA_RsaOrder::SetNumNodes(unsigned int numNodes) {
    assert(numNodes > 0);
    
    this->numNodes = numNodes;
}

ResAllocOrder GA_RsaOrder::GetBoolDistribution() {
    return (ResAllocOrder) boolDistribution(this->random_generator);
}

void GA_RsaOrder::ApplyIndividual(Individual* ind) {
    IndividualBool* indBool = dynamic_cast<IndividualBool*>(ind);
    this->GetSimul()->GetResourceAlloc()
        ->SetResourceAllocOrder(indBool->GetGenes());
}

void GA_RsaOrder::SetIndParameters(Individual* ind) {
    double blockProb = this->GetSimul()->GetData()->GetReqBP();
    IndividualBool* indBool = dynamic_cast<IndividualBool*>(ind);
    
    indBool->SetBlockProb(blockProb);
}

void GA_RsaOrder::SetSelectedPopFitness() {
    double bestPb = Def::Max_Double;
    IndividualBool* auxInd;
    
    //Find the best blocking probability of the selectedPop container
    for(auto it: this->selectedPopulation){
        auxInd = dynamic_cast<IndividualBool*>(it.get());
        
        if(bestPb > auxInd->GetBlockProb())
            bestPb = auxInd->GetBlockProb();
    }
    
    for(auto it: this->selectedPopulation){
        auxInd = dynamic_cast<IndividualBool*>(it.get());
        auxInd->SetFitness(1.0 / (bestPb + auxInd->GetBlockProb()));
    }
}

void GA_RsaOrder::SetTotalPopFitness() {
    double bestPb = Def::Max_Double;
    IndividualBool* auxInd;
    
    //Find the best blocking probability of the selectedPop container
    for(auto it: this->totalPopulation){
        auxInd = dynamic_cast<IndividualBool*>(it.get());
        
        if(bestPb > auxInd->GetBlockProb())
            bestPb = auxInd->GetBlockProb();
    }
    
    for(auto it: this->totalPopulation){
        auxInd = dynamic_cast<IndividualBool*>(it.get());
        auxInd->SetFitness(1.0 / (bestPb + auxInd->GetBlockProb()));
    }
}

void GA_RsaOrder::Crossover() {
    assert(this->selectedPopulation.size() == this->GetNumberIndividuals());
    assert(this->totalPopulation.empty());
    IndividualBool *auxInd1, *auxInd2;
    
    while(this->totalPopulation.size() < this->GetNumberIndividuals()){
        auxInd1 = dynamic_cast<IndividualBool*>(this->RouletteIndividual());
        do{
            auxInd2 = dynamic_cast<IndividualBool*>
                      (this->RouletteIndividual());
        }while(auxInd1 == auxInd2);
        
        this->GenerateNewIndividuals(auxInd1, auxInd2);
    }
}

void GA_RsaOrder::GenerateNewIndividuals(const IndividualBool* const ind1, 
                                         const IndividualBool* const ind2) {
    //Put condition for choose  the crossover
    this->UniformCrossover(ind1, ind2);
}

void GA_RsaOrder::UniformCrossover(const IndividualBool* const ind1, 
                                   const IndividualBool* const ind2) {
    std::shared_ptr<IndividualBool> newInd1 = 
    std::make_shared<IndividualBool>(this);
    std::shared_ptr<IndividualBool> newInd2 = 
    std::make_shared<IndividualBool>(this);
    double auxProb;
    
    for(unsigned int a = 0; a < this->numNodes; a++){
        for(unsigned int b = 0; b < this->numNodes; b++){
            auxProb = this->GetProbDistribution();
            
            if(auxProb < this->GetProbCrossover()){
                newInd1->SetGene(a, b, ind1->GetGene(a, b));
                newInd2->SetGene(a, b, ind2->GetGene(a, b));
            }
            else{
                newInd1->SetGene(a, b, ind2->GetGene(a, b));
                newInd2->SetGene(a, b, ind1->GetGene(a, b));
            }
        }
    }
    
    this->totalPopulation.push_back(newInd1);
    this->totalPopulation.push_back(newInd2);
}

void GA_RsaOrder::Mutation() {
    assert(this->totalPopulation.size() == this->GetNumberIndividuals());
    
    for(auto it: this->totalPopulation){
        this->MutateIndividual(dynamic_cast<IndividualBool*>(it.get()));
    }
    
    this->totalPopulation.insert(this->totalPopulation.end(), 
          this->selectedPopulation.begin(), this->selectedPopulation.end());
    
    this->selectedPopulation.clear();
}

void GA_RsaOrder::MutateIndividual(IndividualBool* const ind) {
    double auxProb;
    
    for(unsigned int a = 0; a < this->numNodes; a++){
        for(unsigned int b = 0; b < this->numNodes; b++){
            auxProb = this->GetProbDistribution();
            
            if(auxProb < this->GetProbMutation())
                ind->SetGene(a, b, !ind->GetGene(a, b));
        }
    }
}
