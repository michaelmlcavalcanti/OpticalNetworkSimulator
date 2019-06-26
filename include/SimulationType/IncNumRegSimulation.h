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
#include "../Structure/Devices/Device.h"

/**
 * @brief Type of simulation that increase the number of regenerators in the
 * network and evaluate the performance.
 */
class IncNumRegSimulation : public SimulationType {
public:
    /**
     * @brief Default constructor of the IncNumRegSimulation object.
     * @param simulIndex Index of this simulation.
     * @param typeSimulation Type of this simulation, used to create the base 
     * class. For this class, the type is IncNumRegType.
     */
    IncNumRegSimulation(unsigned int simulIndex, TypeSimulation typeSimulation);
    /**
     * @brief Default destructor of the IncNumRegSimulation object.
     */
    virtual ~IncNumRegSimulation();
    
    
    void Run() override;
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
     * @brief Load parameters, options and such,
     * to the simulation from a .txt file.
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
private:
    /**
     * @brief Function to set the number of devices in the network. The device
     * type will be chosen...
     * @param numDevices Total number of devices in the network.
     */
    void SetNumberOfDevices(unsigned int numDevices);
private:
    /**
     * @brief Container with the total number of regenerators of each iteration.
     */
    std::vector<unsigned> vecNumReg;
};

#endif /* INCNUMREGSIMULATION_H */

