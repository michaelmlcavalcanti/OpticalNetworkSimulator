/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MinimumSlotsMinReg.h
 * Author: brunovacorreia
 *
 * Created on August 25, 2019, 10:40 PM
 */

#ifndef MINIMUMSLOTSMINREG_H
#define MINIMUMSLOTSMINREG_H

#include "RegeneratorAssignment.h"

class MinimumSlotsMinReg : public RegeneratorAssignment {
public:
    
    MinimumSlotsMinReg(ResourceDeviceAlloc* resDevAlloc);
    
    virtual ~MinimumSlotsMinReg();
    
    
    void ResourceAlloc(CallDevices* call) override;
    
    void OrderRegenerationOptions(CallDevices* call, 
    std::vector<std::tuple<unsigned, unsigned> >& vec) override;
private:

};

#endif /* MINIMUMSLOTSMINREG_H */

