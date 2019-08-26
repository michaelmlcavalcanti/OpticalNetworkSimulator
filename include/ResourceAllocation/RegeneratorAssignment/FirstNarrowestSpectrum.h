/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FirstNarrowestSpectrum.h
 * Author: brunovacorreia
 *
 * Created on August 25, 2019, 11:38 PM
 */

#ifndef FIRSTNARROWESTSPECTRUM_H
#define FIRSTNARROWESTSPECTRUM_H

#include "RegeneratorAssignment.h"

class FirstNarrowestSpectrum : public RegeneratorAssignment {
public:
    
    FirstNarrowestSpectrum(ResourceDeviceAlloc* resDevAlloc);
    
    virtual ~FirstNarrowestSpectrum();
    
    
    void ResourceAlloc(CallDevices* call) override;
    
    bool CreateRegOption(CallDevices* call, unsigned routeInd, 
    std::vector<std::shared_ptr<Route> >& routes, 
    std::vector<TypeModulation>& modulations) override;
private:

};

#endif /* FIRSTNARROWESTSPECTRUM_H */

