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

Core::Core(CoreIndex cId, unsigned int nSlots)
:coreId(cId), slotsStatus(0) {
    this->slotsStatus.assign(nSlots, free);
}

Core::~Core() {

}

void Core::Initialize() {
    this->slotsStatus.assign(this->slotsStatus.size(), free);
}

CoreIndex Core::GetCoreId(){
    return this->coreId;
}

bool Core::IsSlotOccupied(SlotIndex sPosition){
    if(this->slotsStatus.at(sPosition) == occupied)
        return true;
    return false;
}

bool Core::IsSlotFree(SlotIndex sPosition) {
    return !this->IsSlotOccupied(sPosition);
}

void Core::OccupySlot(SlotIndex sPosition){
    assert(sPosition < this->slotsStatus.size());
    assert(this->slotsStatus.at(sPosition) == free);
    
    this->slotsStatus.at(sPosition) = occupied;
}

void Core::ReleaseSlot(SlotIndex sPosition){
    assert(sPosition < this->slotsStatus.size());
    assert(this->slotsStatus.at(sPosition) == occupied);
    
    this->slotsStatus.at(sPosition) = free;
}
