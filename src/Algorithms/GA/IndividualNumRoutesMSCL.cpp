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
:Individual(ga), ga(ga), blockProb(0.0), simulTime(0.0), genes(0) {
    const unsigned int numNodes = this->ga->GetNumNodes();
    
    
}

IndividualNumRoutesMSCL::IndividualNumRoutesMSCL(
const std::shared_ptr<const IndividualNumRoutesMSCL>& orig)
:Individual(orig), ga(orig->ga), blockProb(0.0), simulTime(0.0), 
genes(orig->genes) {
    
}

IndividualNumRoutesMSCL::~IndividualNumRoutesMSCL() {
    
}

double IndividualNumRoutesMSCL::GetBlockProb() const {
    return blockProb;
}

void IndividualNumRoutesMSCL::SetBlockProb(double blockProb) {
    assert(this->GetCount() < this->ga->GetMaxNumSimulation());
    assert(blockProb >= 0.0);
    this->SetCount(this->GetCount()+1);
    
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

double IndividualNumRoutesMSCL::GetMainParameter() {
    return this->GetBlockProb();
}

double IndividualNumRoutesMSCL::GetSecondParameter() {
    return (double) this->GetSimulTime();
}
