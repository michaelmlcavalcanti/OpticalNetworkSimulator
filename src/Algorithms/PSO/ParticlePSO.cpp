/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticlePSO.cpp
 * Author: brunovacorreia
 * 
 * Created on September 11, 2019, 1:52 PM
 */

#include "../../../include/Algorithms/PSO/ParticlePSO.h"
#include "../../../include/Algorithms/PSO/PSO.h"

ParticlePSO::ParticlePSO(PSO* pso)
:pso(pso), fitness(0.0), position(0), velocity(0), bestFitness(0.0), 
bestPosition(0), bestNeighborFitness(0.0), bestNeighborPosition(0), 
neighborParticles(0) {
    unsigned int numDimensions = pso->GetNumberDimensions();
    
    while(position.size() < numDimensions){
        position.push_back(pso->GetRandomPosition());
    }
    
    while(velocity.size() < numDimensions){
        velocity.push_back(pso->GetRandomVelocity());
    }
}

ParticlePSO::ParticlePSO(const std::shared_ptr<const ParticlePSO>& orig)
:pso(orig->pso), fitness(orig->fitness), position(orig->position), 
velocity(orig->velocity), bestFitness(orig->bestFitness), 
bestPosition(orig->bestPosition), bestNeighborFitness(0.0), 
bestNeighborPosition(0), neighborParticles(0) {
    
}

ParticlePSO::~ParticlePSO() {

}

void ParticlePSO::AddNeighborParticle(ParticlePSO* particle) {
    neighborParticles.push_back(particle);
}

void ParticlePSO::CalcNewVelocity() {
    
    for(unsigned int ind = 0; ind < velocity.size(); ind++){
        velocity.at(ind) += pso->GetC1() * pso->GetRandomWeighting() *
                            (bestPosition.at(ind) - position.at(ind));
        velocity.at(ind) += pso->GetC2() * pso->GetRandomWeighting() *
                            (bestNeighborPosition.at(ind) - position.at(ind));
        velocity.at(ind) *= pso->GetConstrictionFactor();
        
        if(velocity.at(ind) < pso->GetMinVelocity())
            velocity.at(ind) = pso->GetMinVelocity();
        else if(velocity.at(ind) > pso->GetMaxVelocity())
            velocity.at(ind) = pso->GetMaxVelocity();
    }
}

void ParticlePSO::CalcNewPosition() {
    
    for(unsigned int ind = 0; ind < position.size(); ind++){
        position.at(ind) += velocity.at(ind);
        
        if(position.at(ind) < pso->GetMinPosition()){
            position.at(ind) = pso->GetMinPosition();
            velocity.at(ind) = 0.0;
        }
        else if(position.at(ind) > pso->GetMaxPosition()){
            position.at(ind) = pso->GetMaxPosition();
            velocity.at(ind) = 0.0;
        }
    }
}

void ParticlePSO::UpdateBestPosition() {
    
    if(fitness > bestFitness){
        bestFitness = fitness;
        bestPosition = position;
    }
}

void ParticlePSO::UpdateNeighborBestPosition() {
    
    for(auto it: neighborParticles){
        
        if(it->GetBestFitness() > bestNeighborFitness){
            bestNeighborFitness = it->GetBestFitness();
            bestNeighborPosition = it->GetBestPosition();
        }
    }
}

double ParticlePSO::GetBestFitness() const {
    return bestFitness;
}

void ParticlePSO::SetFitness(double fitness) {
    assert(fitness > 0.0);
    this->fitness = fitness;
}

std::vector<double> ParticlePSO::GetBestPosition() const {
    return bestPosition;
}
