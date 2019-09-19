/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PSO.h
 * Author: brunovacorreia
 *
 * Created on September 10, 2019, 3:18 PM
 */

#ifndef PSO_H
#define PSO_H

#include <random>
#include <memory>

class SimulationType;
class ParticlePSO;

class PSO {
    
    friend std::ostream& operator<<(std::ostream& ostream, const PSO* pso);
public:
    
    PSO(SimulationType* simul);
    
    virtual ~PSO();
    
    
    struct ParticleCompare{
        bool operator()(const std::shared_ptr<ParticlePSO>& partA,
                        const std::shared_ptr<ParticlePSO>& partB) const;
    };
    
    void LoadFile();
    
    void RunIteration();
    
    virtual void Initialize();
    
    virtual void InitializePopulation();
    
    void SaveBestParticle();
    
    
    const double GetC1() const;

    const double GetC2() const;
    
    const double GetConstrictionFactor() const;
    
    double GetRandomWeighting();
    
    double GetRandomPosition();
    
    double GetRandomVelocity();
    
    double GetLoadPoint() const;

    unsigned int GetNumberIterations() const;

    unsigned int GetNumberParticles() const;

    unsigned int GetNumberDimensions() const;
    
    double GetMaxPosition() const;

    double GetMaxVelocity() const;

    double GetMinPosition() const;

    double GetMinVelocity() const;
    
    unsigned int GetActualIteration() const;

    void SetActualIteration(unsigned int actualIteration);
    
    SimulationType* GetSimul() const;

    ParticlePSO* GetBestParticle() const;
    
    std::ostream& PrintParameters(std::ostream& ostream) const;
private:
    
    void SetLoadPoint(double loadPoint);
    
    void SetNumberIterations(unsigned int numberIterations);
    
    void SetNumberParticles(unsigned int numberParticles);
    
    void SetNumberDimensions(unsigned int numberDimensions);
    
    void SetMinPosition(double minPosition);
    
    void SetMaxPosition(double maxPosition);
    
    void SetMinVelocity(double minVelocity);
    
    void SetMaxVelocity(double maxVelocity);
    
    void SetParticlesNeighbors();
private:
    
    SimulationType* simul;
    
    double loadPoint;
    
    unsigned int actualIteration;
    
    unsigned int numberIterations;
    
    unsigned int numberParticles;
    
    unsigned int numberDimensions;
    
    double minPosition;
    
    double maxPosition;
    
    double minVelocity;
    
    double maxVelocity;
    
    const double c1 = 2.05;
    
    const double c2 = 2.05;
    
    const double phi = c1 + c2;
    
    const double constrictionFactor = 2.0 / std::abs(2 - phi - 
    std::sqrt(phi * phi  - 4 * phi));
    
    std::uniform_real_distribution<double> constDistibution;
    
    std::uniform_real_distribution<double> posDistribution;
    
    std::uniform_real_distribution<double> velDistribution;
    
    std::vector<std::shared_ptr<ParticlePSO>> particles;
    
    std::vector<std::shared_ptr<ParticlePSO>> bestParticles;
public:
    
    static std::default_random_engine random_engine;
};

#endif /* PSO_H */

