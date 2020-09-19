/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CallDevices.cpp
 * Author: BrunoVinicius
 * 
 * Created on April 30, 2019, 2:52 PM
 */

#include "../../include/Calls/CallDevices.h"
#include "../../include/ResourceAllocation/Route.h"

CallDevices::CallDevices(Node* orNode, Node* deNode, double bitRate, 
TIME deacTime):Call(orNode, deNode, bitRate, deacTime), transpSegments(0),
regenerators(0), useRegeneration(false), transponders(0) {
    
}

CallDevices::~CallDevices() {
    
}

void CallDevices::CreateTranspSegments(std::vector<std::shared_ptr<Route> > 
subroutes) {
    transpSegments.clear();
    regenerators.clear();
    std::shared_ptr<Call> auxCall;
    
    for(auto it: subroutes){
        auxCall = std::make_shared<Call>(it->GetOrNode(), 
        it->GetDeNode(), this->GetBitRate(), this->GetDeactivationTime());
        auxCall->SetRoute(it);
        transpSegments.push_back(auxCall);
    }
    subroutes.clear();
}

void CallDevices::SetTranspSegModulation(std::vector<TypeModulation> 
modulations) {
    assert(transpSegments.size() == modulations.size());
    
    for(unsigned a = 0; a < transpSegments.size(); a++){
        transpSegments.at(a)->SetModulation(modulations.at(a));
    }
}

std::vector<Call*> CallDevices::GetTranspSegments() {
    std::vector<Call*> calls(0);
    
    for(auto it: transpSegments)
        calls.push_back(it.get());
    
    return calls;
}

std::vector<std::shared_ptr<Call> > CallDevices::GetTranspSegmentsVec() {
    return transpSegments;
}

void CallDevices::SetTranspSegments(std::vector<std::shared_ptr<Call> > 
transpSeg) {
    transpSegments = transpSeg;
}

unsigned int CallDevices::GetTotalNumSlots() const {
    unsigned int totalNumSlots = 0;
    
    for(auto it: transpSegments){
        totalNumSlots += it->GetTotalNumSlots();
    }
    
    return totalNumSlots;
}

void CallDevices::SetTotalNumSlots() {
    unsigned int totalNumSlots = 0;
    
    if(!transpSegments.empty()){
        for(auto it: transpSegments){
            totalNumSlots += it->GetTotalNumSlots();
        }
        Call::SetTotalNumSlots(totalNumSlots);
    }
    else
        Call::SetTotalNumSlots();
}

std::vector<std::shared_ptr<Regenerator> > CallDevices::GetRegenerators() const{
    return regenerators;
}

void CallDevices::SetRegenerators(std::vector<std::shared_ptr<Regenerator> > 
regenerators) {
    assert(this->regenerators.empty());
    this->regenerators = regenerators;
}

void CallDevices::SetUseRegeneration() {
    assert(this->GetStatus() == Accepted);
    
    if(regenerators.empty())
        useRegeneration = false;
    else
        useRegeneration = true;
}

bool CallDevices::CheckUseRegeneration() const {
    
    return useRegeneration;
}

std::vector<std::shared_ptr<BVT>> CallDevices::GetTransponders() const {
    return transponders;
}

void CallDevices::SetTransponders(std::vector<std::shared_ptr<BVT>> 
transponders) {
    this->transponders = transponders;
}

void CallDevices::ClearTransponders() {
    transponders.clear();
}
