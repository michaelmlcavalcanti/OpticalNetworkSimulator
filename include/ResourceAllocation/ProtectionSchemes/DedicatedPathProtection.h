/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DedicatedPathProtection.h
 * Author: henrique
 *
 * Created on October 26, 2019, 9:36 AM
 */

#ifndef DEDICATEDPATHPROTECTION_H
#define DEDICATEDPATHPROTECTION_H

class CallDevices;


#include "ProtectionScheme.h"



class DedicatedPathProtection : public ProtectionScheme {
public:
    
    DedicatedPathProtection(ResourceDeviceAlloc* rsa);

    virtual ~DedicatedPathProtection();
    
    
     /**
     * @brief Function which creates protection disjoint routes for each 
     * working route defined by routing algorithm and stored in allRoutes vector.
     */
    void CreateProtectionRoutes() override;
     /**
     * @brief Function which create a container of calls for working and backup
     * lightpaths.
     * 
     */
    void CreateProtectionCalls(CallDevices* call) override;
     /**
     * @brief Function which perform RSA for Working and protection paths 
     * according with DPP scheme.
     * @param call Call request the function will try to allocate.
     */
    void ResourceAlloc(CallDevices* call) override;
    /**
    * @brief Function which performs Routing-Spectrum RSA ordering for all Partitioned
    * routes according with DPP scheme for offline routing and no same set of slots
    * between routes.
    * @param call Call request the function will try to allocate.
    */
    void RoutingSpecDPP(CallDevices* call);
    /**
    * @brief Function which performs Spectrum-Routi
    * routes according with DPP scheme for offline
    * route.
    * @param call Call request the function will tr
    */
    void SpecRoutingDPP(CallDevices* call);
    /**
    * @brief Function which performs Spectrum-Routing RSA ordering for all Partitioned
    * routes according with DPP scheme for offline routing and same set of slots for each
    * route.
    * @param call Call request the function will try to allocate.
    */
    void SpecRoutingSameSlotDPP(CallDevices* call);
    
private:
    ProtectionScheme* protectionScheme;
       
};

#endif /* DEDICATEDPATHPROTECTION_H */

    