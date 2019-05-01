/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IncNumRegSimulation.h
 * Author: BrunoVinicius
 *
 * Created on April 30, 2019, 4:47 PM
 */

#ifndef INCNUMREGSIMULATION_H
#define INCNUMREGSIMULATION_H

#include "SimulationType.h"


class IncNumRegSimulation : public SimulationType {
public:
    
    IncNumRegSimulation(unsigned int simulIndex, TypeSimulation typeSimulation);
    
    virtual ~IncNumRegSimulation();
    
    
    void Run() override;

    void RunBase() override;

    void Load() override;

    void LoadFile() override;

    void Print() override;

    void Save() override;

    void Help() override;
private:
    
    std::vector<unsigned> vecNumReg;
};

#endif /* INCNUMREGSIMULATION_H */

