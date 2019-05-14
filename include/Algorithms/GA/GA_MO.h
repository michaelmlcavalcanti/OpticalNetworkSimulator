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
    /**
     * @brief Default constructor of the GA multiobjective.
     * @param simul SimulationType that owns this GA.
     */
    GA_MO(SimulationType* simul);
    /**
     * @brief Default destructor of the GA multiobjective.
     */
    virtual ~GA_MO();
    
    /**
     * @brief Structure for determinate if an individual is dominated by other.
     */
    struct IndividualDominated{
        /**
         * @brief Operator to check if an individual is dominated by the other.
         * @param indA First individual.
         * @param indB Second individual.
         * @return True if individual indA is dominated by individual indB. 
         */
        bool operator()(const std::shared_ptr<Individual>& indA,
                        const std::shared_ptr<Individual>& indB) const;
    };
    /**
     * @brief Structure to compare two individuals, based on their first 
     * parameter (Ascending order).
     */
    struct OrderIndividuals{
        /**
         * @brief Operator to compare two individuals.
         * @param indA First individual.
         * @param indB Second individual.
         * @return True if individual indA first parameter is less than 
         * individual indB first parameter.
         */
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
    
    std::vector<Individual*> GetParetoFront() const;
    
    std::vector<Individual*> GetIniPopulation() const;
    
    
    void print(std::ostream& ostream) const override;
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
    std::vector<std::vector<std::shared_ptr<Individual>>> actualParetoFronts;
    /**
     * @brief Container of individuals, representing the total population.
     */
    std::vector<std::shared_ptr<Individual>> totalPopulation;
};

#endif /* GA_MO_H */

