/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticlePSO_SCRA.cpp
 * Author: brunovacorreia
 * 
 * Created on September 16, 2019, 2:15 PM
 */

#include "../../../include/Algorithms/PSO/ParticlePSO_SCRA.h"
#include "../../../include/Algorithms/PSO/PSO.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Data/Data.h"
#include "../../../include/ResourceAllocation/ResourceDeviceAlloc.h"
#include "../../../include/ResourceAllocation/RegeneratorAssignment/SCRA.h"

ParticlePSO_SCRA::ParticlePSO_SCRA(PSO* pso, Data* data, 
ResourceAlloc* resAlloc): ParticlePSO(pso, data, resAlloc),
resDevAlloc(dynamic_cast<ResourceDeviceAlloc*>(this->resAlloc)),
scra(dynamic_cast<SCRA*>(resDevAlloc->GetRegeneratorAssignment())) {
    assert(pso->GetNumberDimensions() == 3);
}

ParticlePSO_SCRA::ParticlePSO_SCRA(const std::shared_ptr<const 
ParticlePSO_SCRA>& orig): ParticlePSO(orig), resDevAlloc(orig->resDevAlloc),
scra(orig->scra) {

}

ParticlePSO_SCRA::~ParticlePSO_SCRA() {

}

void ParticlePSO_SCRA::CalculateFitness() {
    this->ApplyCoefficients();
    pso->GetSimul()->RunBase();
    this->SetFitness(1 / data->GetPbReq());
    data->Initialize();
}

void ParticlePSO_SCRA::ApplyCoefficients() {
    scra->SetAlpha(position.at(0));
    scra->SetConstSlot(position.at(1));
    scra->SetConstSlot(position.at(2));
}
