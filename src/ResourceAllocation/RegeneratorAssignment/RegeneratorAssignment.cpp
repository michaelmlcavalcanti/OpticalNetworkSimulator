/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RegeneratorAssignment.cpp
 * Author: brunovacorreia
 * 
 * Created on August 25, 2019, 7:43 PM
 */

#include "../../../include/ResourceAllocation/RegeneratorAssignment/RegeneratorAssignment.h"

RegeneratorAssignment::RegeneratorAssignment(ResourceDeviceAlloc* resDevAlloc)
:resDevAlloc(resDevAlloc) {
    
}

RegeneratorAssignment::~RegeneratorAssignment() {

}

void RegeneratorAssignment::OrderRegenerationOptions(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    std::vector<std::vector<unsigned>> vecNumReg = 
    resDevAlloc->resources->GetNumberRegSet(call);
    std::vector<std::vector<double>> vecCosts(0);
    
    //Calculate the cost of each possible regeneration option.
    vecCosts.resize(vecNumReg.size());
    for(unsigned a = 0; a < vecNumReg.size(); a++){
        vecCosts.at(a).resize(vecNumReg.at(a).size());
        
        for(unsigned b = 0; b < vecNumReg.at(a).size(); b++){
            vecCosts.at(a).at(b) = this->CalcTupleCost(call, a, b);
        }
    }
    
    unsigned int auxRouteIndex = 0;
    unsigned int auxIndex = 0;
    double bestCost;
    bool foundOption = true;
    
    //Order the regeneration options, based on the cost.
    while(foundOption){
        foundOption = false;
        bestCost = Def::Max_Double;
        
        for(unsigned a = 0; a < vecCosts.size(); a++){
            for(unsigned b = 0; b < vecCosts.at(a).size(); b++){
                
                if(vecCosts.at(a).at(b) < bestCost){
                    bestCost = vecCosts.at(a).at(b);
                    auxRouteIndex = a;
                    auxIndex = b;
                    foundOption = true;
                }
            }
        }
        
        if(foundOption){
            vec.push_back(std::make_tuple(auxRouteIndex, auxIndex));
            vecCosts.at(auxRouteIndex).at(auxIndex) = Def::Max_Double;
        }
    }
}

bool RegeneratorAssignment::CreateRegOption(CallDevices* call, 
unsigned routeInd, std::vector<std::shared_ptr<Route> >& routes, 
std::vector<TypeModulation>& modulations) {
    std::cerr << "Invalid creation of regeneration option" << std::endl;
    std::abort();
}

double RegeneratorAssignment::CalcTupleCost(CallDevices* call, 
unsigned routeIndex, unsigned subRouteIndex) {
    std::cerr << "Invalid tuple calculation" << std::endl;
    std::abort();
}

unsigned int RegeneratorAssignment::GetN(CallDevices* call) {
    unsigned int numSlots;
    
    numSlots = resDevAlloc->modulation->GetNumberSlots(FirstModulation,
    call->GetBitRate());
    
    return numSlots;
}

bool RegeneratorAssignment::CheckSpectrumAndOSNR(const double bitRate, 
Route* route, TypeModulation modulation) {
    double osnrThreshold = resDevAlloc->modulation->GetOSNRQAM(modulation, 
                                                               bitRate);
    unsigned numSlots = resDevAlloc->modulation->GetNumberSlots(modulation,
                                                                bitRate);
    
    if(!resDevAlloc->topology->CheckOSNR(route, osnrThreshold))
        return false;
    
    if(!resDevAlloc->topology->CheckBlockSlotsDisp(route, numSlots))
        return false;
    
    return true;
}

bool RegeneratorAssignment::CheckSpectrumAndOSNR(const double bitRate, 
Route* route) {
    std::vector<TypeModulation> auxVecModulations = 
    Modulation::GetModulationFormats();
    TypeModulation auxMod;
    
    for(unsigned a = 0; a < auxVecModulations.size(); a++){
        auxMod = auxVecModulations.at(a);
        
        if(auxMod != InvalidModulation){
            if(this->CheckSpectrumAndOSNR(bitRate, route, auxMod))
                return true;
        }
    }
    
    return false;
}

TypeModulation RegeneratorAssignment::GetBestModulation(const double bitRate, 
Route* route) {
    std::vector<TypeModulation> auxVecModulations = 
    Modulation::GetModulationFormats();
    TypeModulation auxMod;
    double osnrThreshold;
    unsigned numSlots;
    
    for(unsigned a = 0; a < auxVecModulations.size(); a++){
        auxMod = auxVecModulations.at(a);
        
        if(auxMod != InvalidModulation){
            osnrThreshold = resDevAlloc->modulation->GetOSNRQAM(auxMod, 
                                                                bitRate);
            numSlots = resDevAlloc->modulation->GetNumberSlots(auxMod, bitRate);

            if(resDevAlloc->topology->CheckOSNR(route, osnrThreshold) && 
            resDevAlloc->topology->CheckBlockSlotsDisp(route, numSlots))
                return auxMod;
        }
    }
    
    return auxVecModulations.back();
}