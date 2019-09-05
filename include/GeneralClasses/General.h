/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   General.h
 * Author: BrunoVinicius
 *
 * Created on November 29, 2018, 1:41 AM
 */

#ifndef GENERAL_H
#define GENERAL_H

#include <cmath>

class General {
public:
    /**
     * @brief Function to convert linear value to a dB value.
     * @param linValue Linear value.
     * @return Double with the value in dB.
     */
    static double LinearTodB(const double linValue);
    /**
     * @brief Function to convert linear power value to a dB power value.
     * @param powerWatts Power value in Watts.
     * @return Power value in dBm.
     */
    static double LinearWTodBm(const double powerWatts);
    /**
     * @brief Function to convert a dB value to a linear value.
     * @param dBValue Value in dB.
     * @return Linear value.
     */
    static double dBToLinear(const double dBValue);
};

#endif /* GENERAL_H */
