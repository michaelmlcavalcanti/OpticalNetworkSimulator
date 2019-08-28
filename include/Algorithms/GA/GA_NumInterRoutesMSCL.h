/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_NumInterRoutesMSCL.h
 * Author: BrunoVinicius
 *
 * Created on March 27, 2019, 3:51 PM
 */

#ifndef GA_NUMINTERROUTESMSCL_H
#define GA_NUMINTERROUTESMSCL_H

class IndividualNumRoutesMSCL;

#include "GA_MO.h"

/**
 * @brief GA algorithm applied for the number of interfering routes to check
 * in the MSCL spectral allocation algorithm.
 */
class GA_NumInterRoutesMSCL : public GA_MO {
public:
    /**
     * @brief Default constructor for GA of number of interfering routes.
     * @param simul SimulationType that owns this GA.
     */
    GA_NumInterRoutesMSCL(SimulationType* simul);
    /**
     * @brief Default destructor for GA of number of interfering routes.
     */
    virtual ~GA_NumInterRoutesMSCL();
    
    /**
     * @brief Initialize the base class, set the number of nodes, that will
     * require to construct the vector of genes, and set the container, with
     * the maximum number of interfering routes, per node pair, too check in 
     * the MSCL spectrum allocation.
     */
    void Initialize() override;
    /**
     * @brief Function to create the initial population in the selected 
     * population container. This GA create individuals of the type
     * IndividualNumRoutesMSCL class.
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
     * @brief Gets the maximum number of interfering routes for a specified node
     * pair and for a specified route of this pair.
     * @param orN Source node index.
     * @param deN Destination node index.
     * @param pos Route index.
     * @return Total number of interfering routes of this route.
     */
    unsigned int GetMaxNumInterRoutes(unsigned int orN, unsigned int deN, 
                                      unsigned int pos);
    /**
     * @brief Gets the number of routes for a specified source-destination node
     * pair.
     * @param orN Source node index.
     * @param deN Destination node index.
     * @return Total number routes.
     */
    unsigned int GetNumRoutes(unsigned int orN, unsigned int deN);
    /**
     * @brief 
     * @param orN
     * @param deN
     * @param pos
     * @return 
     */
    unsigned int CreateGene(unsigned int orN, unsigned int deN, 
                            unsigned int pos);
    
    /**
     * @brief Apply the genes of a specified individual in the network.
     * Apply the number of interfering routes to check by MSCL.
     * @param ind Specified individual.
     */
    void ApplyIndividual(Individual* ind) override;
    /**
     * @brief Set the individual parameters found by the simulation.
     * @param ind Specified individual.
     */
    void SetIndParameters(Individual* ind) override;
private:
    /**
     * @brief Function to create new individuals by crossover.
     * Uses each individual only one time to generate new ones.
     */
    void Crossover();
    /**
     * @brief Function to generate two new individuals of two specified 
     * parents.
     * @param ind1 First parent.
     * @param ind2 Second parent.
     */
    void GenerateNewIndividuals(const IndividualNumRoutesMSCL* const ind1,
                                const IndividualNumRoutesMSCL* const ind2);
    /**
     * @brief One point crossover, in which two parents generate two new
     * individuals. A index of the genes is selected. One new individual will
     * receive the genes of the first parent from 0 to the selected index, and 
     * of the second from that index to the end. The other will receive the 
     * opposite genes structure.
     * @param ind1 First parent.
     * @param ind2 Second parent.
     */
    void OnePointCrossover(const IndividualNumRoutesMSCL* const ind1,
                           const IndividualNumRoutesMSCL* const ind2);
    /**
     * @brief Two point crossover, in which two parents generate two new
     * individuals. Two index of the genes is selected. One new individual will
     * receive the genes of the first parent from 0 to the selected first index
     * and the second index to the end. The other will receive the opposite 
     * genes structure.
     * @param ind1 First parent.
     * @param ind2 Second parent.
     */
    void TwoPointCrossover(const IndividualNumRoutesMSCL* const ind1,
                           const IndividualNumRoutesMSCL* const ind2);
    /**
     * @brief Uniform crossover, in which two parents generate two new
     * individuals. the crossover is done by gene, in which the first new
     * individual has a crossover probability to receive the gene of the first
     * parent. The other new individual will receive the gene of the other 
     * parent.
     * @param ind1 First parent.
     * @param ind2 Second parent.
     */
    void UniformCrossover(const IndividualNumRoutesMSCL* const ind1,
                          const IndividualNumRoutesMSCL* const ind2);
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
    void MutateIndividual(IndividualNumRoutesMSCL* const ind);
    /**
     * @brief Function to update the total number of interfering routes to check
     * for all individuals of the population.
     */
    void UpdateNumInterRoutes();
private:
    /**
     * @brief Number of nodes in the network, used to construct the gene vector.
     */
    unsigned int numNodes;
    /**
     * @brief Container with the maximum number of interfering routes, per 
     * node pair and routes of this pair, the MSCL will check.
     */
    std::vector<std::vector<unsigned int>> vecNumMaxInterRoutes;
    /**
     * @brief Probability distribution used in this GA.
     */
    std::uniform_int_distribution<unsigned int> numInterRoutesDistribution;
};

#endif /* GA_NUMINTERROUTESMSCL_H */

