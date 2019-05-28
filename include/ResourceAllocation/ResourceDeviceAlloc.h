/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ResourceDeviceAlloc.h
 * Author: brunovacorreia
 *
 * Created on May 27, 2019, 10:41 PM
 */

#ifndef RESOURCEDEVICEALLOC_H
#define RESOURCEDEVICEALLOC_H

#include "ResourceAlloc.h"
#include "../Data/Options.h"

class Call;
class CallDevices;

class ResourceDeviceAlloc : public ResourceAlloc {
public:
    ResourceDeviceAlloc(SimulationType *simulType);
    
    virtual ~ResourceDeviceAlloc();
    
    void Load() override;

    
    void ResourAlloc(Call* call) override;

private:
    void RoutingRegSpecAlloc(CallDevices* call);
    
    void SetRegChoiceOrder(CallDevices* call, std::vector<unsigned>& vecIndex);
    
    void SetMinRegChoiceOrder(CallDevices* call, 
                              std::vector<unsigned>& vecIndex);
    
    void SetMaxRegChoiceOrder(CallDevices* call, 
                              std::vector<unsigned>& vecIndex);
};

#endif /* RESOURCEDEVICEALLOC_H */

