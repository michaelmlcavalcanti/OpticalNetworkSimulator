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

class IndividualNumRoutesMSCL : public Individual {
public:
    
    IndividualNumRoutesMSCL(GA_NumInterRoutesMSCL* ga);
    
    IndividualNumRoutesMSCL(const std::shared_ptr
                            <const IndividualNumRoutesMSCL>& orig);

    virtual ~IndividualNumRoutesMSCL();
    
    
    double GetBlockProb() const;

    void SetBlockProb(double blockProb);

    TIME GetSimulTime() const;

    void SetSimulTime(TIME simulTime);
    
    std::vector<std::vector<unsigned int> > GetGenes() const;
    
    unsigned int GetGene(unsigned int orN, unsigned int deN, unsigned int pos) 
    const;

    void SetGenes(std::vector<std::vector<unsigned int> > genes);

    void SetGene(unsigned int orN, unsigned int deN, unsigned int pos, 
                 unsigned int gene);
    
    double GetMainParameter() override;

    double GetSecondParameter() override;

private:
    
    GA_NumInterRoutesMSCL* ga;
    
    double blockProb;
    
    TIME simulTime;
    
    std::vector<std::vector<unsigned int>> genes;
};

#endif /* INDIVIDUALNUMROUTESMSCL_H */

