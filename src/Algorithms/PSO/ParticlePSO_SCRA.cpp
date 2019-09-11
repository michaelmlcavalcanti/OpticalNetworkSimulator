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

ParticlePSO_SCRA::ParticlePSO_SCRA(PSO* pso)
:ParticlePSO(pso) {

}

ParticlePSO_SCRA::ParticlePSO_SCRA(const std::shared_ptr<const 
ParticlePSO_SCRA>& orig): ParticlePSO(orig) {

}

ParticlePSO_SCRA::~ParticlePSO_SCRA() {

}

void ParticlePSO_SCRA::CalculateFitness() {
    SimulationType* simul = pso->GetSimul();
    Data* data = simul->GetData();
    
    //Function to apply the positions in the SCRA.
    simul->RunBase();
    this->SetFitness(1 / data->GetPbReq());
    data->Initialize();
}
