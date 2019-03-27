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
    
    
}

unsigned int GA_NumInterRoutesMSCL::GetNumNodes() const {
    return numNodes;
}

void GA_NumInterRoutesMSCL::SetNumNodes(unsigned int numNodes) {
    assert(numNodes > 0);
    
    this->numNodes = numNodes;
}

void GA_NumInterRoutesMSCL::Crossover() {

}

void GA_NumInterRoutesMSCL::Mutation() {

}
