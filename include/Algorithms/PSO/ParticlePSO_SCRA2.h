/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticlePSO_SCRA2.h
 * Author: brunovacorreia
 *
 * Created on October 17, 2019, 2:42 PM
 */

#ifndef PARTICLEPSO_SCRA2_H
#define PARTICLEPSO_SCRA2_H

class SCRA2;

#include "ParticlePSO_SCRA.h"

class ParticlePSO_SCRA2 : public ParticlePSO_SCRA {
public:
    
    ParticlePSO_SCRA2(PSO* pso, Data* data, ResourceAlloc* resAlloc);
    
    ParticlePSO_SCRA2(const std::shared_ptr<const ParticlePSO_SCRA2>& orig);
    
    virtual ~ParticlePSO_SCRA2();
protected:
    
    void ApplyCoefficients() override;
private:
    
    SCRA2* scra2;
};

#endif /* PARTICLEPSO_SCRA2_H */

