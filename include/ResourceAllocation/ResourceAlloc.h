/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ResourceAlloc.h
 * Author: BrunoVinicius
 *
 * Created on November 27, 2018, 8:33 PM
 */

#ifndef RESOURCEALLOC_H
#define RESOURCEALLOC_H

#include <vector>
#include <memory>
#include <cassert>

class SimulationType;
class Topology;
class Route;
class Routing;
class SA;
class Call;
class Modulation;
class Traffic;
class Resources;
class Options;

#include "../Data/Options.h"

/**
 * @brief Class responsible for resource allocation, at least routing and 
 * spectral allocation.
 */
class ResourceAlloc {
public:
    /**
     * @brief Standard constructor for a ResourceAlloc object.
     * @param simulType SimulationType object that owns 
     * this resource allocation.
     */
    ResourceAlloc(SimulationType *simulType);
    /**
     * @brief Virtual destructor of a ResourceAlloc object.
     */
    virtual ~ResourceAlloc();
    /**
     * @brief Create the objects of the class, based in the input options and 
     * parameters. Set the vector size of allRoutes. Also construct the Routing, 
     * SpecAlloc and Modulation objects. Receive resource allocation and the 
     * physical layer options.
     */
    void Load();
    
    void AdditionalSettings();
    
    /**
     * @brief Choose the type of resource allocation for a call, based in the 
     * option.
     * @param call Call request to apply resource allocation.
     */
    void ResourAlloc(Call* call);
    /**
     * @brief Function to apply resource allocation without modulation 
     * variation. The RSA order will be determined by the vector 
     * resourceAllocOrder.
     * @param call Call request that the function will try to allocate.
     */
    void RSA(Call* call);
    /**
     * @brief Function for routing then spectral allocation for a specified 
     * call. For each route in the set of routes, try the spectral allocation.
     * @param call Call request that the function will try to allocate.
     */
    void RoutingSpec(Call* call);
    /**
     * @brief Function for spectral allocation then routing for a specified 
     * call. Try the first possible set of slots in each possible route. If
     * not possible, try the second set of slots, and so on.
     * @param call Call request that the function will try to allocate.
     */
    void SpecRouting(Call* call);
    
    void RMSA(Call* call);
    /**
     * @brief Function for modulation, then routing and then spectral 
     * allocation for a specified call. For each modulation, try the RSA
     * algorithm.
     * @param call Call request that the function will try to allocate.
     */
    void OnlineModulationRSA(Call* call);
    
    void OfflineModulationRSA(Call* call);
    
    /**
     * @brief Function to input one route to the container of all routes for a 
     * specified node pair.
     * @param orN Source node.
     * @param deN Destination node.
     * @param route Route that will be keep.
     */
    void SetRoute(unsigned int orN, unsigned int deN, 
                  std::shared_ptr<Route> route);
    /**
     * @brief Function to input a set of routes to the container of all routes 
     * for a specified node pair.
     * @param orN Source node.
     * @param deN Destination node.
     * @param routes Set of routes that will be keep.
     */
    void SetRoutes(unsigned int orN, unsigned int deN, 
                   std::vector<std::shared_ptr<Route>> routes);
    /**
     * @brief Function that add a route in the container of all routes 
     * for a specified node pair.
     * @param orN Source node.
     * @param deN Destination node.
     * @param route Route that will be added.
     */
    void AddRoute(unsigned int orN, unsigned int deN, 
                  std::shared_ptr<Route> route);
    /**
     * @brief Function that add a set of routes in the container of all routes 
     * for a specified node pair.
     * @param orN Source node.
     * @param deN Destination node.
     * @param routes Set of routes that will be added.
     */
    void AddRoutes(unsigned int orN, unsigned int deN,
                   std::vector<std::shared_ptr<Route>> routes);
    /**
     * @brief Clear all routes in the container of all routes for a specified 
     * node pair.
     * @param orN Source node.
     * @param deN Destination node.
     */
    void ClearRoutes(unsigned int orN, unsigned int deN);
    /**
     * @brief Function that returns a container of routes of a specified node
     * pair.
     * @param orN Source node.
     * @param deN Destination node.
     * @return Vector of routes.
     */
    std::vector<std::shared_ptr<Route>> GetRoutes(unsigned int orN,
                                                  unsigned int deN);
    /**
     * @brief Check if the routing chosen is offline type.
     * @return True if the routing is offline.
     */
    bool IsOfflineRouting();
    /**
     * @brief Runs the offline routing for all node pairs in the network.
     */
    void RoutingOffline();
    /**
     * @brief Set interfering routes for all routes in the network
     */
    void SetInterferingRoutes();
    
    /**
     * @brief Check if the call request OSNR is acceptable.
     * @param call Call request to evaluate the OSNR.
     * @return True if the call presents a acceptable OSNR.
     */
    bool CheckOSNR(Call* call);
    /**
     * @brief Check if this ResourceAlloc will apply R-SA or SA-R, depending 
     * on the order vector.
     * @param call Call request.
     * @return 0 if will apply R-SA or 1 if will apply SA-R.
     */
    bool CheckResourceAllocOrder(Call* call);
    /**
     * @brief Checks if the simulation will need the container of interfering
     * routes of each node pair route(s).
     * @return True if the simulation uses interfering routes.
     */
    bool CheckInterRouting();
    
