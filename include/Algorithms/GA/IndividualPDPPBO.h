/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IndividualPDPPBO.h
 * Author: Henrique Dinarte
 *
 * Created on April 1, 2021, 2:46 PM
 */

#ifndef INDIVIDUALPDPPBO_H
#define INDIVIDUALPDPPBO_H

#include <ResourceAllocation/ProtectionSchemes/PartitioningDedicatedPathProtection.h>

class GA_PDPPBO;

#include "Individual.h"
#include "../../Calls/EventGenerator.h"
#include "GA_PDPPBO.h"

/**
 * @brief Specific individual for GA_PDPPBO, 
 * composed by a container of genes, blocking probability and 
 * real simulation time.
 */

class IndividualPDPPBO : public Individual {
public:
    /**
     * @brief Default constructor of this individual.
     * @param ga GA_PDPPBO pointer that owns this individual.
     */
    IndividualPDPPBO(GA_PDPPBO* ga);
    /**
     * @brief Copy constructor of this individual. The newly generated 
     * individual will receive the GA_PDPPBO pointer and the genes
     * of the original individual.
     * @param orig
     */
    IndividualPDPPBO(const std::shared_ptr<const IndividualPDPPBO>& orig);
     /**
     * @brief Default destructor of this individual.
     */
    virtual ~IndividualPDPPBO();
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
     * @brief Gets the beta average of this individual.
     * @return Blocking probability.
     */
    double GetBetaAverage() const;
    /**
     * @brief Sets the beta average of this individual.
     * @param blockProb Blocking probability.
     */
    void SetBetaAverage(double betaAverage);
    /**
     * @brief Gets the first parameter(blocking probability) of this 
     * individual.
     * @return First individual parameter.
     */
    double GetMainParameter() override;
    /**
     * @brief Gets the second parameter(beta average) of this individual.
     * @return Second individual parameter.
     */
    double GetSecondParameter() override;

    std::vector<std::vector<std::vector<double>>> GetGenes() const;

    std::vector<double> GetGene(unsigned orIndex, unsigned deIndex, unsigned traffIndex) const;

    void SetGenes(std::vector<std::vector<std::vector<double>>> genes);

    void SetGene(unsigned orIndex, unsigned deIndex, unsigned traffIndex, std::vector<gene> gene);

private:
    /**
     * @brief GA_PDPPBO that owns this individual.
     */
    GA_PDPPBO* ga;
    /**
     * @brief Blocking probability of this individual.
     */
    double blockProb;
    /**
     * @brief Blocking probability of this individual.
     */
    double betaAverage;
     /**
     * @brief Container of individual genes.
     */
    std::vector<std::vector<std::vector<double>>> genes;
};

#endif /* INDIVIDUALPDPPBO_H */

