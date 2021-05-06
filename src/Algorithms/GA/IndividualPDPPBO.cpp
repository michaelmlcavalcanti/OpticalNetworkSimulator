/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IndividualPDPPBO.cpp
 * Author: Henrique Dinarte
 *
 * Created on April 1, 2021, 2:46 PM
 */

#include "../../../include/Algorithms/GA/IndividualPDPPBO.h"
#include "../../../include/Algorithms/GA/GA_PDPPBO.h"

IndividualPDPPBO::IndividualPDPPBO(GA_PDPPBO* ga) : Individual(ga), ga(ga), 
blockProb(0.0), betaAverage(0.0), genes(0), x(0) {
    unsigned int nodeIndex;
    const unsigned int numNodes = this->ga->GetNumNodes();
    const unsigned int numTraffic = ga->GetNumTraffic();
    std::vector<std::vector<double>> auxDist(0);

    //Creation of genes
    this->genes.resize(numNodes*numNodes);
    for(unsigned int orN = 0; orN < numNodes; orN++) {
        for (unsigned int deN = 0; deN < numNodes; deN++) {
            nodeIndex = orN*numNodes+deN;
            genes.at(nodeIndex).resize(numTraffic);
            for(unsigned int trIndex = 0; trIndex < numTraffic; trIndex++){
                genes.at(nodeIndex).at(trIndex) = ga->CreateGene(trIndex);
            }
        }
    }
}

IndividualPDPPBO::IndividualPDPPBO(
const std::shared_ptr<const IndividualPDPPBO>& orig) : Individual(orig), 
ga(orig->ga), blockProb(orig->blockProb), betaAverage(orig->betaAverage),
genes(orig->genes) {
    
}

IndividualPDPPBO::~IndividualPDPPBO() {

}

double IndividualPDPPBO::GetBlockProb() const {
    return blockProb;
}

void IndividualPDPPBO::SetBlockProb(double blockProb) {
    assert(this->GetCount() < this->ga->GetMaxNumSimulation());
    assert(blockProb >= 0.0);
    
    if(this->GetCount() == 1)
        this->blockProb = blockProb;
    this->blockProb = (this->blockProb + blockProb) / 2.0;
}

double IndividualPDPPBO::GetBetaAverage() const {
    return betaAverage;
}

void IndividualPDPPBO::SetBetaAverage(double betaAverage) {
    assert(this->GetCount() < this->ga->GetMaxNumSimulation());
    assert(betaAverage >= 0.0);
    
    if(this->GetCount() == 1)
        this->betaAverage = betaAverage;
    this->betaAverage = (this->betaAverage + betaAverage) / 2.0;
}

double IndividualPDPPBO::GetMainParameter() {
    return this->GetBlockProb();
}

double IndividualPDPPBO::GetSecondParameter() {
    return this->GetBetaAverage();
}

std::vector<std::vector<std::vector<double>>> IndividualPDPPBO::GetGenes() const {
    return this->genes;
}

std::vector<double> IndividualPDPPBO::GetGene(unsigned int orN, 
unsigned int deN, unsigned int traffIndex) const {
    return this->genes.at(orN * ga->GetNumNodes() + deN).at(traffIndex);
}

void IndividualPDPPBO::SetGenes(std::vector<std::vector<std::vector<double>>> genes) {
    this->genes = genes;
}

void IndividualPDPPBO::SetGene(unsigned int orN, unsigned int deN, unsigned int traffIndex, 
std::vector<double> gene) {
    this->genes.at(orN * ga->GetNumNodes() + deN).at(traffIndex) = gene;
}



