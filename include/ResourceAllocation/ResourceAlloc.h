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
class Link;
class Routing;
class SA;
class Call;
class Modulation;
class Traffic;
class Resources;
class Parameters;

#include "../Data/Options.h"
#include "../GeneralClasses/Def.h"

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
    virtual void Load();
    /**
     * @brief Function to apply the additional settings for the resource 
     * allocation. It includes the offline routing, creation of interfering
     * route container, creation of regenerator resources, creation of offline
     * modulations and the RSA order.
     */
    void AdditionalSettings();
    
    /**
     * @brief Choose the type of resource allocation for a call, based on the 
     * option chosen.
     * @param call Call request to apply resource allocation.
     */
    virtual void ResourAlloc(Call* call);
    
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
     * @brief Checks if an specified slot is free in an specified route.
     * @param route Route to be analyzed.
     * @param slot Slot index.
     * @return True if the slot is available.
     */
    bool CheckSlotDisp(Route* route, SlotIndex slot) const;
    /**
     * @brief Checks if a block of slots is free in an specified route.
     * @param route Route to be analyzed.
     * @param firstSlot First slot index.
     * @param lastSlot Last slot index.
     * @return True if the block of slots is available.
     */
    bool CheckSlotsDisp(Route* route, SlotIndex firstSlot, 
                        SlotIndex lastSlot) const;
    /**
     * @brief Checks if there is a contiguous block of free slots in
     * an specified route.
     * @param route Route to be analyzed.
     * @param numSlots Number of slots.
     * @return True if there is a block of available slots.
     */
    bool CheckBlockSlotsDisp(Route* route, unsigned int numSlots) const;
    /**
     * @brief Checks if a block of slots is free in an specified core in a route.
     * @param route Route to be analyzed.
     * @param firstSlot First slot index.
     * @param lastSlot Last slot index.
     * @param core Index of the core.
     * @return True if the block of slots is available.
     */
    bool CheckSlotsDispCore(Route* route, SlotIndex firstSlot,
                            SlotIndex lastSlot, CoreIndex core) const;
    /**
     * @brief Check if the specified OSNR is larger than the signal OSNR for
     * the specified route.
     * @param route Route to be analyzed.
     * @param OSNRth OSNR value for comparison.
     * @return True if the OSNR is larger.
     */
    bool CheckOSNR(const Route* route, double OSNRth);
    
    /**
     * @brief Get the SimulationType object that owns this object.
     * @return SimulationType object.
     */
    SimulationType* GetSimulType() const;
    /**
     * @brief Get the Topology object this object will use.
     * @return Topology object.
     */
    Topology* GetTopology() const;
    /**
     * @brief Function to get the resource pointer.
     * @return Resource pointer.
     */
    Resources* GetResources() const;
    /**
     * @brief Function to get the modulation pointer.
     * @return Modulation pointer.
     */
    Modulation* GetModulation() const;
    /**
     * @brief Function to get the traffic pointer.
     * @return Traffic pointer.
     */
    Traffic* GetTraffic() const;
    /**
     * @brief Function to get the resource allocation option.
     * @return Resource allocation option.
     */
    ResourceAllocOption GetResourAllocOption() const;
    
    /**
     * @brief Gets the container that indicate the RSA order (R-SA or SA-R) for
     * each node pair in the network.
     * @return Container of RSA order for all node pairs.
     */
    std::vector<ResAllocOrder> GetResourceAllocOrder() const;
    /**
     * @brief Set the container that indicate the RSA order (R-SA or SA-R) for
     * each node pair in the network.
     * @param resourceAllocOrder Container of RSA order for all node pairs.
     */
    void SetResourceAllocOrder(std::vector<ResAllocOrder> resourceAllocOrder);
    /**
     * @brief Set the container that indicate the RSA order (R-SA or SA-R) for
     * each node pair in the network of the output of the first simulation.
     */
    void SetResourceAllocOrderGA();
    
    bool RsaOrderTopology();
    
    void SetResourceAllocOrderHE();
    
    void SetResAllocOrderHeuristicsRing();
    
    /**
     * @brief 
     * @param route 
     */
    void DisableRouteLinks(Route* route);
    /**
     * @brief Function to calculate the network fragmentation ratio.
     * @return Network fragmentation ratio.
     */
    double CalcNetworkFragmentation() const;
    
    /**
     * @brief Gets the container of interfering routes of a specified node pair 
     * and route of this pair.
     * @param ori Source node index.
     * @param des Destination node index.
     * @param pos Route index of the node pair.
     * @return Interfering routes.
     */
    std::vector<std::shared_ptr<Route>> GetInterRoutes(int ori, int des, 
                                                       int pos);
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
    /**
     * @brief Function to get the container with the number of slots possible 
     * call requests.
     * @return Container of number of slots.
     */
    std::vector<unsigned int> GetNumSlotsTraffic() const;
    /**
     * @brief Function to set the container with the possible number of slots
     * a connection request could have.
     */
    void SetNumSlotsTraffic();
    /**
     * @brief Function to get the availability vector for a specified route.
     * @param route Specified route.
     * @return Container of slots state.
     */
    std::vector<SlotState> GetDispVector(Route* route) const;
    /**
     * @brief Function to calculate the number of forms of a call request number
     * of slots in a specified availability vector.
     * @param callSize Call request number of slots.
     * @param dispVec Availability vector.
     * @return Number of forms.
     */
    unsigned int CalcNumFormAloc(unsigned int callSize, 
    std::vector<SlotState>& dispVec) const;
    /**
     * @brief Function to calculate the number of forms of a call request number
     * of slots in a specified route.
     * @param route Specified route.
     * @param callSize Call request number of slots.
     * @return Number of forms.
     */
    unsigned int CalcNumForms(Route* route, unsigned int callSize);
    /**
     * @brief Function to calculate the number of simultaneous allocations of 
     * a call request number of slots in a specified availability vector.
     * @param callSize Call request number of slots.
     * @param dispVec Availability vector.
     * @return Number of forms.
     */
    unsigned int CalcNumSimultAloc(unsigned int callSize, 
    std::vector<SlotState>& dispVec) const;
    /**
     * @brief Function to get the blocks of free slots equal or larger then the 
     * specified call request number of slots.
     * @param callSize Call request number of slots.
     * @param dispVec Availability vector.
     * @return Container with the size of free slots blocks.
     */
    std::vector<unsigned int> GetBlocksFreeSlots(unsigned int callSize,
    std::vector<SlotState>& dispVec) const;
