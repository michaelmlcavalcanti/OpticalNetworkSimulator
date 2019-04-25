/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Modulation.h
 * Author: BrunoVinicius
 *
 * Created on November 28, 2018, 1:56 PM
 */

#ifndef MODULATION_H
#define MODULATION_H

class ResourceAlloc;
class Call;

#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>

enum TypeModulation{
    InvalidModulation,
    QAM_4,
    QAM_8,
    QAM_16,
    QAM_32,
    QAM_64,
    FirstModulation = QAM_4,
    LastModulation = QAM_64,
    FixedModulation = QAM_8
};

class Modulation {

public:
    
    Modulation(ResourceAlloc* resourAlloc, double slotBand);
    
    virtual ~Modulation();
    
    
    void SetModulationParam(Call* call);
    
    double BandwidthQAM(unsigned int M, double Rbps);
    
    double GetOSNRQAM(unsigned int M, double Rbps);
    
    double GetSNRbQAM(unsigned int M);
    
    double GetsnrbQAM(unsigned int M);
    
    bool isEON();
    
    std::vector<unsigned int> GetPossibleSlots(std::vector<double> traffic);
private:
    
    std::vector<unsigned int> GetPossibleSlotsFixedMod(std::vector<double>&
                                                       traffic);
    
    std::vector<unsigned int> GetPossibleSlotsVariableMod(std::vector<double>&
                                                          traffic);
private:
    
    ResourceAlloc* resourAlloc;
    
    double slotBandwidth;
    
    const double BER;
    
    const unsigned int polarization;
    
    const double rollOff;
    
    static const boost::unordered_map<TypeModulation, 
                                      unsigned int> mapModulation;
};

#endif /* MODULATION_H */

