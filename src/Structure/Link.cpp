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
#include "../../include/Structure/Core.h"

std::ostream& operator<<(std::ostream& ostream,
Link* link) {
    ostream << "Source node: " << link->GetOrigimNode()
            << std::endl;
    ostream << "Destination node: " << link->GetDestinationNode()
            << std::endl;
    ostream  << "Length (km): " << link->GetLength() / 1E3
            << std::endl;
    ostream << "Number of sections: " << link->GetNumberSections()
            << std::endl;
    ostream << "Cost: " << link->GetCost()
            << std::endl;
    ostream << "Number of cores: " << link->GetNumberCores()
            << std::endl;
    
    return ostream;
}

Link::Link(Topology* topPointer, NodeIndex origimNode, 
NodeIndex destinationNode, double length, unsigned int numberSections, 
unsigned int numberCores, unsigned int numberSlots)
:topPointer(topPointer), origimNode(origimNode), 
destinationNode(destinationNode), length(length), 
numberSections(numberSections), cost(0.0), cores(0), linkState(working), 
utilization(0), use(0) {
    
    for(unsigned int a = 0; a < numberCores; a++)
        cores.push_back(std::make_shared<Core>(this, a, numberSlots));
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
    
    for(auto it: cores){
        it->Initialize();
    }
    use = 0;
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
    return this->linkState;
}

void Link::SetLinkState(State linkState) {
    this->linkState = linkState;
}

unsigned int Link::GetUtilization() const {
    return utilization;
}

void Link::SetUtilization(unsigned int utilization) {
    this->utilization = utilization;
}

unsigned int Link::GetNumberCores() const {
    return cores.size();
}

unsigned int Link::GetUse() const {
    return use;
}

void Link::IncrementUse() {
    use += 1;
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

void Link::OccupySlot(const CoreIndex coreId, const SlotIndex slotId) {
    assert(coreId < cores.size());
    assert(cores.at(coreId)->IsSlotFree(slotId));
    
    cores.at(coreId)->OccupySlot(slotId);
}

void Link::OccupySlot(const CoreIndex coreId, const SlotIndex slotId, SlotState state) {
        assert(coreId < cores.size());

        assert(cores.at(coreId)->IsSlotFree(slotId, state));

        cores.at(coreId)->OccupySlot(slotId, state);
}

void Link::ReleaseSlot(const CoreIndex coreId, const SlotIndex slotId) {
    assert(coreId < cores.size());
    assert(cores.at(coreId)->IsSlotOccupied(slotId));
    
    cores.at(coreId)->ReleaseSlot(slotId);
}

bool Link::IsSlotOccupied(const SlotIndex index) const {

    return cores.front()->IsSlotOccupied(index);
}

bool Link::IsSlotOccupied1(const SlotIndex index, SlotState type) const {

    return cores.front()->IsSlotOccupied(index, type);
}

bool Link::IsSlotOccupied(const CoreIndex coreId, const SlotIndex slotId) {
    assert(coreId < cores.size());
    
    return cores.at(coreId)->IsSlotOccupied(slotId);
}

bool Link::IsSlotOccupied(const CoreIndex coreId, const SlotIndex slotId, SlotState type) {
    assert(coreId < cores.size());

    return cores.at(coreId)->IsSlotOccupied(slotId, type);
}

bool Link::IsSlotFree(const SlotIndex index) const {
    
    return cores.front()->IsSlotFree(index);
}

bool Link::IsSlotFree(const CoreIndex coreId, const SlotIndex slotId) {
    assert(coreId < cores.size());
    
    return cores.at(coreId)->IsSlotFree(slotId);
}

unsigned int Link::GetNumSlots() const {
    return cores.front()->GetNumSlots();
}

unsigned int Link::GetNumSlots(const CoreIndex coreId) const {
    assert(coreId < cores.size());
    
    return cores.at(coreId)->GetNumSlots();
}

unsigned int Link::GetNumberFreeSlots() const {
    unsigned int numFreeSlots = 0;
    unsigned int numSlots = cores.front()->GetNumSlots();
    
    for(unsigned int a = 0; a < numSlots; a++){
        if(cores.front()->IsSlotFree(a))
            numFreeSlots++;
    }
    
    return numFreeSlots;
}

unsigned int Link::GetNumberFreeSlots(const CoreIndex coreId) const {
    assert(coreId < cores.size());
    unsigned int numFreeSlots = 0;
    unsigned int numSlots = cores.at(coreId)->GetNumSlots();
    
    for(unsigned int a = 0; a < numSlots; a++){
        if(cores.at(coreId)->IsSlotFree(a))
            numFreeSlots++;
    }
    
    return numFreeSlots;
}

unsigned int Link::GetNumberOccupiedSlots() const {
    unsigned int numOccupiedSlots = 0;
    unsigned int numSlots = cores.front()->GetNumSlots();
    numOccupiedSlots = numSlots - this->GetNumberFreeSlots();
    
    return numOccupiedSlots;
}

unsigned int Link::GetNumberOccupiedSlots(const CoreIndex coreId) const {
    assert(coreId < cores.size());
    unsigned int numOccupiedSlots = 0;
    unsigned int numSlots = cores.at(coreId)->GetNumSlots();
    numOccupiedSlots = numSlots - this->GetNumberFreeSlots();
    
    return numOccupiedSlots;
}

std::vector<SlotState> Link::GetVecDisp() const {
    return cores.front()->GetSlotsStatus();
}

std::vector<SlotState> Link::GetVecDisp(const CoreIndex coreId) const {
    return cores.at(coreId)->GetSlotsStatus();
}

Topology* Link::GetTopology() const {
    return this->topPointer;
}
