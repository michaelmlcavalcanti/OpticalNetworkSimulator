//
// Created by michaelcavalcanti on 07/09/22.
//

#ifndef NEWPATHPROTECTION_H
#define NEWPATHPROTECTION_H
class CallDevices;

#include "ProtectionScheme.h"


class NewPathProtection : public ProtectionScheme {
public:

    NewPathProtection(ResourceDeviceAlloc* rsa);
    virtual ~NewPathProtection();

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
    * routes according with PDPP scheme for offline YEN routing and no same set of slots
    * between routes.
    * @param call Call request the function will try to allocate.
    */
    void RoutingSpecProtected(CallDevices* call);
    void RoutingSpecUnprotected(CallDevices* call);
    /**
     * @brief Function which calculates the partial bit rate for each
     * partitioned routes based in incoming traffic demand. A vector
     * PDPPBitRateDistOptions is loaded with equally distribution between routes.
     */
    void LoadBitRateOptions(int numSchProtRoutes);
    /**
     * @brief Function which compute a partial bit rate distribution for all
      * source-destination pair and for all incoming traffic demand possibilities.
     */
    void LoadBitRateNodePairDist();
    /**
     * @brief Function which create a partial bit rate distribution according on scheme
     * (equally for PDPP and not equally for GA_PDPPBO) for each
     * partitioned routes based in incoming traffic demand. A vector
     * PDPPBitRateNodePairDist is loaded.
     */
    void CreateBitRateOptions();
    /**
     * @brief Function which get the PDPPBitRateNodePairDist container.
     */
    std::vector<std::vector<std::vector<double>>> GetBitRateNodePairsDist() const;
    /**
     * @brief Function which set the PDPPBitRateNodePairDist container.
     */
    void SetBitRateNodePairsDist(std::vector<std::vector<std::vector<double>>>
                                 BitRateNodePairsDist);
private:
    /**
     * @brief Vector to store the partial bit rate distribution options for each
     * possible incoming traffic demand. The 1ª dimension is the incoming traffic
     * demands possibilities and the 2ª dimension is the bit rate distribution
     * options for the protection routes.
     */
    std::vector<std::vector<double>> BitRateDistOptions;
    /**
    * @brief Vector to store the partial bit rate distribution for all
    * source-destination pair. The 1ª dimension is the node pair index, the
    * 2ª dimension is the traffic demands possibilities and 3ª dimension is the
    * bit rate distribution defined for the protection routes.
    */
    std::vector<std::vector<std::vector<double>>> BitRateNodePairsDist;

    void CreateUnprotectionCalls(CallDevices *call);
};

#endif //NEWPATHPROTECTION_H
