/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IndividualNumRoutesMSCL.h
 * Author: BrunoVinicius
 *
 * Created on March 30, 2019, 1:48 PM
 */

#ifndef INDIVIDUALNUMROUTESMSCL_H
#define INDIVIDUALNUMROUTESMSCL_H

class GA_NumInterRoutesMSCL;

#include "Individual.h"
#include "../../Calls/CallGenerator.h"

/**
 * @brief Specific individual for GA_NumInterRoutesMSCL, 
 * composed by a container of genes, blocking probability and 
 * real simulation time.
 */
class IndividualNumRoutesMSCL : public Individual {
public:
    /**
     * @brief Default constructor of this individual.
     * @param ga GA_NumInterRoutesMSCL pointer that owns this individual.
     */
    IndividualNumRoutesMSCL(GA_NumInterRoutesMSCL* ga);
    /**
     * @brief Copy constructor of this individual. The newly generated 
     * individual will receive the GA_NumInterRoutesMSCL pointer and the genes
     * of the original individual.
     * @param orig
     */
    IndividualNumRoutesMSCL(const std::shared_ptr
                            <const IndividualNumRoutesMSCL>& orig);
    
    IndividualNumRoutesMSCL(const std::shared_ptr<const 
                            IndividualNumRoutesMSCL>& orig, double value);
    /**
     * @brief Default destructor of this individual.
     */
    virtual ~IndividualNumRoutesMSCL();
    
    /**
     * @brief Gets the blocking probability of this individual.
     * @return Blocking probability.
     */
    double GetBlockProb() const;
    /**
     * @brief Sets the blocking probability of this individual.
     * @param blockProb Blocking probability.
     */
    void SetBlockProb(double blockProb);
    /**
     * @brief Gets the simulation time of this individual.
     * @return Simulation time.
     */
    TIME GetSimulTime() const;
    /**
     * @brief Gets the simulation time of this individual.
     * @param simulTime Simulation time.
     */
    void SetSimulTime(TIME simulTime);
    
    unsigned int GetTotalNumInterRoutes() const;
    
    void SetTotalNumInterRoutes();
    
    /**
     * @brief Gets the container of genes of this individual.
     * @return Container of genes.
     */
    std::vector<std::vector<unsigned int> > GetGenes() const;
    /**
     * @brief Gets a specified, determined by the input parameters, gene.
     * @param orN Source node.
     * @param deN Destination node.
     * @param pos Index of the vector of routes for the node pair.
     * @return Gene.
     */
    unsigned int GetGene(unsigned int orN, unsigned int deN, unsigned int pos) 
    const;
    /**
     * @brief Set the container of genes of this individual
     * @param genes Container of genes.
     */
    void SetGenes(std::vector<std::vector<unsigned int> > genes);
    /**
     * @brief Sets a specified, determined by the input parameters, gene.
     * @param orN Source node.
     * @param deN Destination node.
     * @param pos Index of the vector of routes for the node pair.
     * @param gene Gene.
     */
    void SetGene(unsigned int orN, unsigned int deN, unsigned int pos, 
                 unsigned int gene);
    
    
    /**
     * @brief Gets the first parameter(blocking probability) of this 
     * individual.
     * @return First individual parameter.
     */
    double GetMainParameter() override;
    /**
     * @brief Gets the second parameter(simulation time) of this individual.
     * @return Second individual parameter.
     */
    double GetSecondParameter() override;
private:
    /**
     * @brief GA_NumInterRoutesMSCL that owns this individual.
     */
    GA_NumInterRoutesMSCL* ga;
    /**
     * @brief Blocking probability of this individual.
     */
    double blockProb;
    /**
     * @brief Individual simulation time.
     */
    TIME simulTime;
    
    unsigned int totalNumInterRoutes;
    /**
     * @brief Container of individual genes.
     */
    std::vector<std::vector<unsigned int>> genes;
};

#endif /* INDIVIDUALNUMROUTESMSCL_H */

