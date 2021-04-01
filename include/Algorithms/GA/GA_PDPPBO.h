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

#include "GA_MO.h"

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
     * @brief Gets the number of nodes of the network.
     * @return Number of nodes.
     */
    unsigned int GetNumNodes() const;
    /**
     * @brief Sets the number of nodes of the network.
     * @param numNodes Number of nodes.
     */
    void SetNumNodes(unsigned int numNodes);
private:
    /**
     * @brief Number of nodes in the network, used to construct the gene vector.
     */
    unsigned int numNodes;
    
};

#endif /* GA_PDPPBO_H */

