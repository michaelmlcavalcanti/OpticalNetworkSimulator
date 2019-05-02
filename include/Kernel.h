/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Kernel.h
 * Author: bruno
 *
 * Created on August 2, 2018, 3:42 PM
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <vector>
#include <memory>
#include <iostream>
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>

class SimulationType;

/**
 * @brief Enumerate the types of simulations the Kernel can create.
 */
enum TypeSimulation{
    InvalidSimulation,
    MultiLoadSimulationType,
    IncNumRegType,
    GaSimulationType,
    FirstSimulation = MultiLoadSimulationType,
    LastSimulation = GaSimulationType
};

/**
 * @brief The Kernel class is responsible for creating the
 * simulations, runs those simulations created and to
 * output the results of those simulations.
 */
class Kernel {
public:
    /**
     * @brief Standard constructor for a Kernel object.
     * @param numSimulations total number of 
     * simulations for this Kernel.
     */
    Kernel();
    /**
     * @brief Copy constructor for a Kernel object.
     * @param orig original Kernel object.
     */
    Kernel(const Kernel& orig);
    /**
     * @brief Default destructor of a Kernel object
     */
    virtual ~Kernel();
    
    /**
     * @brief Runs the Kernel functions.
     * Three steps are done, each one for all simulation.
     */
    void Run();
    /**
     * @brief Function to create all simulation of this kernel.
     */
    void CreateSimulations();
private:
    /**
     * @brief Function to apply the pre-simulation for a specified simulation
     * object.
     * @param simul Simulation object.
     */
    void Pre_Simulation(SimulationType* simul);
    /**
     * @brief Print and simulate a specified simulation object.
     * @param simul Simulation object.
     */
    void Simulation(SimulationType* simul);
    /**
     * @brief Save the data of the simulation.
     * @param simul Simulation object.
     */
    void Pos_Simulation(SimulationType* simul);
private:
    /**
     * @brief Total number of simulations in this kernel.
     */
    unsigned int numberSimulations;
    /**
     * @brief Vector with all pointers to SimulationType objects.
     */
    std::vector<std::shared_ptr<SimulationType>> simulations;
    
    static const boost::unordered_map<TypeSimulation, 
    std::string> mapSimulationType;
};

#endif /* KERNEL_H */

