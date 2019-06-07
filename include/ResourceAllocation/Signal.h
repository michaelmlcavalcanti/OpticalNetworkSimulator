/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Signal.h
 * Author: BrunoVinicius
 *
 * Created on November 29, 2018, 9:11 PM
 */

#ifndef SIGNAL_H
#define SIGNAL_H

#include <cassert>

class Signal {
public:
    /**
     * @brief Default constructor of a signal. Begin with the update of the 
     * input signal power, ASE power and  nonlinear power.
     */
    Signal();
    /**
     * @brief Default destructor of a signal.
     */
    virtual ~Signal();
    
    /**
     * @brief Function to get the signal power.
     * @return Signal power.
     */
    double GetSignalPower() const;
    /**
     * @brief Function to set the signal power.
     * @param signalPower Signal power.
     */
    void SetSignalPower(double signalPower);
    /**
     * @brief Function to get the signal ASE power.
     * @return Signal ASE power.
     */
    double GetAsePower() const;
    /**
     * @brief Function to set the signal ASE power.
     * @param asePower Signal ASE power.
     */
    void SetAsePower(double asePower);
    /**
     * @brief Function to get the signal nonlinear power.
     * @return Signal nonlinear power.
     */
    double GetNonLinearPower() const;
    /**
     * @brief Function to set the signal nonlinear power.
     * @param nonLinearPower Signal nonlinear power.
     */
    void SetNonLinearPower(double nonLinearPower);
    /**
     * @brief Function to calculate and get the signal OSNR(linear). The OSNR is
     * calculated as function of signal power divided by the sum of ASE and 
     * nonlinear powers.
     * @return Signal OSNR.
     */
    double GetOsnr();
    /**
     * @brief Function to calculate and get the signal OSNR(dB).
     * @return Signal OSNR.
     */
    double GetOSNR();
    /**
     * @brief Function that calculate the ASE power.
     * @param fn Noise factor.
     * @param gain Gain of the EDFA.
     * @return ASE power.
     */
    static double pASE(double fn, double gain);
    /**
     * @brief Function to calculate the power spectral density.
     * @param fn Noise factor.
     * @param gain Gain of the EDFA.
     * @return Power spectral density.
     */
    static double nASE(double fn, double gain); 
    
    /**
     * @brief Central frequency.
     */
    static const double v;
    /**
     * @brief Planck constant.
     */
    static const double h;
    /**
     * @brief Amplifier Noise Figure.   
     */
    static const double Fn;
    /**
     * @brief Amplifier Noise Factor.
     */
    static const double fn;
    /**
     * @brief Reference Bandwidth.
     */
    static const double Bo;
    /**
     * @brief Fiber attenuation coefficient.
     */
    static const double Alpha;
    /**
     * @brief Fiber attenuation coefficient
     */
    static const double alpha;
    /**
     * @brief Signal Input Power(Watts).
     */
    static const double pIn;
    /**
     * @brief Signal Input Power(dBm).
     */
    static const double Pin;
    /**
     * @brief Reference Power at the fiber input(Watts).
     */
    static const double pRef;
    /**
     * @brief Reference Power at the fiber input(dBm).
     */
    static const double Pref;
    /**
     * @brief OSNR of the input signal(dB).
     */
    static const double OSNRin;
    /**
     * @brief OSNR of the input signal(linear).
     */
    static const double osnrIn;
    /**
     * @brief WSS loss(dB).
     */
    static const double Lwss;
    /**
     * @brief WSS loss(linear).
     */
    static const double lWSS;
private:
    /**
     * @brief Signal power(linear).
     */
    double signalPower;
    /**
     * @brief Signal ASE power(linear).
     */
    double asePower;
    /**
     * @brief Signal nonlinear power(linear).
     */
    double nonLinearPower;
};

#endif /* SIGNAL_H */

