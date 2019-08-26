/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MinimumSlotsMinReg.cpp
 * Author: brunovacorreia
 * 
 * Created on August 25, 2019, 10:40 PM
 */

#include "../../../include/ResourceAllocation/RegeneratorAssignment/MinimumSlotsMinReg.h"

MinimumSlotsMinReg::MinimumSlotsMinReg(ResourceDeviceAlloc* resDevAlloc)
:RegeneratorAssignment(resDevAlloc) {
    
}

MinimumSlotsMinReg::~MinimumSlotsMinReg() {
    
}

void MinimumSlotsMinReg::ResourceAlloc(CallDevices* call) {
    resDevAlloc->RoutingOffVirtRegSpecAlloc(call);
}

void MinimumSlotsMinReg::OrderRegenerationOptions(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    std::vector<std::vector<unsigned>> vecNumReg = 
    resDevAlloc->resources->GetNumberRegSet(call);
    std::vector<std::vector<unsigned>> vecNumSlots = 
    resDevAlloc->resources->GetNumberSlotsSet(call);
    unsigned int posSize = 0;
    unsigned int auxRouteIndex = 0;
    unsigned int auxIndex = 0;
    unsigned int auxNumReg;
    unsigned int auxNumSlots;
    
    for(unsigned int a = 0; a < vecNumReg.size(); a++){
        posSize += vecNumReg.at(a).size();
    }
    
    while(vec.size() < posSize){
        auxNumReg = Def::Max_UnInt;
        auxNumSlots = Def::Max_UnInt;
        
        for(unsigned int a = 0; a < vecNumReg.size(); a++){
            for(unsigned int b = 0; b < vecNumReg.at(a).size(); b++){
                
                if(auxNumSlots >= vecNumSlots.at(a).at(b)){
                    if(auxNumSlots == vecNumSlots.at(a).at(b) && 
                    auxNumReg <= vecNumReg.at(a).at(b)){
                        continue;
                    }
                    else{
                        auxNumSlots = vecNumSlots.at(a).at(b);
                        auxNumReg = vecNumReg.at(a).at(b);
                        auxRouteIndex = a;
                        auxIndex = b;
                    }
                }
            }
        }
        vec.push_back(std::make_tuple(auxRouteIndex, auxIndex));
        vecNumReg.at(auxRouteIndex).at(auxIndex) = Def::Max_UnInt;
        vecNumSlots.at(auxRouteIndex).at(auxIndex) = Def::Max_UnInt;
    }
}
