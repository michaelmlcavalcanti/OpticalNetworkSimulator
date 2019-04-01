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

#include <cassert>

#include "../../../include/Algorithms/GA/GA_NumInterRoutesMSCL.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Structure/Topology.h"
#include "../../../include/Data/Data.h"
#include "../../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../../include/Algorithms/GA/IndividualNumRoutesMSCL.h"

GA_NumInterRoutesMSCL::GA_NumInterRoutesMSCL(SimulationType* simul)
:GA_MO(simul), numNodes(0), vecNumMaxInterRoutes(0) {
    
}

GA_NumInterRoutesMSCL::~GA_NumInterRoutesMSCL() {
    
}

void GA_NumInterRoutesMSCL::Initialize() {
    GA_MO::Initialize();
    this->SetNumNodes(this->GetSimul()->GetTopology()->GetNumNodes());
    
    //Criar funções para receber vetor de rotas interferentes, e criar o vetor
    //de número máximo de rotas a serem analisadas.
}

void GA_NumInterRoutesMSCL::InitializePopulation() {
    assert(this->paretoFronts.empty() && this->totalPopulation.empty());
    
    //Funções para criar os indivíduos da classe específica
}

void GA_NumInterRoutesMSCL::CreateNewPopulation() {
    this->totalPopulation.clear();
    this->Crossover();
    this->Mutation();
}

void GA_NumInterRoutesMSCL::ApplyIndividual(Individual* ind) {
    
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
    
    
}

void GA_NumInterRoutesMSCL::Mutation() {

}
