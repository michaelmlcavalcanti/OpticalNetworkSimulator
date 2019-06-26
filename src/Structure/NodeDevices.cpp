/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NodeDevices.cpp
 * Author: BrunoVinicius
 * 
 * Created on April 29, 2019, 3:08 PM
 */

#include "../../include/Structure/NodeDevices.h"
#include "../../include/Structure/Devices/Regenerator.h"

NodeDevices::NodeDevices(Topology* topPointer, NodeId nodeId)
:Node(topPointer, nodeId), numRegenerator(0), numFreeRegenerators(0),
regenerators(0), transponders(0) {
    
}

NodeDevices::~NodeDevices() {

}

void NodeDevices::Initialize() {
    Node::Initialize();
    this->numFreeRegenerators = this->numRegenerator;
    
    if(this->numRegenerator > this->regenerators.size())
        this->CreateRegenerators();
    else if(this->numRegenerator < this->regenerators.size())
        this->DeleteRegenerators();
    
    for(auto it: this->regenerators){
        it->Initialize();
    }
}

unsigned int NodeDevices::GetNumRegenerator() const {
    return numRegenerator;
}

void NodeDevices::SetNumRegenerator(unsigned int numRegenerator) {
    this->numRegenerator = numRegenerator;
}

unsigned int NodeDevices::GetNumFreeRegenerators() const {
    return numFreeRegenerators;
}

unsigned int NodeDevices::GetNumOccRegenerators() const {
    return (numRegenerator - numFreeRegenerators);
}

void NodeDevices::IncreaseNumFreeRegenerators() {
    assert(numFreeRegenerators < numRegenerator);
    numFreeRegenerators++;
}

void NodeDevices::DecreaseNumFreeRegenerators() {
    assert(numFreeRegenerators > 0);
    numFreeRegenerators--;
}

bool NodeDevices::isThereFreeRegenerators(double bitRate) const {
    unsigned int numReg = (unsigned int) std::ceil(bitRate / 
                          Regenerator::GetTrafficSupported());
    
    if(numReg <= numFreeRegenerators)
        return true;
    return false;
}

std::vector<std::shared_ptr<Regenerator> > NodeDevices::
GetFreeRegenenerators(double bitRate) const {
    std::vector<std::shared_ptr<Regenerator>> vecReg(0);
    unsigned int numReg = (unsigned int) std::ceil(bitRate / 
                          Regenerator::GetTrafficSupported());
    
    for(auto it: regenerators){
        
        if(!it->IsActive())
            vecReg.push_back(it);
        
        if(vecReg.size() == numReg)
            break;
    }
    
    return vecReg;
}

unsigned int NodeDevices::GetNumTransponder() const {
    return numTransponder;
}

void NodeDevices::SetNumTransponder(unsigned int numTransponder) {
    this->numTransponder = numTransponder;
}

void NodeDevices::CreateRegenerators() {
    
    while(this->numRegenerator > this->regenerators.size()){
        this->regenerators.push_back(std::make_shared<Regenerator>(
        this->GetTopology(), this));
    }
}

void NodeDevices::DeleteRegenerators() {

    while(this->numRegenerator < this->regenerators.size()){
        this->regenerators.back().reset();
        this->regenerators.pop_back();
    }
}
