/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_SingleObjective.h
 * Author: BrunoVinicius
 *
 * Created on February 11, 2019, 10:53 PM
 */

#ifndef GA_SINGLEOBJECTIVE_H
#define GA_SINGLEOBJECTIVE_H

#include "SimulationType.h"

class GA;

#include <iostream>

/**
 * @brief Derived class from SimulationType.
 * This type pf simulation runs a genetic algorithm for a single load point,
 * for a single objective(fitness).
 */
class GA_SingleObjective : public SimulationType {

public:
    /**
     * @brief Standard constructor for a GA_SingleObjective object.
     * @param simulIndex Index of this simulation.
     */
    GA_SingleObjective(unsigned int simulIndex);
    /**
     * @brief Destructor of a GA_SingleObjective object.
     */
    virtual ~GA_SingleObjective();
    
    /**
     * @brief Runs the genetic algorithm simulation for a fixed load point.
     */
    void Run() override;
    /**
     * @brief Load simulation inputs from the terminal.
     */
    void Load() override;
    /**
     * @brief Load simulation inputs from files.
     */
    void LoadFile() override;
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
     */
    void Help() override;
    /**
     * @brief Get the GA algorithm of this simulation.
     * @return Weak pointer of GA algorithm.
     */
    GA* GetGA() const;
    
    /**
     * @brief Runs the simulation for the selected population of the GA.
     */
    void RunSelectPop();
    /**
     * @brief @brief Runs the simulation for the total population of the GA.
     */
    void RunTotalPop();
    /**
     * @brief Check and run the simulation if there is any individual, of total
     * population, with less simulation than the minimum required.
     */
    void CheckMinSimul();
private:
    /**
     * 
     * @brief Genetic algorithm of this simulation.
     */
    std::shared_ptr<GA> gaAlgorithm;
    
};

#endif /* GA_SINGLEOBJECTIVE_H */

