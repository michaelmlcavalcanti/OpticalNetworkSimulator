/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PartitioningDedicatedPathProtection.h
 * Author: henrique
 *
 * Created on August 26, 2020, 4:44 PM
 */

#ifndef PARTITIONINGDEDICATEDPATHPROTECTION_H
#define PARTITIONINGDEDICATEDPATHPROTECTION_H

class CallDevices;

#include "ProtectionScheme.h"

class PartitioningDedicatedPathProtection : public ProtectionScheme {
public:
        
    PartitioningDedicatedPathProtection(ResourceDeviceAlloc* rsa);
    virtual ~PartitioningDedicatedPathProtection();
    
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
     * according with PDPP scheme.
     * @param Call vector which contain working and protection paths.
     */
    void ResourceAlloc(CallDevices* call) override;
     /**
     * @brief Function which perform RSA for Working and protection paths  
     * according with PDPP scheme for offline routing and no same slots 
     * between work and Backup paths.
     * @param call Call request the function will try to allocate.
     */
     void RoutingOffNoSameSlotProtPDPPSpecAlloc(CallDevices* call);    
     
     void CalcPDPPBitRate(CallDevices* call);
     
     void CreatePDPPBitRateOptions();
     
    private:
        
    std::vector<double> NodePairPDPPBitRateDist;
        
};

#endif /* PARTITIONINGDEDICATEDPATHPROTECTION_H */

