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
blockProb(0.0), betaAverage(0.0), genes(0) {
    const unsigned int numNodes = this->ga->GetNumNodes();
    
    //Creation of genes    
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

double IndividualPDPPBO::GetMainParameter() {
    return this->GetBlockProb();
}

double IndividualPDPPBO::GetSecondParameter() {

}

