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

/**
 * @brief Type of simulation that runs a generic Particle Swarm Optimization(PSO)
 * algorithm.
 */
class SimulationPSO : public SimulationType{
public:
    /**
     * @brief Default constructor for a PSO simulation.
     * @param simulIndex Simulation index.
     * @param typeSimulation Defines the type of simulation.
     */
    SimulationPSO(unsigned int simulIndex, TypeSimulation typeSimulation);
    /**
     * @brief Default destructor for a PSO simulation.
     */
    virtual ~SimulationPSO();
    
    /**
     * @brief Function to run all PSO iterations. This function also initialize
     * the population.
     */
    void Run() override;
    /**
     * @brief Function used to call the Run function of the base class 
     * SimulationType.
     */
    void RunBase() override;
    /**
     * @brief Function to load the simulation parameters from the terminal and, 
     * create the PSO and load its parameters.
     */
    void Load() override;
    /**
     * @brief Function to load the simulation parameters from a input file and,
     * create the PSO and load its parameters.
     */
    void LoadFile() override;
    /**
     * @brief Function to apply the additional settings for the base simulation
     * and initialize the PSO.
     */
    void AdditionalSettings() override;
    /**
     * @brief Print the base simulation and the PSO parameters in terminal.
     */
    void Print() override;
    /**
     * @brief Save the base simulation results and the PSO in output files.
     */
    void Save() override;
    /**
     * @brief Function to print the description of the PSO simulation.
     * @param ostream Output stream.
     * @return Output stream with the description.
     */
    std::ostream& Help(std::ostream& ostream) override;
    /**
     * @brief Function to return the PSO algorithm of this simulation.
     * @return PSO algorithm.
     */
    PSO* GetPSO();
private:
    /**
     * @brief Function to create the PSO algorithm.
     */
    void CreatePSO();
private:
    /**
     * @brief PSO algorithm.
     */
    std::shared_ptr<PSO> psoAlgorithm;
};

#endif /* SIMULATIONPSO_H */

