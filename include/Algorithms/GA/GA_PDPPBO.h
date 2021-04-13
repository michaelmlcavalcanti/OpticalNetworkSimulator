/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_PDPPBO.h
 * Author: Henrique Dinarte
 *
 * Created on April 1, 2021, 2:14 PM
 */

#ifndef GA_PDPPBO_H
#define GA_PDPPBO_H

class IndividualPDPPBO;
class PartitioningDedicatedPathProtection;

#include "GA_MO.h"
#include "IndividualPDPPBO.h"

/**
 * @brief GA algorithm applied for reaching a optimized Bit Rate distribution 
 * in the PDPP algorithm.
 */
class GA_PDPPBO : public GA_MO {
public:
    /**
     * @brief Default constructor for GA of optimized PDPP bit rate distribution.
     * @param simul SimulationType that owns this GA.
     */
    GA_PDPPBO(SimulationType* simul);
    /**
     * @brief Default destructor for GA of optimized PDPP bit rate distribution.
     */
    virtual ~GA_PDPPBO();
    /**
     * @brief Initialize the base class, set the number of nodes, that will
     * require to construct the vector of genes, and set the container, 
     */
    void Initialize() override;
     /**
     * @brief Function to create the initial population in the selected 
     * population container. This GA create individuals of the type
     * PDPPBO class.
     */
    void InitializePopulation() override;
    /**
     * @brief Call the functions of crossover and mutation for create a new
     * population.
     */
    void CreateNewPopulation() override;
     /**
     * @brief Apply the genes of a specified individual in the network.
     * Apply the Bit rate distribution to check by PDPP.
     * @param ind Specified individual.
     */
    void ApplyIndividual(Individual* ind) override;
    /**
     * @brief Set the individual parameters found by the simulation.
     * @param ind Specified individual.
     */
    void SetIndParameters(Individual* ind) override;
    
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

    unsigned int GetNumTraffic() const;
    /**
     * @brief This function Loads the PDPPBitRateAllDistOption container with 
     * all PDPP bit rate distribution option (3 routes) for each traffic demand.
     */
    void LoadPDPPBitRateAllDistOption();
    /**
     * @brief Function to search a Bit Rate available in the vector of Bit Rate options
     * and create a gene 
     * @param trIndex
     * @return 
     */
    std::vector<double> CreateGene(unsigned int trIndex);
    
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
    void GenerateNewIndividuals(const IndividualPDPPBO* const ind1,
                                const IndividualPDPPBO* const ind2);
     /**
     * @brief Uniform crossover, in which two parents generate two new
     * individuals. the crossover is done by gene, in which the first new
     * individual has a crossover probability to receive the gene of the first
     * parent. The other new individual will receive the gene of the other 
     * parent.
     * @param ind1 First parent.
     * @param ind2 Second parent.
     */
    void UniformCrossover(const IndividualPDPPBO* const ind1,
                          const IndividualPDPPBO* const ind2);
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
    void MutateIndividual(IndividualPDPPBO* const ind);
     /**
     * @brief Function to update the total number of Bit Rate distribution to check
     * for all individuals of the population.
     */
    void UpdatePDPPBO();
    
private:
    PartitioningDedicatedPathProtection* pdppbo;
    /**
     * @brief Number of nodes in the network, used to construct the gene vector.
     */
    unsigned int numNodes;

    unsigned int numTraffic;
    /**
     * @brief Container with the all possibilities of PDPP bit rate distribution
     * for each traffic demand option. The first dimension is traffic demand option,
     * the second one is the number of Bit rate distribution possibilities and 
     * the third one is the container with the Bit Rate distribution (3 routes)
     */
    std::vector<std::vector<std::vector<double>>> PDPPBitRateAllDistOption;

    std::vector<std::uniform_int_distribution<unsigned int>> indexDistOption;
};

#endif /* GA_PDPPBO_H */

