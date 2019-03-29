/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_SO.h
 * Author: BrunoVinicius
 *
 * Created on March 25, 2019, 1:48 PM
 */

#ifndef GA_SO_H
#define GA_SO_H

#include "GA.h"

/**
 * @brief Generic genetic algorithm class for single objective. This class 
 * contains all generic functions for this type of GA.
 */
class GA_SO : public GA {
    /**
     * @brief Struct to compare two individuals, based on their fitness.
     */
    struct IndividualCompare{
        /**
         * @brief Operator to compare two distinct individuals.
         * @param indA First individual.
         * @param indB Second individual.
         * @return True if the first individual fitness is larger than 
         * the second.
         */
        bool operator()(const std::shared_ptr<Individual>& indA,
                        const std::shared_ptr<Individual>& indB) const;
    };
    
    friend std::ostream& operator<<(std::ostream& ostream, 
    const GA_SO* ga);
    
public:
    /**
     * @brief Default constructor of a GA_SO algorithm.
     * @param simul Simulation that own this algorithm.
     */
    GA_SO(SimulationType* simul);
    /**
     * @brief Default destructor of a GA_SO algorithm.
     */
    virtual ~GA_SO();
    
    /**
     * @brief Initialize the GA algorithm with the probability distribution.
     */
    virtual void Initialize() override;
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
     * @brief Keeps the selected population in the initial population container.
     */
    void KeepInitialPopulation() override;
    /**
     * @brief Selects the best population among the total population generated
     * by crossover and mutation. The selection is made first, selecting a 
     * specified number of best individuals, and for last, choosing randomly
     * the rest of the selected population.
     */
    void SelectPopulation() override;
    /**
     * @brief Function that saves the best and the worst individuals in their
     * respective containers.
     */
    void SaveIndividuals() override;

    /**
     * @brief Sets the sumFitness variable.
     * @param sumFitness Sum of fitness.
     */
    void SetSumFitness(double sumFitness);
    /**
     * @brief Calculate and sets the sum of all selected population 
     * individuals fitness.
     */
    void SetSumFitnessSelectedPop();
    
    /**
     * @brief Gets the worst individual of the actual generation.
     * @return Worst individual.
     */
    Individual* GetWorstIndividual() const;
    /**
     * @brief Gets the best individual of the actual generation.
     * @return Best individual.
     */
    Individual* GetBestIndividual() const;
    /**
     * @brief Gets an specified individual from the initial population
     * container,
     * @param index Index of the initial population container.
     * @return Specified individual.
     */
    Individual* GetIniIndividual(unsigned int index);
    
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
     * @brief Pick an individual based on the roulette choosing process. 
     * @return Chosen individual.
     */
    Individual* RouletteIndividual();
    /**
     * @brief Sets the fitness of all individuals in the selected population
     * container.
     */
    virtual void SetSelectedPopFitness() = 0;
    /**
     * @brief Sets the fitness of all individuals in the total population
     * container.
     */
    virtual void SetTotalPopFitness() = 0;
    
    /**
     * @brief Runs the simulation for the selected population of the GA.
     */
    void RunSelectPop() override;
    /**
     * @brief @brief Runs the simulation for the total population of the GA.
     */
    void RunTotalPop() override;
    /**
     * @brief Check and run the simulation if there is any individual, of total
     * population, with less simulation than the minimum required.
     */
    void CheckMinSimul() override;

private:
    /**
     * @brief Number of best individuals the process of selection will choose.
     */
    const unsigned int numBestIndividuals;
    /**
     * @brief Sum of all individuals fitness of the selected individuals 
     * container.
     */
    double sumFitness;
    
    /**
     * @brief Container of individuals, representing the initial population.
     */
    std::vector<std::shared_ptr<Individual>> initialPopulation;
    /**
     * @brief Container of individuals, representing the best individuals, one
     * for each generation.
     */
    std::vector<std::shared_ptr<Individual>> bestIndividuals;
    /**
     * @brief Container of individuals, representing the worst individuals, one
     * for each generation.
     */
    std::vector<std::shared_ptr<Individual>> worstIndividuals;
    
    /**
     * @brief Probability distribution used in this GA algorithm for the 
     * roulette draw.
     */
    std::uniform_real_distribution<double> fitnessDistribution;
public:
    /**
     * @brief Container of individuals, representing the selected population.
     */
    std::vector<std::shared_ptr<Individual>> selectedPopulation;
    /**
     * @brief Container of individuals, representing the total population.
     */
    std::vector<std::shared_ptr<Individual>> totalPopulation;
};

#endif /* GA_SO_H */

