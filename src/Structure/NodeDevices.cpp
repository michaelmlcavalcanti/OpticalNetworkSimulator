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
#include "../../include/Structure/Devices/SBVT_TSS_ML.h"

NodeDevices::NodeDevices(Topology* topPointer, NodeId nodeId)
:Node(topPointer, nodeId), numRegenerator(0), numFreeRegenerators(0),
regenerators(0), numTransponder(0), transponders(0) {
    
}

NodeDevices::~NodeDevices() {

}

void NodeDevices::Initialize() {
    Node::Initialize();
    
    if(numRegenerator > 0)
        this->InitializeRegnerators();
    if(numTransponder > 0)
        this->InitializeTransponders();
    
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

bool NodeDevices::isThereFreeBVT(unsigned int numSlots) const {
    //Need implementaion
    
    return true;
}

std::vector<std::shared_ptr<BVT> > NodeDevices::GetBVTs(unsigned int numSlots) 
const {
    std::vector<std::shared_ptr<BVT>> vecBVT(0);
    
    //Need implementaion
    
    return vecBVT;
}

void NodeDevices::InitializeRegnerators() {
    this->numFreeRegenerators = this->numRegenerator;
    
    if(this->numRegenerator > this->regenerators.size())
        this->CreateRegenerators();
    else if(this->numRegenerator < this->regenerators.size())
        this->DeleteRegenerators();
    
    for(auto it: this->regenerators){
        it->Initialize();
    }
}

void NodeDevices::InitializeTransponders() {
    
    if(numTransponder > transponders.size())
        this->CreateTransponders();
    else if(numTransponder < transponders.size())
        this->DeleteTransponders();
    
    for(auto it: transponders){
        it->Initialize();
    }
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

void NodeDevices::CreateTransponders() {
    
    while(numTransponder > transponders.size()){
        transponders.push_back(std::make_shared<SBVT_TSS_ML>(
        this->GetTopology(), this));
    }
}

void NodeDevices::DeleteTransponders() {
    
    while(numTransponder < transponders.size()){
        transponders.back().reset();
        transponders.pop_back();
    }
}
