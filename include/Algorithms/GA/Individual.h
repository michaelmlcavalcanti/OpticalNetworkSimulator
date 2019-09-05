/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Individual.h
 * Author: BrunoVinicius
 *
 * Created on March 8, 2019, 9:45 AM
 */

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <memory>

class GA;

/**
 * @brief Generic individual class, used by genetic algorithm.
 */
class Individual {
    
    friend std::ostream& operator<<(std::ostream& ostream, 
    Individual* ind);
public:
    /**
     * @brief Default constructor of a generic individual.
     * @param ga GA algorithm that owns this individual.
     */
    Individual(GA* ga);
    /**
     * @brief Copy constructor of a generic individual.
     * @param orig Original individual to be copied.
     */
    Individual(const std::shared_ptr<const Individual>& orig);
    /**
     * @brief Default destructor of a generic individual.
     */
    virtual ~Individual();

    /**
     * @brief Gets the fitness of this individual.
     * @return Individual fitness.
     */
    double GetFitness() const;
    /**
     * @brief Sets the fitness of this individual.
     * @param fitness Individual fitness.
     */
    void SetFitness(double fitness);
    /**
     * @brief Gets the simulation count of this individual.
     * @return Simulation count.
     */
    unsigned int GetCount() const;
    /**
     * @brief Sets the simulation count of this individual.
     * @param count Simulation count.
     */
    void SetCount(unsigned int count);
    /**
     * @brief Return the main parameter of this individual. This function was
     * made because for some types of GA, the fitness don't correspond to the
     * main parameter of the individual, as blocking probability.
     * @return Main parameter.
     */
    virtual double GetMainParameter() = 0;
    /**
     * @brief Return the second parameter of this individual. This function was
     * made because for some types of GA, the fitness don't correspond to the
     * main parameter of the individual, as blocking probability.
     * @return Second parameter.
     */
    virtual double GetSecondParameter() = 0;
private:
    /**
     * @brief Generic GA that owns this individual.
     */
    GA* ga;
    /**
     * @brief Fitness value of the individual.
     */
    double fitness;
    /**
     * @brief Simulation count of this individual.
     */
    unsigned int count;
};

#endif /* INDIVIDUAL_H */

