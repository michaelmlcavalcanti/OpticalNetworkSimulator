/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_MO.h
 * Author: BrunoVinicius
 *
 * Created on March 27, 2019, 3:28 PM
 */

#ifndef GA_MO_H
#define GA_MO_H

#include "GA.h"

/**
 * @brief Generic genetic algorithm class for multi-objective. This class 
 * contains all generic functions for this type of GA.
 */
class GA_MO : public GA {
public:
    
    GA_MO(SimulationType* simul);
    
    virtual ~GA_MO();
    
    struct IndividualDominated{
        
        bool operator()(const std::shared_ptr<Individual>& indA,
                        const std::shared_ptr<Individual>& indB) const;
    };
    
    struct OrderIndividuals{
        
        bool operator()(const std::shared_ptr<Individual>& indA,
                        const std::shared_ptr<Individual>& indB) const;
    };
    
    void Initialize() override;
    /**
     * @brief Initialize the population with random individuals, created only 
     * in the derived class. 
     */
    virtual void InitializePopulation() = 0;
    /**
     * @brief Function to create the new population based on the select 
     * population.
     */
    virtual void CreateNewPopulation() = 0;
    /**
     * @brief Keeps the initial population in a container.
     */
    void KeepInitialPopulation() override;
    /**
     * @brief Select the population, creating the Pareto fronts.
     */
    void SelectPopulation() override;
    /**
     * @brief Keeps the first Pareto front of each generation.
     */
    void SaveIndividuals() override;

    /**
     * @brief Apply the gene of a specified individual in the network.
     * @param ind Individual pointer.
     */
    virtual void ApplyIndividual(Individual* ind) = 0;
    /**
     * @brief Set the individual parameters found by the simulation.
     * @param ind Specified individual.
     */
    virtual void SetIndParameters(Individual* ind) = 0;
    
    /**
     * @brief Runs the simulation for the Pareto fronts of the GA.
     */
    void RunSelectPop() override;
    /**
     * @brief Runs the simulation for the total population of the GA.
     */
    void RunTotalPop() override;
    /**
     * @brief Check and run the simulation if there is any individual, of total
     * population, with less simulation than the minimum required.
     */
    void CheckMinSimul() override;
    /**
     * @brief Gets the number of individuals in the Pareto fronts.
     * @return 
     */
    unsigned int GetNumIndParetoFronts() const;
private:
    /**
     * @brief Container of individuals, representing the initial population.
     */
    std::vector<std::shared_ptr<Individual>> initialPopulation;
    /**
     * @brief Container to keep the first Pareto front of each generation.
     */
    std::vector<std::vector<std::shared_ptr<Individual>>> firstParetoFronts;
public:
    /**
     * @brief Container of the Pareto fronts of the GA.
     */
    std::vector<std::vector<std::shared_ptr<Individual>>> paretoFronts;
    /**
     * @brief Container of individuals, representing the total population.
     */
    std::vector<std::shared_ptr<Individual>> totalPopulation;
};

#endif /* GA_MO_H */

