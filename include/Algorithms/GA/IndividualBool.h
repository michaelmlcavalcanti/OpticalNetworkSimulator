/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IndividualBool.h
 * Author: BrunoVinicius
 *
 * Created on February 11, 2019, 11:31 AM
 */

#ifndef INDIVIDUALBOOL_H
#define INDIVIDUALBOOL_H

#include <vector>
#include <memory>
#include <iostream>

class GA_RsaOrder;

#include "Individual.h"

/**
 * @brief Specific individual for RSA order, composed by boolean genes.
 */
class IndividualBool : public Individual {

public:
    /**
     * @brief Default constructor for this specified individual.
     * @param ga GA_RsaOrder pointer that owns this individual.
     */
    IndividualBool(GA_RsaOrder* ga);
    /**
     * @brief Constructor for this individual, that receives a predetermined 
     * gene to apply on it.
     * @param ga GA_RsaOrder pointer that owns this individual.
     * @param gene Gene value for the individual.
     */
    IndividualBool(GA_RsaOrder* ga, bool gene);
    /**
     * @brief Copy constructor for this individual.
     * @param orig Original individual to be copied.
     */
    IndividualBool(const std::shared_ptr<const IndividualBool>& orig);
    /**
     * @brief Default destructor of this individual.
     */
    virtual ~IndividualBool();
    
    /**
     * @brief Gets the gene of  this individual, for a specified node pair, 
     * used to find the container position.
     * @param orNode Source node, used to find the container position.
     * @param deNode Destination node, used to find the container position.
     * @return Gene, in boolean value, of this individual.
     */
    bool GetGene(unsigned int orNode, unsigned int deNode) const;
    /**
     * @brief Gets the container of boolean values, genetical representation 
     * of this individual.
     * @return Genes of this individual.
     */
    std::vector<bool> GetGenes() const;
    /**
     * @brief Sets the gene of  this individual, for a specified node pair, 
     * used to find the container position.
     * @param orNode Source node, used to find the container position.
     * @param deNode Destination node, used to find the container position.
     * @param value Gene, in boolean value, of this individual.
     */
    void SetGene(unsigned int orNode, unsigned int deNode, bool value);
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
     * @brief Gets the main parameter of this individual, in this case the
     * blocking probability.
     * @return Blocking probability.
     */
    double GetMainParameter() override;

private:
    /**
     * @brief GA_RsaOrder that owns this individual.
     */
    GA_RsaOrder* ga;
    /**
     * @brief Genes of this individual, with boolean values.
     */
    std::vector<bool> genes;
    /**
     * @brief Blocking probability of this individual.
     */
    double blockProb;
};

#endif /* INDIVIDUALBOOL_H */