    /**
     * @brief Get the SimulationType object that owns this object.
     * @return SimulationType object.
     */
    SimulationType* GetSimulType() const;
    /**
     * @brief Set the SimulationType object that owns this object.
     * @param simulType SimulationType object.
     */
    void SetSimulType(SimulationType* simulType);
    /**
     * @brief Get the Topology object this object will use.
     * @return Topology object.
     */
    Topology* GetTopology() const;
    /**
     * @brief Set the Topology object this object will use.
     * @param topology Topology object.
     */
    void SetTopology(Topology* topology);
    
    Resources* GetResources() const;
    
    Traffic* GetTraffic() const;
    
    void SetTraffic(Traffic* traffic);
    
    ResourceAllocOption GetResourAllocOption() const;
    
    /**
     * @brief Gets the container that indicate the RSA order (R-SA or SA-R) for
     * each node pair in the network.
     * @return Container of RSA order for all node pairs.
     */
    std::vector<bool> GetResourceAllocOrder() const;
    /**
     * @brief Set the container that indicate the RSA order (R-SA or SA-R) for
     * each node pair in the network.
     * @param resourceAllocOrder Container of RSA order for all node pairs.
     */
    void SetResourceAllocOrder(std::vector<bool> resourceAllocOrder);
    /**
     * @brief Set the container that indicate the RSA order (R-SA or SA-R) for
     * each node pair in the network of the output of the first simulation.
     */
    void SetResourceAllocOrder();
    
    void SetResAllocOrderHeuristicsRing();
    /**
     * @brief Gets the container of interfering routes of a specified node pair 
     * and route of this pair.
     * @param ori Source node index.
     * @param des Destination node index.
     * @param pos Route index of the node pair.
     * @return Interfering routes.
     */
    std::vector<std::shared_ptr<Route>> GetInterRoutes(int ori,int des,int pos);
    /**
     * @brief Gets the container of interfering routes of a specified node pair 
     * and route of this pair.
     * @param orNode Source node index.
     * @param deNode Destination node index.
     * @param route Route that determine the index of the node pair.
     * @return Interfering routes.
     */
    std::vector<std::shared_ptr<Route>> GetInterRoutes(unsigned int orNode,
    unsigned int deNode, Route* route);
    /**
     * @brief Gets the container of number of interfering routes the MSCL 
     * will check for all node pair and routes in the network.
     * @return Container of number of interfering routes to check.
     */
    std::vector<std::vector<unsigned int>> GetNumInterRoutesToCheck();
    /**
     * @brief Gets the number of interfering routes to check for an specified 
     * node pair and position. Used for MSCL SA algorithm.
     * @param orNode Source node index.
     * @param deNode Destination node index.
     * @param route Route that determine the vector index.
     * @return Number of interfering routes to check.
     */
    unsigned int GetNumInterRoutesToCheck(unsigned int orNode, 
    unsigned int deNode, Route* route);
    /**
     * @brief Set the container of number of interfering routes the MSCL 
     * will check for all node pair and routes in the network.
     * This function will construct the container with the maximum number of
     * interfering routes.
     */
    void SetNumInterRoutesToCheck();
    /**
     * @brief Set the container of number of interfering routes the MSCL 
     * will check for all node pair and routes in the network.
     * @param numInterRoutesToCheck Container of number of interfering routes 
     * to check.
     */
    void SetNumInterRoutesToCheck(std::vector<std::vector<unsigned int>> 
    numInterRoutesToCheck);
    
    std::vector<unsigned int> GetNumSlotsTraffic() const;
private:
    
    void CreateRouting();
    
    void CreateSpecAllocation();
    
    void SetNumSlotsTraffic();
    /**
     * @brief Update the cost values for all routes in container. Used for
     * offline routing.
     */
    void UpdateRoutesCosts();
    
    void CreateRsaOrder();
private:
    /**
     * @breif Pointer to a SimulationType object that
     * owns this RSA.
     */
    SimulationType* simulType;
    /**
     * @brief Pointer to the Topology object of this simulation.
     */
    Topology* topology;
    /**
     * @brief Pointer to the Traffic object of this simulation.
     */
    Traffic* traffic;
    /**
     * @brief Pointer to the Option object of this simulation.
     */
    Options* options;
    /**
     * @brief Resource allocation option chosen.
     */
    ResourceAllocOption resourAllocOption;
    /**
     * @brief Physical layer option chosen.
     */
    PhysicalLayerOption phyLayerOption;
    /**
     * @brief Routing object owned by this object.
     */
    std::shared_ptr<Routing> routing;
    /**
     * @brief SA object owned by this object.
     */
    std::shared_ptr<SA> specAlloc;
    /**
     * @brief Modulation object owned by this object.
     */
    std::shared_ptr<Modulation> modulation;
    /**
     * @brief Resource object owned by the object.
     */
    std::shared_ptr<Resources> resources;
};
#endif /* RESOURCEALLOC_H */
