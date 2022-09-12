/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Core.cpp
 * Author: paulofilho
 * 
 * Created on 4 de fevereiro de 2019, 15:17
 */

#include "../../include/Structure/Core.h"

std::ostream& operator<<(std::ostream& ostream,
Core* core) {
    ostream  << "Core Id: " << core->GetCoreId() << std::endl;
    
    return ostream;
}

Core::Core(Link* link, CoreIndex coreId, unsigned int numSlots)
:link(link), coreId(coreId), slotsStatus(0), coreState(working) {
    slotsStatus.assign(numSlots, free);
}

Core::~Core() {

}

void Core::Initialize() {
    slotsStatus.assign(slotsStatus.size(), free);
}

CoreIndex Core::GetCoreId(){
    return coreId;
}

bool Core::IsSlotOccupied(SlotIndex sPosition){
    
    if(slotsStatus.at(sPosition) == occupied)
        return true;
    return false;
}

bool Core::IsSlotOccupied(SlotIndex sPosition, SlotState type){
    bool statusSlot;
    if(type == free){
        if(slotsStatus.at(sPosition) == free){
            statusSlot = false;
        }else{
            statusSlot = true;
        }

    }else{
        if(slotsStatus.at(sPosition) == free || slotsStatus.at(sPosition) == reserved){
            statusSlot = false;
        }else{
            statusSlot = true;
        }
    }
    return statusSlot;
}

bool Core::IsSlotFree(SlotIndex sPosition) {
    return !this->IsSlotOccupied(sPosition);
}

void Core::OccupySlot(SlotIndex sPosition){
    assert(sPosition < slotsStatus.size() && slotsStatus.at(sPosition) == free);
    
    slotsStatus.at(sPosition) = occupied;
}

void Core::ReleaseSlot(SlotIndex sPosition){
    assert(sPosition < slotsStatus.size() && 
    slotsStatus.at(sPosition) == occupied);
    
    slotsStatus.at(sPosition) = free;
}

State Core::IsCoreWorking() {
    return coreState;
}

void Core::SetCoreState(State state) {
    coreState = state;
}

unsigned int Core::GetNumSlots() {
    return slotsStatus.size();
}

std::vector<SlotState> Core::GetSlotsStatus() const {
    return slotsStatus;
}
