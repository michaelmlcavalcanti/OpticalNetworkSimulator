/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MinimumRegeneration.h
 * Author: brunovacorreia
 *
 * Created on August 25, 2019, 10:27 PM
 */

#ifndef MINIMUMREGENERATION_H
#define MINIMUMREGENERATION_H

#include "RegeneratorAssignment.h"

class MinimumRegeneration : public RegeneratorAssignment {
public:
    
    MinimumRegeneration(ResourceDeviceAlloc* resDevAlloc);
    
    virtual ~MinimumRegeneration();
    
    
    void ResourceAlloc(CallDevices* call) override;
    
    void OrderRegenerationOptions(CallDevices* call, 
    std::vector<std::tuple<unsigned, unsigned> >& vec) override;
private:

};

#endif /* MINIMUMREGENERATION_H */

