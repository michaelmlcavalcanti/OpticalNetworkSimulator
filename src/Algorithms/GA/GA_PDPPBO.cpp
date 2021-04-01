/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_PDPPBO.cpp
 * Author: Henrique Dinarte
 *
 * Created on April 1, 2021, 2:14 PM
 */

#include <condition_variable>

#include "../../../include/Algorithms/GA/GA_PDPPBO.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Structure/Topology.h"

GA_PDPPBO::GA_PDPPBO(SimulationType* simul) : GA_MO(simul), numNodes(0) {

}

GA_PDPPBO::~GA_PDPPBO() {

}

void GA_PDPPBO::Initialize() {
    GA_MO::Initialize();
    this->SetNumNodes(this->GetSimul()->GetTopology()->GetNumNodes());
}

unsigned int GA_PDPPBO::GetNumNodes() const {
    return numNodes;
}

void GA_PDPPBO::SetNumNodes(unsigned int numNodes) {
    assert(numNodes > 0);
    
    this->numNodes = numNodes;
}

