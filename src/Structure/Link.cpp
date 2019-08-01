/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Link.cpp
 * Author: bruno
 * 
 * Created on August 4, 2018, 1:54 AM
 */

#include "../../include/Structure/Link.h"
#include "../../include/Structure/Topology.h"
#include "../../include/ResourceAllocation/Signal.h"
#include "../../include/GeneralClasses/General.h"

std::ostream& operator<<(std::ostream& ostream,
Link* link) {
    ostream << "Origim node: " << link->GetOrigimNode()
            << std::endl;
    ostream << "Destination node: " << link->GetDestinationNode()
            << std::endl;
    ostream  << "Length (km): " << link->GetLength() / 1E3
            << std::endl;
    ostream << "Number of sections: " << link->GetNumberSections()
            << std::endl;
    ostream << "Cost: " << link->GetCost()
            << std::endl;
    
    return ostream;
}

Link::Link(Topology* topPointer, NodeIndex origimNode, 
NodeIndex destinationNode, double length, 
unsigned int numberSections, unsigned int numberSlots) 
:topPointer(topPointer), origimNode(origimNode), 
destinationNode(destinationNode), length(length), 
numberSections(numberSections), cost(0.0), slotsStatus(0), linkWorking(true), 
utilization(0) {
    slotsStatus.assign(numberSlots, free);
}

Link::~Link() {
    
}

bool Link::operator==(const Link& right) const {
    if(right.origimNode == this->origimNode && 
    right.destinationNode == this->destinationNode)
        return true;
    
    return false;
}

void Link::Initialize() {
    //Make all slots status for free
    this->slotsStatus.assign(this->slotsStatus.size(), free);
}

NodeIndex Link::GetOrigimNode() const {
    return origimNode;
}

NodeIndex Link::GetDestinationNode() const {
    return destinationNode;
}

double Link::GetLength() const {
    return length;
}

unsigned int Link::GetNumberSections() const {
    return numberSections;
}

double Link::GetCost() const {
    return cost;
}

void Link::SetCost(double cost) {
    this->cost = cost;
}

bool Link::IsLinkWorking() const {
    return this->linkWorking;
}

void Link::SetLinkState(bool linkWorking) {
    this->linkWorking = linkWorking;
}

unsigned int Link::GetUtilization() const {
    return utilization;
}

void Link::SetUtilization(unsigned int utilization) {
    this->utilization = utilization;
}

void Link::CalcSignal(Signal* signal) const {
    double signalPower = signal->GetSignalPower();
    double asePower = signal->GetAsePower();
    double nonLinearPower = signal->GetNonLinearPower();
    
    double lSec = (double) this->GetLength()/this->GetNumberSections();
    double gLSec = 1.0/(General::dBToLinear(lSec*Signal::Alpha));
    double gAmp = 1.0/gLSec;
    
    for(unsigned int sec = 0; sec < this->numberSections; sec++){
        signalPower *= gLSec;
        asePower *= gLSec;
        nonLinearPower *= gLSec;
        nonLinearPower += 0.0;
        
        signalPower *= gAmp;
        asePower *= gAmp;
        asePower += Signal::pASE(Signal::fn, gAmp);
        nonLinearPower *= gAmp;
    }
    
    signal->SetSignalPower(signalPower);
    signal->SetAsePower(asePower);
    signal->SetNonLinearPower(nonLinearPower);
}

void Link::OccupySlot(const SlotIndex index) {
    assert(this->IsSlotFree(index));
    
    this->slotsStatus.at(index) = occupied;
}

void Link::ReleaseSlot(const SlotIndex index) {
    assert(this->IsSlotOccupied(index));
    
    this->slotsStatus.at(index) = free;
}

bool Link::IsSlotOccupied(const SlotIndex index) const {
    
    if(this->slotsStatus.at(index) == occupied)
        return true;
    return false;
}

bool Link::IsSlotFree(const SlotIndex index) const {
    
    if(this->slotsStatus.at(index) == free)
        return true;
    return false;
}

unsigned int Link::GetNumSlots() const {
    return slotsStatus.size();
}

unsigned int Link::GetNumberFreeSlots() const {
    unsigned int numFreeSlots = 0;
    unsigned int numSlots = this->topPointer->GetNumSlots();
    
    for(unsigned int a = 0; a < numSlots; a++){
        if(this->IsSlotFree(a))
            numFreeSlots++;
    }
    
    return numFreeSlots;
}

unsigned int Link::GetNumberOccupiedSlots() const {
    unsigned int numOccupiedSlots = 0;
    unsigned int numSlots = this->topPointer->GetNumSlots();
    numOccupiedSlots = numSlots - this->GetNumberFreeSlots();
    
    return numOccupiedSlots;
    
}

Topology* Link::GetTopology() const{
    return this->topPointer;
}
