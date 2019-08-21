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

/**
 * @brief Class responsible to apply the resource allocation with devices.
 */
class ResourceDeviceAlloc : public ResourceAlloc {
public:
    /**
     * @brief Standard constructor for a resource allocation with devices use.
     * @param simulType Simulation that owns this object.
     */
    ResourceDeviceAlloc(SimulationType *simulType);
    /**
     * @brief Standard destructor for a resource allocation wiith devices.
     */
    virtual ~ResourceDeviceAlloc();
    /**
     * @brief Load the ResourceAlloc.
     */
    void Load() override;

    /**
     * @brief Choose the type of resource allocation for a call, based on the 
     * option chosen.
     * @param call Call request to apply resource allocation.
     */
    void ResourAlloc(Call* call) override;
private:
    /**
     * @brief Function to apply the resource allocation with virtualized 
     * regeneration option. First apply the routing, than try one possible
     * allocation combination of route with regeneration option. The order
     * is decided based on the metric chosen.
     * @param call Call request the function will try to allocate.
     */
    void RoutingVirtRegSpecAlloc(CallDevices* call);
    
    void RoutingTranspSpecAlloc(CallDevices* call);
    
    /**
     * @brief Function to order the tuples of route and regeneration combination
     * for a call request.
     * @param call Call request to order.
     * @param vec Container of tuples of route and regeneration combination 
     * indexes.
     */
    void OrderRegenerationOptions(CallDevices* call, 
    std::vector<std::tuple<unsigned, unsigned>>& vec);
    /**
     * @brief Function to order the tuples of route and regeneration combination 
     * indexes based on the metric of minimum regeneration combination.
     * @param call Call request to order.
     * @param vec Container of tuples of route and regeneration combination 
     * indexes.
     */
    void SetMinRegChoiceOrder(CallDevices* call, 
    std::vector<std::tuple<unsigned, unsigned>>& vec);
    /**
     * @brief Function to order the tuples of route and regeneration combination 
     * indexes based on the metric of minimum slots combination. If there is
     * a draw, the function will favor the one with less used regenerators.
     * @param call Call request to order.
     * @param vec Container of tuples of route and regeneration combination 
     * indexes.
     */
    void SetMinSlotsChoiceOrder(CallDevices* call, 
    std::vector<std::tuple<unsigned, unsigned>>& vec);
    
    void SetMinSlotsMaxRegChoiceOrder(CallDevices* call,
    std::vector<std::tuple<unsigned, unsigned>>& vec);
    /**
     * @brief Function to order the tuples of route and regeneration combination 
     * indexes based on the metric of maximum regeneration combination. If 
     * there is a draw, the function will favor the one with less used slots.
     * @param call Call request to order.
     * @param vec Container of tuples of route and regeneration combination 
     * indexes.
     */
    void SetMaxRegChoiceOrder(CallDevices* call, 
    std::vector<std::tuple<unsigned, unsigned>>& vec);
    
    void SetRegChoiceOrderFLR(CallDevices* call,
    std::vector<std::tuple<unsigned, unsigned>>& vec);
    
    void SetRegChoiceOrderFNS(CallDevices* call,
    std::vector<std::tuple<unsigned, unsigned>>& vec);
    /**
     * @brief Function to order the tuples of route and regeneration combination 
     * indexes based on a cost metric.
     * @param call Call request to order.
     * @param vec Container of tuples of route and regeneration combination 
     * indexes.
     */
    void SetCostMetric(CallDevices* call, 
    std::vector<std::tuple<unsigned, unsigned>>& vec);
    /**
     * @brief Function to calculate the cost of the tuple, that is composed by
     * a route and a regeneration option. The cost depends on the cost metric
     * chosen.
     * @param call Call request to evaluate.
     * @param routeIndex Route index of the container of possible routes.
     * @param subRouteIndex Regeneration option index.
     * @return Tuple cost.
     */
    double CalcTupleCost(CallDevices* call, unsigned routeIndex,
                              unsigned subRouteIndex);
    /**
     * @brief Function to calculate the tuple cost based on the DRE2BR 
     * metric (Walkowiak).
     * @param call Call request to order.
     * @param routeIndex Route index.
     * @param subRouteIndex Regeneration combination index.
     * @return Cost of the tuple.
     */
    double DRE2BR_Cost(CallDevices* call, unsigned routeIndex, 
                  unsigned subRouteIndex);
    /**
     * @brief  Function to calculate the tuple cost based on the SCRA metric.
     * @param call Call request to order.
     * @param routeIndex Route index.
     * @param subRouteIndex Regeneration combination index.
     * @return Cost of the tuple.
     */
    double SCRA1_Cost(CallDevices* call, unsigned routeIndex, 
                unsigned subRouteIndex);
    
    double SCRA2_Cost(CallDevices* call, unsigned routeIndex, 
                unsigned subRouteIndex);
    
    double SCRA3_Cost(CallDevices* call, unsigned routeIndex, 
                unsigned subRouteIndex);
    
    double SCRA4_Cost(CallDevices* call, unsigned routeIndex, 
                unsigned subRouteIndex);
    
    double SCRA5_Cost(CallDevices* call, unsigned routeIndex, 
                unsigned subRouteIndex);
    /**
     * @brief Function to get the call request number of slots using the 
     * least efficient modulation format.
     * @param call Call request.
     * @return Number of slots.
     */
    unsigned int GetN(CallDevices* call);
    /**
     * @brief Function to check the OSNR for a call request with devices.
     * @param call Call to be analise.
     * @return True if the OSNR is acceptable.
     */
    bool CheckOSNR(CallDevices* call);
};

#endif /* RESOURCEDEVICEALLOC_H */

