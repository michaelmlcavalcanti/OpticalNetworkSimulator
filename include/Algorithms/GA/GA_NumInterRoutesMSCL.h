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

class GA_NumInterRoutesMSCL : public GA_MO {
public:
    
    GA_NumInterRoutesMSCL(SimulationType* simul);
    
    virtual ~GA_NumInterRoutesMSCL();
    

    void Initialize() override;
    
    void InitializePopulation() override;

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
    
    void ApplyIndividual(Individual* ind) override;
    /**
     * @brief Set the individual parameters found by the simulation.
     * @param ind Specified individual.
     */
    void SetIndParameters(Individual* ind) override;
    
    
private:
    
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
private:
    /**
     * @brief Number of nodes in the network, used to construct the gene vector.
     */
    unsigned int numNodes;
    
    std::vector<unsigned int> vecNumMaxInterRoutes;
    
    std::uniform_int_distribution<int> numInterRoutesDistribution;
};

#endif /* GA_NUMINTERROUTESMSCL_H */

