/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_NumInterRoutesMSCL.cpp
 * Author: BrunoVinicius
 * 
 * Created on March 27, 2019, 3:51 PM
 */

#include "../../../include/Algorithms/GA/GA_NumInterRoutesMSCL.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Structure/Topology.h"
#include "../../../include/Data/Data.h"
#include "../../../include/Data/Options.h"
#include "../../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../../include/Algorithms/GA/IndividualNumRoutesMSCL.h"

GA_NumInterRoutesMSCL::GA_NumInterRoutesMSCL(SimulationType* simul)
:GA_MO(simul), numNodes(0), vecNumMaxInterRoutes(0) {
    assert(this->GetSimul()->GetOptions()->GetSpecAllOption() == SpecAllMSCL);
}

GA_NumInterRoutesMSCL::~GA_NumInterRoutesMSCL() {
    
}

void GA_NumInterRoutesMSCL::Initialize() {
    GA_MO::Initialize();
    this->SetNumNodes(this->GetSimul()->GetTopology()->GetNumNodes());
    this->vecNumMaxInterRoutes = this->GetSimul()->GetResourceAlloc()
        ->GetNumInterRoutesToCheck();
}

void GA_NumInterRoutesMSCL::InitializePopulation() {
    assert(this->paretoFronts.empty() && this->totalPopulation.empty());
    
    for(unsigned int a = 0; a < this->GetNumberIndividuals(); a++){
        this->paretoFronts.front().push_back(std::make_shared
             <IndividualNumRoutesMSCL>(this));
    }
}

void GA_NumInterRoutesMSCL::CreateNewPopulation() {
    this->totalPopulation.clear();
    this->Crossover();
    this->Mutation();
}

void GA_NumInterRoutesMSCL::ApplyIndividual(Individual* ind) {
    IndividualNumRoutesMSCL* auxInd = dynamic_cast<IndividualNumRoutesMSCL*>
                                      (ind);
    this->GetSimul()->GetResourceAlloc()
        ->SetNumInterRoutesToCheck(auxInd->GetGenes());
}

void GA_NumInterRoutesMSCL::SetIndParameters(Individual* ind) {
    double blockProb = this->GetSimul()->GetData()->GetPbReq();
    TIME simulTime = this->GetSimul()->GetData()->GetRealSimulTime();
    
    IndividualNumRoutesMSCL* auxInd = dynamic_cast<IndividualNumRoutesMSCL*>
                                      (ind);
    
    auxInd->SetBlockProb(blockProb);
    auxInd->SetSimulTime(simulTime);
}

unsigned int GA_NumInterRoutesMSCL::GetNumNodes() const {
    return numNodes;
}

void GA_NumInterRoutesMSCL::SetNumNodes(unsigned int numNodes) {
    assert(numNodes > 0);
    
    this->numNodes = numNodes;
}

unsigned int GA_NumInterRoutesMSCL::GetMaxNumInterRoutes(unsigned int orN, 
unsigned int deN, unsigned int pos) {
    return this->vecNumMaxInterRoutes.at(orN*this->numNodes+deN).at(pos);
}

unsigned int GA_NumInterRoutesMSCL::GetNumRoutes(unsigned int orN, 
unsigned int deN) {
    return this->vecNumMaxInterRoutes.at(orN*this->numNodes+deN).size();
}

unsigned int GA_NumInterRoutesMSCL::CreateGene(unsigned int orN, 
unsigned int deN, unsigned int pos) {
    this->numInterRoutesDistribution = 
    std::uniform_int_distribution<unsigned int>(0, this->GetMaxNumInterRoutes(
    orN, deN, pos));
    
    return this->numInterRoutesDistribution(this->random_generator);
}

void GA_NumInterRoutesMSCL::Crossover() {
    assert(this->totalPopulation.empty());
    assert(this->GetNumIndParetoFronts() == this->GetNumberIndividuals());
    std::shared_ptr<IndividualNumRoutesMSCL> auxInd1, auxInd2;
    std::vector<std::shared_ptr<IndividualNumRoutesMSCL>> auxVecTotalPop;
    
    //Put all individual in the Pareto fronts to an auxiliary vector.
    for(auto it1: this->paretoFronts){
        for(auto it2: it1){
            auxVecTotalPop.push_back(
            std::dynamic_pointer_cast<IndividualNumRoutesMSCL>(it2));
        }
    }
    
    //Shuffle the auxiliary vector.
    std::shuffle(auxVecTotalPop.begin(), auxVecTotalPop.end(), 
                 this->random_generator);
    
    while(!auxVecTotalPop.empty()){
        auxInd1 = auxVecTotalPop.back();
        auxVecTotalPop.pop_back();
        auxInd2 = auxVecTotalPop.back();
        auxVecTotalPop.pop_back();
        
        this->GenerateNewIndividuals(auxInd1.get(), auxInd2.get());
    }
}

