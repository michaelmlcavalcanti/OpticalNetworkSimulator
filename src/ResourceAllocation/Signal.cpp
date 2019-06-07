/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Signal.cpp
 * Author: BrunoVinicius
 * 
 * Created on November 29, 2018, 9:11 PM
 */

#include "../../include/ResourceAllocation/Signal.h"
#include "../../include/GeneralClasses/General.h"

const double Signal::v = 193.4E12;
const double Signal::h = 6.62606957E-34;
const double Signal::Fn = 5.0;
const double Signal::fn = General::dBToLinear(Fn);
const double Signal::Bo = 12.5E9;
const double Signal::Alpha = 0.2E-3;
const double Signal::pIn = 1E-3;
const double Signal::Pin = General::LinearWTodBm(pIn);
const double Signal::pRef = 1E-3;
const double Signal::Pref = General::LinearWTodBm(pRef);
const double Signal::OSNRin = 1E3;
const double Signal::osnrIn = General::dBToLinear(OSNRin);

Signal::Signal() {
    this->signalPower = this->pIn;
    this->asePower = this->pIn/this->osnrIn;
    this->nonLinearPower = 0.0;
}

Signal::~Signal() {

}

double Signal::GetSignalPower() const {
    return signalPower;
}

void Signal::SetSignalPower(double signalPower) {
    this->signalPower = signalPower;
}

double Signal::GetAsePower() const {
    return asePower;
}

void Signal::SetAsePower(double asePower) {
    this->asePower = asePower;
}

double Signal::GetNonLinearPower() const {
    return nonLinearPower;
}

void Signal::SetNonLinearPower(double nonLinearPower) {
    this->nonLinearPower = nonLinearPower;
}

double Signal::GetOsnr() {
    return this->signalPower/(this->asePower+this->nonLinearPower);
}

double Signal::GetOSNR() {
    return General::LinearTodB(this->GetOsnr());
}

double Signal::pASE(double fn, double gain) {
    return 2 * Signal::nASE(fn, gain) * Signal::Bo;
}

double Signal::nASE(double fn, double gain) {
    assert(gain >=  1.0);
    
    return (Signal::h * Signal::v * (gain-1.0) * fn) / 2.0;
}
