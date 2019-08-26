/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FirstLongestReach.h
 * Author: brunovacorreia
 *
 * Created on August 25, 2019, 8:04 PM
 */

#ifndef FIRSTLONGESTREACH_H
#define FIRSTLONGESTREACH_H

#include "RegeneratorAssignment.h"

class FirstLongestReach : public RegeneratorAssignment{
public:
    
    FirstLongestReach(ResourceDeviceAlloc* resDevAlloc);
    
    virtual ~FirstLongestReach();
    
    
    void ResourceAlloc(CallDevices* call) override;
    
    void OrderRegenerationOptions(CallDevices* call, 
    std::vector<std::tuple<unsigned, unsigned> >& vec) override;
    
    bool CreateRegOption(CallDevices* call, unsigned routeInd, 
    std::vector<std::shared_ptr<Route> >& routes, 
    std::vector<TypeModulation>& modulations) override;
private:

};

#endif /* FIRSTLONGESTREACH_H */

