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
    
    static unsigned int Max_UnInt;
    
    static unsigned int Min_UnInt;
    /**
     * @brief Define the minimum double value
     */
    static double Min_Double;
    /**
     * @brief Define the maximum double value
     */
    static double Max_Double;

    static std::random_device randomDevice;
    
    static std::default_random_engine pseudoRandomDevice;
private:

};

#endif /* DEF_H */

