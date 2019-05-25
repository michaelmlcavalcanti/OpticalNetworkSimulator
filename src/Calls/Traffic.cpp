/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Traffic.cpp
 * Author: bruno
 * 
 * Created on August 14, 2018, 1:15 AM
 */

#include <fstream>

#include "../../include/Calls/Traffic.h"

#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Data/Options.h"
#include "../../include/GeneralClasses/Def.h"

std::ostream& operator<<(std::ostream& ostream, 
const Traffic* traffic) {
    unsigned int size = traffic->GetVecTraffic().size();
    
    ostream << "TRAFFIC" << std::endl;
    
    for(unsigned int a = 0; a < size; ++a){
        ostream << "Traffic " << a+1 << "(Gbps): "
                << (traffic->GetTraffic(a) / 1E9) << std::endl;
    }
    
    return ostream;
}

Traffic::Traffic(SimulationType* simulType)
:simulType(simulType), vecTraffic(0) {
}

Traffic::Traffic(const Traffic& orig) {
}

Traffic::~Traffic() {
}

void Traffic::LoadFile() {
    std::ifstream auxIfstream;
    int auxInt;
    double auxDouble;
    
    this->simulType->GetInputOutput()->LoadTraffic(auxIfstream);
    
    auxIfstream >> auxInt;
    for(int a = 0; a < auxInt; a++){
        auxIfstream >> auxDouble;
        vecTraffic.push_back(auxDouble);
    }
}

std::vector<double> Traffic::GetVecTraffic() const {
    return vecTraffic;
}

double Traffic::GetTraffic(unsigned int index) const {
    return this->vecTraffic.at(index);
}

void Traffic::SetVecTraffic(std::vector<double> vecTraffic) {
    this->vecTraffic = vecTraffic;
}

unsigned int Traffic::GetTrafficIndex(double bitRate) const{
    
    for(unsigned a = 0; a < vecTraffic.size(); a++){
        
        if(vecTraffic.at(a) == bitRate)
            return a;
    }
    
    return Def::Max_UnInt;
}
