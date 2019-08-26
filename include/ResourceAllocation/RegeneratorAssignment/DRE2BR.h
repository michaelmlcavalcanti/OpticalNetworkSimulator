/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DRE2BR.h
 * Author: brunovacorreia
 *
 * Created on August 26, 2019, 10:34 AM
 */

#ifndef DRE2BR_H
#define DRE2BR_H

#include "RegeneratorAssignment.h"

class DRE2BR : public RegeneratorAssignment {
public:
    
    DRE2BR(ResourceDeviceAlloc* resDevAlloc);
    
    virtual ~DRE2BR();
    
    
    void ResourceAlloc(CallDevices* call) override;
    
    double CalcTupleCost(CallDevices* call, unsigned routeIndex, 
    unsigned subRouteIndex) override;
private:

};

#endif /* DRE2BR_H */

