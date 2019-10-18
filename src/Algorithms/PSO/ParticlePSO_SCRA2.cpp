/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticlePSO_SCRA2.cpp
 * Author: brunovacorreia
 * 
 * Created on October 17, 2019, 2:42 PM
 */

#include "../../../include/Algorithms/PSO/ParticlePSO_SCRA2.h"
#include "../../../include/Algorithms/PSO/PSO.h"
#include "../../../include/ResourceAllocation/RegeneratorAssignment/SCRA2.h"
#include "../../../include/ResourceAllocation/ResourceDeviceAlloc.h"

ParticlePSO_SCRA2::ParticlePSO_SCRA2(PSO* pso, Data* data, 
ResourceAlloc* resAlloc):ParticlePSO_SCRA(pso, data, resAlloc),
scra2(dynamic_cast<SCRA2*>(scra)) {
    assert(pso->GetNumberDimensions() == 4);
}

ParticlePSO_SCRA2::ParticlePSO_SCRA2(const std::shared_ptr<const
ParticlePSO_SCRA2>& orig):ParticlePSO_SCRA(orig), scra2(orig->scra2) {
    
}

ParticlePSO_SCRA2::~ParticlePSO_SCRA2() {
    
}

void ParticlePSO_SCRA2::ApplyCoefficients() {
    ParticlePSO_SCRA::ApplyCoefficients();
    scra2->SetConstNumForms(position.at(4));
}
