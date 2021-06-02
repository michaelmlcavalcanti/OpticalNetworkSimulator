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
     * @brief Function which performs Routing-Spectrum RSA ordering for all Partitioned
     * routes according with PDPP scheme for offline routing and no same slots
     * between routes.
     * @param call Call request the function will try to allocate.
     */
    void RoutingOffNoSameSlotProtPDPPSpecAlloc(CallDevices* call);
    /**
    * @brief Function which performs Spectrum-Routing RSA ordering for all Partitioned
    * routes according with PDPP scheme for offline routing and no same slots
    * between routes.
    * @param call Call request the function will try to allocate.
    */
    void SpecRoutingPDPP(CallDevices* call);
    /**
     * @brief Function which compute a partial bit rate distribution for all 
      * source-destination pair and for all incoming traffic demand possibilities.
     */
    void LoadPDPPBitRateNodePairDist();
    /**
     * @brief Function which calculates the partial bit rate for each 
     * partitioned routes based in incoming traffic demand. A vector 
     * PDPPBitRateDistOptions is loaded with equally distribution between routes. 
     */
    void LoadPDPPBitRateOptions();
    /**
     * @brief Function which create a partial bit rate distribution according on scheme
     * (equally for PDPP and not equally for GA_PDPPBO) for each 
     * partitioned routes based in incoming traffic demand. A vector 
     * PDPPBitRateNodePairDist is loaded. 
     */
    void CreatePDPPBitRateOptions();
    /**
     * @brief Function which get the PDPPBitRateNodePairDist container. 
     */
    std::vector<std::vector<std::vector<double>>> GetPDPPBitRateNodePairsDist() const;
    /**
     * @brief Function which set the PDPPBitRateNodePairDist container. 
     */
    void SetPDPPBitRateNodePairsDist(std::vector<std::vector<std::vector<double>>>
    PDPPBitRateNodePairsDist);
    /**
    * @brief Set the container that indicate the PDPP Bit rate distribution option for
     * each node pair in the network of the output of the first simulation.
    */
    void SetPDPPBitRateNodePairDistGA();

      
private:
    /**
     * @brief Vector to store the partial bit rate distribution options for each
     * possible incoming traffic demand. The 1ª dimension is the incoming traffic
     * demands possibilities and the 2ª dimension is the bit rate distribution
     * options for the protection routes.
     */
    std::vector<std::vector<double>> PDPPBitRateDistOptions;
     /**
     * @brief Vector to store the partial bit rate distribution for all 
     * source-destination pair. The 1ª dimension is the node pair index, the 
     * 2ª dimension is the traffic demands possibilities and 3ª dimension is the 
     * bit rate distribution defined for the protection routes.
     */
    std::vector<std::vector<std::vector<double>>> PDPPBitRateNodePairsDist;
         
};

#endif /* PARTITIONINGDEDICATEDPATHPROTECTION_H */