void GA_NumInterRoutesMSCL::GenerateNewIndividuals(
const IndividualNumRoutesMSCL* const ind1, 
const IndividualNumRoutesMSCL* const ind2) {
    
    this->UniformCrossover(ind1, ind2);
}

void GA_NumInterRoutesMSCL::UniformCrossover(const IndividualNumRoutesMSCL* 
const ind1, const IndividualNumRoutesMSCL* const ind2) {
    std::shared_ptr<IndividualNumRoutesMSCL> newInd1 = 
    std::make_shared<IndividualNumRoutesMSCL>(this);
    std::shared_ptr<IndividualNumRoutesMSCL> newInd2 = 
    std::make_shared<IndividualNumRoutesMSCL>(this);
    double auxProb;
    unsigned int numRoutes = 0;
    
    for(unsigned int orN = 0; orN < this->numNodes; orN++){
        for(unsigned int deN = 0; deN < this->numNodes; deN++){
            numRoutes = this->vecNumMaxInterRoutes.at(orN*this->numNodes+deN)
                        .size();
            
            for(unsigned int pos = 0; pos < numRoutes; pos++){
                auxProb = this->GetProbDistribution();
                
                if(auxProb < this->GetProbCrossover()){
                    newInd1->SetGene(orN, deN, pos, ind1->GetGene(orN, deN, 
                                                                  pos));
                    newInd2->SetGene(orN, deN, pos, ind2->GetGene(orN, deN, 
                                                                  pos));
                }
                else{
                    newInd1->SetGene(orN, deN, pos, ind2->GetGene(orN, deN, 
                                                                  pos));
                    newInd2->SetGene(orN, deN, pos, ind1->GetGene(orN, deN, 
                                                                  pos));
                }
            }
        }
    }
    
    this->totalPopulation.push_back(newInd1);
    this->totalPopulation.push_back(newInd2);
}

void GA_NumInterRoutesMSCL::Mutation() {
    assert(this->totalPopulation.size() == this->GetNumberIndividuals());
    assert(this->GetNumIndParetoFronts() == this->GetNumberGenerations());
    unsigned int popSize = this->totalPopulation.size();
    std::shared_ptr<IndividualNumRoutesMSCL> newInd;
    
    //Create a copy of each individual generated by the crossover
    for(auto it: this->totalPopulation){
        newInd = std::make_shared<IndividualNumRoutesMSCL>(
                 std::dynamic_pointer_cast<IndividualNumRoutesMSCL>(it));
        this->totalPopulation.push_back(newInd);
    }
    
    //Apply the mutation only in the copies created.
    for(unsigned int a = popSize; a < (2*popSize); a++){
        this->MutateIndividual(dynamic_cast<IndividualNumRoutesMSCL*>
                              (this->totalPopulation.at(a).get()));
    }
    
    for(auto it: this->paretoFronts){
        this->totalPopulation.insert(this->totalPopulation.end(),
             it.begin(), it.end());
    }
    
    this->paretoFronts.clear();
}

void GA_NumInterRoutesMSCL::MutateIndividual(IndividualNumRoutesMSCL* 
const ind) {
    double auxProb;
    unsigned int numRoutes;
    
    for(unsigned int orN = 0; orN < this->numNodes; orN++){
        for(unsigned int deN = 0; deN < this->numNodes; deN++){
            numRoutes = this->vecNumMaxInterRoutes.at(orN*this->numNodes+deN)
                        .size();
            
            for(unsigned int pos = 0; pos < numRoutes; pos++){
                auxProb = this->GetProbDistribution();
                
                if(auxProb < this->GetProbMutation()){
                    ind->SetGene(orN, deN, pos, 
                                 this->CreateGene(orN, deN, pos));
                }
            }
        }
    }
}
