/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticlePSO.h
 * Author: brunovacorreia
 *
 * Created on September 11, 2019, 1:52 PM
 */

#ifndef PARTICLEPSO_H
#define PARTICLEPSO_H

#include <vector>
#include <memory>
#include <cassert>
#include <ostream>

class PSO;
class Data;
class ResourceAlloc;

class ParticlePSO {
    
    friend std::ostream& operator<<(std::ostream& ostream, 
                                    const ParticlePSO* particle);
public:
    
    ParticlePSO(PSO* pso, Data* data, ResourceAlloc* resAlloc);
    
    ParticlePSO(const std::shared_ptr<const ParticlePSO>& orig);

    virtual ~ParticlePSO();
    
    
    void AddNeighborParticle(ParticlePSO* particle);
    
    void CalcNewVelocity();
    
    void CalcNewPosition();
    
    virtual void CalculateFitness() = 0;
    
    void UpdateBestPosition();
    
    void UpdateNeighborBestPosition();
    
    
    double GetBestFitness() const;
    
    virtual double GetMainParameter() const = 0;
    
    virtual void UpdateMainParameter() = 0;

    void SetFitness(double fitness);

    std::vector<double> GetBestPosition() const;
protected:
    
    PSO* pso;
    
    Data* data;
    
    ResourceAlloc* resAlloc;
    
    double fitness;
    
    std::vector<double> position;
    
    std::vector<double> velocity;
    
    double bestFitness;
    
    std::vector<double> bestPosition;
    
    double bestNeighborFitness;
    
    std::vector<double> bestNeighborPosition;
    
    std::vector<ParticlePSO*> neighborParticles;
};

#endif /* PARTICLEPSO_H */

