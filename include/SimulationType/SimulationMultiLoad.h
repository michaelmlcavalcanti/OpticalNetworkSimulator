/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationMultiLoad.h
 * Author: brunovacorreia
 *
 * Created on July 31, 2019, 2:13 PM
 */

#ifndef SIMULATIONMULTILOAD_H
#define SIMULATIONMULTILOAD_H

#include "SimulationType.h"

/**
 * @brief Derived class from SimulationType.
 * This type of simulation runs for a specified number 
 * of load points, simulating for each one.
 */
class SimulationMultiLoad : public SimulationType {
public:
    /**
     * @brief Standard constructor for a multi-load simulation object.
     * @param simulIndex Index of this simulation.
     */
    SimulationMultiLoad(unsigned int simulIndex, TypeSimulation typeSimulation);
    /**
     * @brief Destructor of a multi-load simulation object.
     */
    virtual ~SimulationMultiLoad();
    
    /**
     * @brief Runs a multiple load simulation with specified parameters.
     */
    virtual void Run() override;
    /**
     * @brief Function used to call the Run function of the base class 
     * SimulationType.
     */
    void RunBase() override;
    /**
     * @brief Load parameters to the simulation from the terminal.
     */
    void Load() override;
    /**
     * @brief Load parameters, options and others
     * to the simulation, from a .txt file.
     */
    void LoadFile() override;
    /**
     * @brief Print this simulation in terminal.
     */
    void Print() override;
    /**
     * @brief Save the results in .txt files.
     */
    void Save() override;
    /**
     * @brief Prints a description of the simulation.
     */
    void Help() override;
};

#endif /* SIMULATIONMULTILOAD_H */

