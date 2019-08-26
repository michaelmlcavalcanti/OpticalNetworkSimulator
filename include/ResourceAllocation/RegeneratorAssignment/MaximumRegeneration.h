/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MaximumRegeneration.h
 * Author: brunovacorreia
 *
 * Created on August 25, 2019, 11:18 PM
 */

#ifndef MAXIMUMREGENERATION_H
#define MAXIMUMREGENERATION_H

#include "RegeneratorAssignment.h"

class MaximumRegeneration : public RegeneratorAssignment {
public:
    
    MaximumRegeneration(ResourceDeviceAlloc* resDevAlloc);
    
    virtual ~MaximumRegeneration();
    
    
    void ResourceAlloc(CallDevices* call) override;
    
    void OrderRegenerationOptions(CallDevices* call, 
    std::vector<std::tuple<unsigned, unsigned> >& vec) override;
private:

};

#endif /* MAXIMUMREGENERATION_H */

