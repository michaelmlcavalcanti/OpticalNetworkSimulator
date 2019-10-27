/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Def.h
 * Author: BrunoVinicius
 *
 * Created on November 21, 2018, 2:37 PM
 */

#ifndef DEF_H
#define DEF_H

#include <random>
#include <limits>

typedef bool CarrierState;
typedef bool SlotState;
#define free false
#define occupied true

typedef bool ResAllocOrder;
#define r_sa false
#define sa_r true

typedef double TIME;

typedef bool UseRegeneration;

typedef unsigned int SimulIndex;
typedef unsigned int NodeIndex;
typedef unsigned int CoreIndex;
typedef unsigned int SlotIndex;
typedef unsigned int RouteIndex;

/**
 * @brief Class responsible to define constants, random engines and other
 * parameters.
 */
class Def {
public:
    /**
     * @brief Define the minimum integer value
     */
    static int Min_Int;
    /**
     * @brief Define the maximum integer value
     */
    static int Max_Int;
    /**
     * @brief Define the maximum unsigned integer value.
     */
    static unsigned int Max_UnInt;
    /**
     * @brief Define the minimum unsigned integer value.
     */
    static unsigned int Min_UnInt;
    /**
     * @brief Define the minimum double value
     */
    static double Min_Double;
    /**
     * @brief Define the maximum double value
     */
    static double Max_Double;
    /**
     * @brief Random device used in the engines to generate real random values.
     */
    static std::random_device randomDevice;
    /**
     * @brief Pseudo random engine.
     */
    static std::default_random_engine pseudoRandomEngine;
    /**
     * @brief Random engine.
     */
    static std::default_random_engine randomEngine;
};

#endif /* DEF_H */

