/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IndividualNumRoutesMSCL.cpp
 * Author: BrunoVinicius
 * 
 * Created on March 30, 2019, 1:48 PM
 */

#include "../../../include/Algorithms/GA/IndividualNumRoutesMSCL.h"
#include "../../../include/Algorithms/GA/GA_NumInterRoutesMSCL.h"

IndividualNumRoutesMSCL::IndividualNumRoutesMSCL(GA_NumInterRoutesMSCL* ga)
:Individual(ga), ga(ga), blockProb(0.0), simulTime(0.0), 
totalNumInterRoutes(0), genes(0) {
    const unsigned int numNodes = this->ga->GetNumNodes();
    unsigned int numRoutes;
    
    //Creation of genes
    this->genes.resize(numNodes*numNodes);
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            numRoutes = this->ga->GetNumRoutes(orN, deN);
            this->genes.at(orN*numNodes+deN).resize(numRoutes);
            
            for(unsigned int pos = 0; pos < numRoutes; pos++){
                this->genes.at(orN*numNodes+deN).at(pos) = 
                this->ga->CreateGene(orN, deN, pos);
            }
        }
    }
}

IndividualNumRoutesMSCL::IndividualNumRoutesMSCL(
const std::shared_ptr<const IndividualNumRoutesMSCL>& orig)
:Individual(orig), ga(orig->ga), blockProb(orig->blockProb), 
simulTime(orig->simulTime), totalNumInterRoutes(orig->totalNumInterRoutes), 
genes(orig->genes) {
    
}

IndividualNumRoutesMSCL::IndividualNumRoutesMSCL(const std::shared_ptr<const 
IndividualNumRoutesMSCL>& orig, double value):Individual(orig, value), 
ga(orig->ga), blockProb(value), simulTime(value), 
totalNumInterRoutes((unsigned) value), genes(orig->genes){

}

IndividualNumRoutesMSCL::~IndividualNumRoutesMSCL() {
    
}

double IndividualNumRoutesMSCL::GetBlockProb() const {
    return blockProb;
}

void IndividualNumRoutesMSCL::SetBlockProb(double blockProb) {
    assert(this->GetCount() < this->ga->GetMaxNumSimulation());
    assert(blockProb >= 0.0);
    
    if(this->GetCount() == 1)
        this->blockProb = blockProb;
    this->blockProb = (this->blockProb + blockProb) / 2.0;
}

TIME IndividualNumRoutesMSCL::GetSimulTime() const {
    return simulTime;
}

void IndividualNumRoutesMSCL::SetSimulTime(TIME simulTime) {
    assert(this->GetCount() < this->ga->GetMaxNumSimulation());
    assert(simulTime > 0.0);
    
    if(this->GetCount() == 1)
        this->simulTime = simulTime;
    this->simulTime = (this->simulTime + simulTime) / 2.0;
}

unsigned int IndividualNumRoutesMSCL::GetTotalNumInterRoutes() const {
    return totalNumInterRoutes;
}

void IndividualNumRoutesMSCL::SetTotalNumInterRoutes() {
    unsigned int total = 0;
    
    for(auto it1: genes)
        for(auto it2: it1)
            total += it2;
    
    totalNumInterRoutes = total;
}

std::vector<std::vector<unsigned int> > IndividualNumRoutesMSCL::GetGenes() 
const {
    return genes;
}

unsigned int IndividualNumRoutesMSCL::GetGene(unsigned int orN, 
unsigned int deN, unsigned int pos) const {
    return this->genes.at(orN*this->ga->GetNumNodes()+deN).at(pos);
}

void IndividualNumRoutesMSCL::SetGenes(std::vector<std::vector<unsigned int> > 
genes) {
    this->genes = genes;
}

void IndividualNumRoutesMSCL::SetGene(unsigned int orN, unsigned int deN, 
unsigned int pos, unsigned int gene) {
    this->genes.at(orN*this->ga->GetNumNodes()+deN).at(pos) = gene;
}

double IndividualNumRoutesMSCL::GetMainParameter() {
    return this->GetBlockProb();
}

double IndividualNumRoutesMSCL::GetSecondParameter() {
    return (double) this->GetTotalNumInterRoutes();
}
