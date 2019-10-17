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
scra(dynamic_cast<SCRA*>(resDevAlloc->GetRegeneratorAssignment())),
blockProb(0.0), bestBlockProb(0.0) {
    assert(pso->GetNumberDimensions() == 3 || pso->GetNumberDimensions() == 4);
}

ParticlePSO_SCRA::ParticlePSO_SCRA(const std::shared_ptr<const 
ParticlePSO_SCRA>& orig): ParticlePSO(orig), resDevAlloc(orig->resDevAlloc),
scra(orig->scra), blockProb(orig->blockProb), 
bestBlockProb(orig->bestBlockProb) {

}

ParticlePSO_SCRA::~ParticlePSO_SCRA() {

}

void ParticlePSO_SCRA::CalculateFitness() {
    this->ApplyCoefficients();
    pso->GetSimul()->RunBase();
    this->SetBlockProb(data->GetPbReq());
    this->SetFitness(1 / data->GetPbReq());
    data->Initialize();
}

double ParticlePSO_SCRA::GetMainParameter() const {
    return this->GetBestBlockProb();
}

void ParticlePSO_SCRA::UpdateMainParameter() {
    this->SetBestBlockProb(this->GetBlockProb());
}

void ParticlePSO_SCRA::ApplyCoefficients() {
    scra->SetAlpha(position.at(0));
    scra->SetConstSlot(position.at(1));
    scra->SetConstReg(position.at(2));
}

double ParticlePSO_SCRA::GetBlockProb() const {
    return blockProb;
}

void ParticlePSO_SCRA::SetBlockProb(double blockProb) {
    assert(blockProb > 0.0 && blockProb < 1.0);
    this->blockProb = blockProb;
}

double ParticlePSO_SCRA::GetBestBlockProb() const {
    return bestBlockProb;
}

void ParticlePSO_SCRA::SetBestBlockProb(double bestBlockProb) {
    assert(bestBlockProb > 0.0 && bestBlockProb < 1.0);
    this->bestBlockProb = bestBlockProb;
}
