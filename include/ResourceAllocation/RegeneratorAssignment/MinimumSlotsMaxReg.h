/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MinimumSlotsMaxReg.h
 * Author: brunovacorreia
 *
 * Created on August 25, 2019, 10:53 PM
 */

#ifndef MINIMUMSLOTSMAXREG_H
#define MINIMUMSLOTSMAXREG_H

#include "RegeneratorAssignment.h"

class MinimumSlotsMaxReg : public RegeneratorAssignment {
public:
    
    MinimumSlotsMaxReg(ResourceDeviceAlloc* resDevAlloc);
    
    virtual ~MinimumSlotsMaxReg();
    
    
    void ResourceAlloc(CallDevices* call) override;
    
    void OrderRegenerationOptions(CallDevices* call, 
    std::vector<std::tuple<unsigned, unsigned> >& vec) override;
private:

};

#endif /* MINIMUMSLOTSMAXREG_H */

