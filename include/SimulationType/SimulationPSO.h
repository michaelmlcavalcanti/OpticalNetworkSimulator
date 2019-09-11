/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationPSO.h
 * Author: brunovacorreia
 *
 * Created on September 16, 2019, 9:33 PM
 */

#ifndef SIMULATIONPSO_H
#define SIMULATIONPSO_H

#include "SimulationType.h"

class PSO;

class SimulationPSO : public SimulationType{
public:
    
    SimulationPSO(unsigned int simulIndex, TypeSimulation typeSimulation);
    
    virtual ~SimulationPSO();
    
    
    void Run() override;

    void RunBase() override;

    void Load() override;

    void LoadFile() override;

    void AdditionalSettings() override;

    void Print() override;

    void Save() override;

    std::ostream& Help(std::ostream& ostream) override;
    
    PSO* GetPSO();
private:
    
    void CreatePSO();
private:
    
    std::shared_ptr<PSO> psoAlgorithm;
};

#endif /* SIMULATIONPSO_H */

