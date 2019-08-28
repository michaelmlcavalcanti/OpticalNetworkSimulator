/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationGA.h
 * Author: brunovacorreia
 *
 * Created on July 31, 2019, 1:32 PM
 */

#ifndef SIMULATIONGA_H
#define SIMULATIONGA_H

#include "SimulationType.h"

class GA;

/**
 * @brief Derived class from SimulationType.
 * This type of simulation runs a genetic algorithm, single or multiple 
 * objectives, for a specified load point.
 */
class SimulationGA : public SimulationType {
public:
    /**
     * @brief Standard constructor for a GA simulation object.
     * @param simulIndex Index of this simulation.
     */
    SimulationGA(unsigned int simulIndex, TypeSimulation typeSimulation);
    /**
     * @brief Destructor of a GA simulation object.
     */
    virtual ~SimulationGA();
        
    /**
     * @brief Runs the genetic algorithm simulation for a fixed load point.
     */
    void Run() override;
    /**
     * @brief Function used to call the Run base SimulationType class function.
     */
    void RunBase() override;
    /**
     * @brief Load simulation inputs from the terminal.
     */
    void Load() override;
    /**
     * @brief Load simulation inputs from files.
     */
    void LoadFile() override;
    /**
     * @brief Function to apply the additional settings of the GA simulation.
     * Also initialize the GA algorithm.
     */
    void AdditionalSettings() override;
    /**
     * @brief Print this simulation
     */
    void Print() override;
    /**
     * @brief Save the simulation results in the files.
     */
    void Save() override;
    /**
     * @brief Prints a description of the simulation.
     * @param ostream Output stream.
     */
    std::ostream& Help(std::ostream& ostream) override;
    /**
     * @brief Get the GA algorithm of this simulation.
     * @return Weak pointer of GA algorithm.
     */
    GA* GetGA() const;
private:
    /**
     * @brief Function to create the GA algorithm, based on the option chosen.
     */
    void CreateGA();
private:
    /**
     * 
     * @brief Genetic algorithm of this simulation.
     */
    std::shared_ptr<GA> gaAlgorithm;
};

#endif /* SIMULATIONGA_H */

