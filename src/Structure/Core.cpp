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
    
    if(slotsStatus.at(sPosition) == occupied || slotsStatus.at(sPosition) == reserved || slotsStatus.at(sPosition) == reutilized)
        return true;
    return false;
}

bool Core::IsSlotOccupied(SlotIndex sPosition, SlotState type){

    if(type == occupied || type == reserved){
        if(slotsStatus.at(sPosition) == free){
            return false;
        }else{
            return true;
        }
    }else if(type == reutilized){
        if(slotsStatus.at(sPosition) == free || slotsStatus.at(sPosition) == reserved){
            return false;
        }else{
            return true;
        }
    }else{
        return false;
    }
}

bool Core::IsSlotFree(SlotIndex sPosition) {
    return !this->IsSlotOccupied(sPosition);
}

bool Core::IsSlotFree(SlotIndex sPosition, SlotState state) {
    return !this->IsSlotOccupied(sPosition, state);
}

void Core::OccupySlot(SlotIndex sPosition){
    assert(sPosition < slotsStatus.size() && slotsStatus.at(sPosition) == free);
    
    slotsStatus.at(sPosition) = occupied;
}

void Core::OccupySlot(SlotIndex sPosition, SlotState state){
    if(state == occupied){
        assert(sPosition < slotsStatus.size() && slotsStatus.at(sPosition) == free);
        slotsStatus.at(sPosition) = occupied;
    }else if(state == reserved){
        assert(sPosition < slotsStatus.size() && slotsStatus.at(sPosition) == free);
        slotsStatus.at(sPosition) = reserved;
    }else if(state == reutilized){
        if(slotsStatus.at(sPosition) == free){
            assert(sPosition < slotsStatus.size() && slotsStatus.at(sPosition) == free);
            slotsStatus.at(sPosition) = occupied;
        }else if(slotsStatus.at(sPosition) == reserved){
            assert(sPosition < slotsStatus.size() && slotsStatus.at(sPosition) == reserved);
            slotsStatus.at(sPosition) = reutilized;
        }
    }
}

void Core::ReleaseSlot(SlotIndex sPosition){

    if(slotsStatus.at(sPosition) == occupied){
        assert(sPosition < slotsStatus.size() && slotsStatus.at(sPosition) == occupied);
        slotsStatus.at(sPosition) = free;
    }else if(slotsStatus.at(sPosition) == reserved){
        assert(sPosition < slotsStatus.size() && slotsStatus.at(sPosition) == reserved);
        slotsStatus.at(sPosition) = free;
    }else if(slotsStatus.at(sPosition) == reutilized){
        assert(sPosition < slotsStatus.size() && slotsStatus.at(sPosition) == reutilized);
        slotsStatus.at(sPosition) = reserved;
    }
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
