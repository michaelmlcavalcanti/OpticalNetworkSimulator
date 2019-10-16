/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SCRA2.h
 * Author: brunovacorreia
 *
 * Created on October 15, 2019, 8:11 PM
 */

#ifndef SCRA2_H
#define SCRA2_H

#include "SCRA.h"

class SCRA2 : public SCRA {
public:
    
    SCRA2(ResourceDeviceAlloc* resDevAlloc, double alpha, double constSlot, 
          double constReg, double constNumForms);
    
    SCRA2(ResourceDeviceAlloc* resDevAlloc, std::ifstream& file);

    virtual ~SCRA2();

    double CalcTupleCost(CallDevices* call, unsigned routeIndex, 
    unsigned subRouteIndex) override;

    void SetConstNumForms(double constNumForms);
private:
    
    double constNumForms;
};

#endif /* SCRA2_H */

