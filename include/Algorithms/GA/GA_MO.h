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

class GA_MO : public GA {
public:
    
    GA_MO(SimulationType* simul);
    
    virtual ~GA_MO();
    
    
    void Initialize() override;

    virtual void InitializePopulation() = 0;
    
    virtual void CreateNewPopulation() = 0;

    void KeepInitialPopulation() override;

    void SelectPopulation() override;

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
    
    
    void RunSelectPop() override;
    
    void RunTotalPop() override;
    
    void CheckMinSimul() override;

private:
    /**
     * @brief Container of individuals, representing the initial population.
     */
    std::vector<std::shared_ptr<Individual>> initialPopulation;
    
    std::vector<std::vector<std::shared_ptr<Individual>>> firstParetoFronts;

public:
    
    std::vector<std::vector<std::shared_ptr<Individual>>> paretoFronts;
    /**
     * @brief Container of individuals, representing the total population.
     */
    std::vector<std::shared_ptr<Individual>> totalPopulation;
};

#endif /* GA_MO_H */

