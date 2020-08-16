/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProtectionScheme.h
 * Author: henrique
 *
 * Created on October 26, 2019, 9:31 AM
 */

#ifndef PROTECTIONSCHEME_H
#define PROTECTIONSCHEME_H

class CallDevices;
class ResourceDeviceAlloc;


#include "../Resources.h"
#include "../Routing.h"
#include "../Route.h"
#include "../../Structure/Topology.h"
#include "../ResourceDeviceAlloc.h"

class ProtectionScheme {
public:
    
    ProtectionScheme(ResourceDeviceAlloc* rsa);
    
    virtual ~ProtectionScheme();
    /**
     * @brief Function to allocate a call with work and protection routes.
     * @param CallDevices call with work and protection routes.
     */
    virtual void ResourceAlloc(CallDevices* call) = 0;
    /**
     * @brief Function which creates a protection route for each working route
     * defined by routing algorithm and stored in allRoutes vector.
     */
    virtual void CreateProtectionRoutes() = 0;
    /**
     * @brief Function which checks if the set of slots used to working and 
     * Protection paths are coherent in terms of continuity and contiguity.
     * 
     */
    void CheckProtectionAllocDisp();
    
protected:
    Resources* resources;
    ResourceDeviceAlloc* resDevAlloc;
    Routing* routing;
    Topology* topology;
};

#endif /* PROTECTIONSCHEME_H */

