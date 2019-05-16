/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_RsaOrder.h
 * Author: BrunoVinicius
 *
 * Created on February 11, 2019, 11:02 AM
 */

#ifndef GA_RSAORDER_H
#define GA_RSAORDER_H

#include "GA_SO.h"

class SimulationType;
class IndividualBool;

/**
 * @brief GA algorithm applied for RSA order.
 */
class GA_RsaOrder : public GA_SO {
public:
    /**
     * @brief Default constructor for GA RSA order.
     * @param simul SimulationType that owns this GA RSA order.
     */
    GA_RsaOrder(SimulationType* simul);
    /**
     * @brief Default destructor for GA RSA order.
     */
    virtual ~GA_RsaOrder();
    
    /**
     * @brief Initialize the base class, set the number of nodes, that will
     * require to construct the vector of genes, and set the boolean 
     * distribution.
     */
    void Initialize() override;
    /**
     * @brief Function to create the initial population in the selected 
     * population container. This GA create individuals of the type
     * IndividualBool class.
     */
    void InitializePopulation() override;
    /**
     * @brief Call the functions of crossover and mutation for create a new
     * population.
     */
    void CreateNewPopulation() override;

    /**
     * @brief Gets the number of nodes of the network.
     * @return Number of nodes.
     */
    unsigned int GetNumNodes() const;
    /**
     * @brief Sets the number of nodes of the network.
     * @param numNodes Number of nodes.
     */
    void SetNumNodes(unsigned int numNodes);
    /**
     * @brief Gets a randomly boolean value.
     * @return Boolean value.
     */
    bool GetBoolDistribution();
    
    /**
     * @brief Apply the genes of a specified individual in the network RSA 
     * order .
     * @param ind Specified individual.
     */
    void ApplyIndividual(Individual* ind) override;
    /**
     * @brief Set the individual parameters. For this GA, set the blocking
     * probability found and the fitness.
     * @param ind Specified individual.
     */
    void SetIndParameters(Individual* ind) override;
    /**
     * @brief Sets the fitness of all individuals in the selected population
     * container.
     */
    void SetSelectedPopFitness() override;
    /**
     * @brief Sets the fitness of all individuals in the total population
     * container.
     */
    void SetTotalPopFitness() override;
private:
    /**
     * @brief Function to create new individuals by crossover.
     * Uses the roulette to choose the parents.
     */
    void Crossover();
    /**
     * @brief Function to generate two new individuals of two specified 
     * parents.
     * @param ind1 First parent.
     * @param ind2 Second parent.
     */
    void GenerateNewIndividuals(const IndividualBool* const ind1,
                                const IndividualBool* const ind2);
    /**
     * @brief One point crossover, in which two parents generate two new
     * individuals. A index of the genes is selected. One new individual will
     * receive the genes of the first parent from 0 to the selected index, and 
     * of the second from that index to the end. The other will receive the 
     * opposite genes structure.
     * @param ind1 First parent.
     * @param ind2 Second parent.
     */
    void OnePointCrossover(const IndividualBool* const ind1,
                           const IndividualBool* const ind2);
    /**
     * @brief Two point crossover, in which two parents generate two new
     * individuals. Two index of the genes is selected. One new individual will
     * receive the genes of the first parent from 0 to the selected first index
     * and the second index to the end. The other will receive the opposite 
     * genes structure.
     * @param ind1 First parent.
     * @param ind2 Second parent.
     */
    void TwoPointCrossover(const IndividualBool* const ind1,
                           const IndividualBool* const ind2);
    /**
     * @brief Uniform crossover, in which two parents generate two new
     * individuals. the crossover is done by gene, in which the first new
     * individual has a crossover probability to receive the gene of the first
     * parent. The other new individual will receive the gene of the other 
     * parent.
     * @param ind1 First parent.
     * @param ind2 Second parent.
     */
    void UniformCrossover(const IndividualBool* const ind1,
                          const IndividualBool* const ind2);
    /**
     * @brief Apply the mutation in all the new individuals created by the 
     * crossover process. After that, this function add the possible parents,
     * in the selected population container, to the total population container.
     * The selected population is clean.
     */
    void Mutation();
    /**
     * @brief Apply the mutation in a specified individual. Each gene of this 
     * individual has the mutation probability to be generated randomly.
     * @param ind Specified individual.
     */
    void MutateIndividual(IndividualBool* const ind);
    
    void DuplicateTotalPop();
private:
    /**
     * @brief Number of nodes in the network, used to construct the gene vector.
     */
    unsigned int numNodes;
    /**
     * @brief Boolean distribution.
     */
    std::uniform_int_distribution<int> boolDistribution;
};

#endif /* GA_RSAORDER_H */

