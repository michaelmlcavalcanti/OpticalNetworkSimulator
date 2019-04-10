/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Def.cpp
 * Author: BrunoVinicius
 * 
 * Created on November 21, 2018, 2:37 PM
 */

#include "../../include/GeneralClasses/Def.h"

int Def::Min_Int = std::numeric_limits<int>::lowest();
int Def::Max_Int = std::numeric_limits<int>::max();
unsigned int Def::Min_UnInt = std::numeric_limits<unsigned int>::lowest();
unsigned int Def::Max_UnInt = std::numeric_limits<unsigned int>::max();
double Def::Min_Double = std::numeric_limits<double>::lowest();
double Def::Max_Double = std::numeric_limits<double>::max();
std::random_device Def::randomDevice;
std::default_random_engine Def::pseudoRandomEngine(0);
std::default_random_engine Def::randomEngine(Def::randomDevice());