private:
    /**
     * @brief Function to apply resource allocation without modulation 
     * variation. The RSA order will be determined by the vector 
     * resourceAllocOrder.
     * @param call Call request that the function will try to allocate.
     */
    void RSA(Call* call);
    /**
     * @brief Function to do the resource allocation with variable modulation 
     * formats for a specified call request. Can be done online or offline.
     * @param call Call request.
     */
    void RMSA(Call* call);
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
    /**
     * @brief Function that varies the modulation format to find the best
     * possible one.
     * @param call Call request to allocate.
     */
    void OnlineModulationRSA(Call* call);
    /**
     * @brief Function that apply the pre-computed modulation format for a 
     * specified call request.
     * @param call Call request to allocate.
     */
    void OfflineModulationRSA(Call* call);
    
    /**
     * @brief Function to create the routing object and the other parameters
     * used, based on the routing choice.
     */
    void CreateRouting();
    /**
     * @brief Function to create a spectrum allocation object based on the
     * option chosen.
     */
    void CreateSpecAllocation();
    /**
     * @brief Update the cost values for all routes in container. Used for
     * offline routing.
     */
    void UpdateRoutesCosts();
    /**
     * @brief Function to create the container of RSA order per node pair.
     */
    void CreateRsaOrder();
    
    /**
     * @brief unction to calculate the fragmentation for a specified link.
     * @param link Link to calculate.
     * @return Fragmentation value.
     */
    double CalcLinkFragmentation(Link* link) const;
    /**
     * @brief Function to calculate the fragmentation for a specified link.
     * Spectrum Fragmentation metric (Wang/Mukherjee)
     * @param link Link to calculate.
     * @return Fragmentation value.
     */
    double CalcLinkFragmentationFR(Link* link) const;
    /**
     * @brief Function to calculate the fragmentation for a specified link.
     * External Fragmentation metric.
     * @param link Link to calculate.
     * @return Fragmentation value.
     */
    double CalcLinkFragmentationEF(Link* link) const;
    /**
     * @brief Function to calculate the fragmentation for a specified link.
     * Access Blocking Probability metric.
     * @param link Link to calculate.
     * @return Fragmentation value.
     */
    double CalcLinkFragmentationABP(Link* link) const;
public:
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
     * @breif Pointer to a SimulationType object that
     * owns this RSA.
     */
    SimulationType* simulType;
    /**
     * @breif Pointer to the Parameters object of this simulation.
     */
    Parameters* parameters;
    
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
