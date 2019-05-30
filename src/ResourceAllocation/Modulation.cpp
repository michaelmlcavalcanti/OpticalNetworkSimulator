/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Modulation.cpp
 * Author: BrunoVinicius
 * 
 * Created on November 28, 2018, 1:56 PM
 */

#include "../../include/ResourceAllocation/Modulation.h"
#include "../../include/Calls/Call.h"
#include "../../include/Calls/CallDevices.h"
#include "../../include/GeneralClasses/General.h"
#include "../../include/Data/Options.h"
#include "../../include/ResourceAllocation/ResourceAlloc.h"

const boost::unordered_map<TypeModulation, unsigned int> 
Modulation::mapModulation = boost::assign::map_list_of
    (InvalidModulation, 0)
    (QAM_4, 2)
    (QAM_8, 3)
    (QAM_16, 4)
    (QAM_32, 5)
    (QAM_64, 6);

Modulation::Modulation(ResourceAlloc* resourAlloc, double slotBand)
:resourAlloc(resourAlloc), slotBandwidth(slotBand), BER(1E-3), polarization(1),
rollOff(0.0) {
    
}

Modulation::~Modulation() {
    
}

void Modulation::SetModulationParam(Call* call) {
    double bandwidth, OSNRth;
    unsigned int numSlots;
    unsigned int modValue = this->mapModulation.at(call->GetModulation());
    double bitRate = call->GetBitRate();
    
    bandwidth = this->BandwidthQAM(modValue, bitRate);
    call->SetBandwidth(bandwidth);
    
    if(this->isEON())
        numSlots = std::ceil(bandwidth/this->slotBandwidth);
    else
        numSlots = 1;
    call->SetNumberSlots(numSlots);
    call->SetTotalNumSlots();
    OSNRth = this->GetOSNRQAM(modValue, bitRate);
    call->SetOsnrTh(OSNRth);
}

void Modulation::SetModulationParam(CallDevices* call) {
    std::vector<Call*> calls = call->GetTranspSegments();
    
    for(auto it: calls)
        this->SetModulationParam(it);
    call->SetTotalNumSlots();
}

double Modulation::BandwidthQAM(unsigned int M, double Rbps) {
    assert(M >= 2  && M <= 6);
    
    return ((1.0+this->rollOff)*Rbps)/(this->polarization*M);
}

double Modulation::GetOSNRQAM(unsigned int M, double Rbps) {
    double snrb = this->GetsnrbQAM(M);
    
    return General::LinearTodB((Rbps*snrb)/
                               (this->polarization*this->slotBandwidth));
}

double Modulation::GetsnrbQAM(unsigned int M) {
    double SNRb = this->GetSNRbQAM(M);
    
    return General::dBToLinear(SNRb);
}

double Modulation::GetSNRbQAM(unsigned int M) {
    
    if(this->BER == 1E-3){
        switch(M){
            case 2:
                return 6.79;
                break;
            case 3:
                return 9.03;
                break;
            case 4:
                return 10.52;
                break;
            case 5:
                return 12.57;
                break;
            case 6:
                return 14.77;
                break;
            default:
                std::cerr << "Invalid modulation format" << std::endl;
                std::abort();
        }
    }
    else if(this->BER == 3.8E-3){
        switch(M){
            case 2:
                return 5.52;
                break;
            case 3:
                return 7.83;
                break;
            case 4:
                return 9.17;
                break;
            case 5:
                return 11.23;
                break;
            case 6:
                return 13.34;
                break;
            default:
                std::cerr << "Invalid modulation format" << std::endl;
                std::abort();
        }
    }
    
    std::cerr << "Problem in modulation" << std::endl;
    std::abort();
}

bool Modulation::isEON() const {
    if(this->slotBandwidth <= 12.5*1E9)
        return true;
    
    return false;
}

std::vector<unsigned int> Modulation::GetPossibleSlots(std::vector<double> 
                                                       traffic) {
    std::vector<unsigned int> posSlots(0);
    ResourceAllocOption option = this->resourAlloc->GetResourAllocOption();
    
    if(!this->isEON()){
        posSlots.push_back(1);
        return posSlots;
    }
    
    switch(option){
        case ResourAllocRSA:
            posSlots = this->GetPossibleSlotsFixedMod(traffic);
            break;
        case ResourAllocRMSA:
            posSlots = this->GetPossibleSlotsVariableMod(traffic);
            break;
        default:
            std::cerr << "Invalid resource allocation option" << std::endl;
    }
    
    return posSlots;
}

std::vector<unsigned int> Modulation::GetPossibleSlotsFixedMod(
std::vector<double>& traffic) {
    std::vector<unsigned int> posSlots(0);
    double bitRate, bandwidth;
    unsigned int numSlots;
    TypeModulation modType = FixedModulation;
    
    for(unsigned a = 0; a < traffic.size(); a++){
        bitRate = traffic.at(a);
        bandwidth = this->BandwidthQAM(modType, bitRate);
        numSlots = std::ceil(bandwidth/this->slotBandwidth);
        
        if( std::find(posSlots.begin(), posSlots.end(), numSlots) == 
        posSlots.end() ){
            posSlots.push_back(numSlots);
        }
        
    }
    
    return posSlots;
}

std::vector<unsigned int> Modulation::GetPossibleSlotsVariableMod(
std::vector<double>& traffic) {
    std::vector<unsigned int> posSlots(0);
    double bitRate, bandwidth;
    unsigned int numSlots;
    TypeModulation mod;
    
    for(mod = FirstModulation; mod <= LastModulation; 
    mod = TypeModulation(mod+1)){
        for(unsigned a = 0; a < traffic.size(); a++){
            bitRate = traffic.at(a);
            bandwidth = this->BandwidthQAM(mod, bitRate);
            numSlots = std::ceil(bandwidth/this->slotBandwidth);
            
            if( std::find(posSlots.begin(), posSlots.end(), numSlots) == 
            posSlots.end() ){
                posSlots.push_back(numSlots);
            }
        }
    }
    
    return posSlots;
}
