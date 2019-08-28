/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationMultiNumDevices.h
 * Author: brunovacorreia
 *
 * Created on July 31, 2019, 2:37 PM
 */

#ifndef SIMULATIONMULTINUMDEVICES_H
#define SIMULATIONMULTINUMDEVICES_H

#include "SimulationType.h"
#include "../Structure/Devices/Device.h"

/**
 * @brief Type of simulation that increase the number of devices in the
 * network and evaluate the performance.
 */
class SimulationMultiNumDevices : public SimulationType {
public:
    /**
     * @brief Default constructor of the multi number of devices simulation
     * object.
     * @param simulIndex Index of this simulation.
     * @param typeSimulation Type of this simulation, used to create the base 
     * class.
     */
    SimulationMultiNumDevices(unsigned int simulIndex, 
    TypeSimulation typeSimulation);
    /**
     * @brief Default destructor of the multi number of devices simulation
     * object.
     */
    virtual ~SimulationMultiNumDevices();
    
    /**
     * @brief Function to run the simulation for a variable number of devices
     * in the topology.
     */
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
     * @param ostream Output stream.
     */
    std::ostream& Help(std::ostream& ostream) override;
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
    std::vector<unsigned> vecNumDevices;
};

#endif /* SIMULATIONMULTINUMDEVICES_H */

