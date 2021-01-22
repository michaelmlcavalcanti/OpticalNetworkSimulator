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

#include "../../Calls/CallDevices.h"
#include "../Resources.h"
#include "../Routing.h"
#include "../Route.h"
#include "../../Structure/Topology.h"
#include "../ResourceDeviceAlloc.h"
#include "../../Data/Parameters.h"
#include "../Modulation.h"

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
     * @brief Function which create a container of calls for working and backup
     * lightpaths.
     * 
     */
    virtual void CreateProtectionCalls(CallDevices* call) = 0;
    /**
     * @brief Function which returns the number of protected calls of network.
     * 
     */
    long long int GetNumProtectedCalls() const;
     /**
     * @brief Function which increments the number of protected calls of network.
     * 
     */
    void IncrementNumProtectedCalls();
    /**
     * @brief Function which increments the number of nonprotected calls of network.
     * 
     */
    void IncrementNumNonProtectedCalls();

    //void SetPDPPBitRateDistribution(int BitRateDistChoice);
    
    /**
     * @brief vector which stores the distribution Bit rate choice for each route
     * of PDPP protection scheme. 
     * 
     */
    std::vector<unsigned int> PDPPBitRateDistribution;
        
protected:
    ResourceDeviceAlloc* resDevAlloc;
    Resources* resources;
    Topology* topology;
    Parameters* parameters;
    Modulation* modulation;
    
    Routing* routing;
    ProtectionScheme* protectionScheme;
    /**
     * @brief Total number of routes (W + B) in the protection scheme. 
     */
    unsigned int numSchProtRoutes;
    /**
     * @brief Container of call requests for working (first position) and 
     * backup lightpaths (another positions). 
     * Used if the simulation make use of protection. 
     */
    std::vector<std::shared_ptr<Call>> protectionCalls;
    /**
     * @brief Total number of protected calls by the protection scheme. 
     */
    long long int numProtectedCalls;
    
    long int numNonProtectedCalls;
    
};

#endif /* PROTECTIONSCHEME_H */

