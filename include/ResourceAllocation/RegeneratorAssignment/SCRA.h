/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SCRA.h
 * Author: brunovacorreia
 *
 * Created on August 26, 2019, 2:42 PM
 */

#ifndef SCRA_H
#define SCRA_H

#include "RegeneratorAssignment.h"

class SCRA : public RegeneratorAssignment {
public:
    
    SCRA(ResourceDeviceAlloc* resDevAlloc, double alpha, double constSlot,
         double constReg);
    
    SCRA(ResourceDeviceAlloc* resDevAlloc, std::ifstream& file);

    virtual ~SCRA();
    
    void ResourceAlloc(CallDevices* call) override;
    
    virtual double CalcTupleCost(CallDevices* call, unsigned routeIndex, 
    unsigned subRouteIndex) override;
    
    void SetAlpha(double alpha);
    
    void SetConstSlot(double constSlot);
    
    void SetConstReg(double constReg);
protected:
    
    double alpha;
    
    double constSlot;
    
    double constReg;
};

#endif /* SCRA_H */

