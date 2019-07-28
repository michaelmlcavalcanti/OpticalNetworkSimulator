/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Resources.h
 * Author: brunovacorreia
 *
 * Created on May 21, 2019, 7:32 PM
 */

#ifndef RESOURCES_H
#define RESOURCES_H

class ResourceAlloc;
class Route;
class Call;

#include "Modulation.h"
#include "../GeneralClasses/Def.h"

/**
 * @brief Class that represents the control plane resources, such as routes (of
 * all network node pairs), regeneration parameters, RSA order and others.
 */
class Resources {
public:
    /**
     * @brief Standard constructor of Resources class.
     * @param resourceAlloc ResourceAlloc object that owns this Resources.
     * @param modulation Modulation object.
     */
    Resources(ResourceAlloc* resourceAlloc, Modulation* modulation);
    /**
     * @@brief Standard destructor of Resources class. 
     */
    virtual ~Resources();
    
    /**
     * @brief Function to create the offline regeneration resources.
     */
    void CreateRegenerationResources();
    /**
     * @brief Function to create the offline best modulation for each node pair
     * for each possible traffic request.
     */
    void CreateOfflineModulation();
    
    
    void Save();
    
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
     * @brief Function to get the container of routes of all network node
     * pairs.
     * @return Container with all routes.
     */
    std::vector<std::vector<std::shared_ptr<Route>>> GetRoutes() const;
    /**
     * @brief Function to get the container of routes of all network node
     * pairs.
     * @param allRoutes Container with all routes.
     */
    void SetRoutes(std::vector<std::vector<std::shared_ptr<Route>>> allRoutes);
    
    /**
     * @brief Function to get the offline set of modulation formats for each
     * possible route to allocate the call request.
     * @param call Call request.
     * @return Container off modulation formats.
     */
    std::vector<TypeModulation> GetOfflineModulationFormats(Call* call);
    /**
     * @brief Function to get the container with the number of regenerators
     * used by each possible route and each regeneration combination of those
     * routes of a call request.
     * @param call Call request.
     * @return Container with the number of regenerators.
     */
    std::vector<std::vector<unsigned>> GetNumberRegSet(Call* call);
    /**
     * @brief Function to get the container with the number of slots used by 
     * each possible route and each regeneration combination of those routes
     * of a call request.
     * @param call Call request.
     * @return Container with the number of slots.
     */
    std::vector<std::vector<unsigned>> GetNumberSlotsSet(Call* call);
    /**
     * @brief Function to get the number of regeneratos for a specified call
     * request (With predefined bit rate and node pairs), a route index and a
     * possible regeneration option index.
     * @param call Call request.
     * @param routeIndex Route index.
     * @param subRouteIndex Regeneration option index.
     * @return Number of regenerators this option uses.
     */
    unsigned int GetNumberReg(Call* call, unsigned routeIndex, 
                                    unsigned subRouteIndex);
    /**
     * @brief Function to get the number of slots for a specified call request 
     * (With predefined bit rate and node pairs), a route index and a
     * possible regeneration option index.
     * @param call Call request.
     * @param routeIndex Route index.
     * @param subRouteIndex Regeneration option index.
     * @return Number of slots this option uses.
     */
    unsigned int GetNumberSlots(Call* call, unsigned routeIndex, 
                                      unsigned subRouteIndex);
    /**
     * @brief Function to get the number of slots per transparent segment for a 
     * specified call request (With predefined bit rate and node pairs), a 
     * route index and a possible regeneration option index.
     * @param call Call request.
     * @param routeIndex Route index.
     * @param subRouteIndex Regeneration option index.
     * @return Container with the number of slots per transparent segment.
     */
    std::vector<unsigned> GetNumSlotsPerTranspSegments(Call* call, 
    unsigned routeIndex, unsigned subRouteIndex);
    /**
     * @brief Function to get the routes of the transparent segments that 
     * compose the total route for a specified call request (With predefined 
     * route and node pair) for a specified regeneration combination.
     * @param call Call request.
     * @param subRouteIndex Regeneration option index.
     * @return Container of routes of each transparent segment.
     */
    std::vector<std::shared_ptr<Route>> GetRoutesTranspSegments(Call* call, 
    unsigned int subRouteIndex);
    /**
     * @brief Function to get the routes of the transparent segments that 
     * compose the total route for a specified call request (With predefined 
     * node pair) for a specified route and regeneration combination.
     * @param call Call request.
     * @param routeIndex Route index.
     * @param subRouteIndex Regeneration option index.
     * @return Container of routes of each transparent segment.
     */
    std::vector<std::shared_ptr<Route>> GetRoutesTranspSegments(Call* call, 
    unsigned int routeIndex, unsigned int subRouteIndex);
    /**
     * @brief Function to get the modulation formats of each transparent segment
     * for a call request (With predefined bit rate, node pair and route) for
     * a specified regeneration combination.
     * @param call Call request.
     * @param subRouteIndex Regeneration option index.
     * @return Container of modulation formats.
     */
    std::vector<TypeModulation> GetTranspSegmentsModulation(Call* call,
                                                unsigned int subRouteIndex);
private:
    /**
     * @brief Function to create the sets of possible transparent segments for
     * all node pairs routes.
     */
    void CreateRoutesTranspSegments();
    /**
     * @brief Function to create the possible sets of transparent segments of
     * a specified route.
     * @param totalRoute Original complete route.
     * @param curNode Route node index that serve as function source node.
     * @param index1 Node pair index.
     * @param index2 Route index (Used for multiples routes for each node pair).
     */
    void MakePartialRoutes(std::shared_ptr<Route> totalRoute, 
    unsigned int curNode, unsigned index1, unsigned int index2);
    /**
     * @brief Function to create the transparent segments parameters, such as 
     * number of regenerators used, number of slots used, number of slots per
     * transparent segment and best modulation format per transparent segment.
     * This function create the parameters for each traffic request and each 
     * node pair.
     */
    void CreateRoutesTranspSegmentsParameters();
    /**
     * @brief Function to find the best modulation format for a specified 
     * transparent segment.
     * @param trIndex Traffic request index.
     * @param nodeIndex Node pair index.
     * @param routeIndex Node pair route index.
     * @param numSubRoutesIndex Set of transparent segments index.
     * @param subRouteIndex Transparent segment index.
     */
    void TestBestModulation(unsigned trIndex, unsigned nodeIndex, 
    unsigned routeIndex, unsigned numSubRoutesIndex, unsigned subRouteIndex);
    /**
     * @brief Function to remove the sets of transparent segments in which, at
     * least, one transparent segment that does not present a modulation format 
     * that can reach the destination node with the minimum OSNR necessary.
     */
    void RemoveInvalidTranspSegmentsSets();
    
    /**
     * @brief Function to find the best modulation format for a specified 
     * route. If no modulation format is found, the programs stops the 
     * simulation and indicate the node pair not capable of transparent 
     * communication,  indicating the use of a small maximum section distance.
     * @param trIndex Traffic index.
     * @param nodeIndex Node pair index.
     * @param routeIndex Node pair route index.
     */
    void TestBestModulation(unsigned trIndex, unsigned nodeIndex, 
    unsigned routeIndex);
    /**
     * @brieff Function to find the route index for a specified route with node
     * pair.
     * @param route Route to find.
     * @param orNode Source node index.
     * @param deNode Destination node index.
     * @return Route index (container of trial routes).
     */
    unsigned GetRouteIndex(Route* route, unsigned orNode, unsigned deNode);
public:
    /**
     * @brief Vector of all routes for all network node pairs.
     */
    std::vector<std::vector<std::shared_ptr<Route>>> allRoutes;
    /**
     *@brief Vector of vector of vector of route pointers to store interfering 
     * routes of all routes in the network
     */
    std::vector<std::vector<std::vector<std::shared_ptr<Route>>>> interRoutes;
    /**
     * @brief Container of the number of interfering routes the algorithm
     * will check, by node pair and route(s) of this pair.
     */
    std::vector<std::vector<unsigned int>> numInterRoutesToCheck;
    
    /**
     * @brief Container with all possible sets of sub-routes for all routes of
     * all node pairs in the network. This parameter is used for simulations
     * with regeneration, in which will be used to list all possible combination
     * of transparent segments.
     */
    std::vector<std::vector<std::vector<std::vector<std::shared_ptr<Route>>>>>
    subRoutes;
    /**
     * @brief Container with the number of regenerators used in every 
     * combination of transparent segments of all traffics of all routes of all 
     * node pairs in the network.
     */
    std::vector<std::vector<std::vector<std::vector<unsigned int>>>> numReg;
    /**
     * @brief Container with the number of slots used in every 
     * combination of transparent segments of all traffics of all routes of all 
     * node pairs in the network.
     */
    std::vector<std::vector<std::vector<std::vector<unsigned int>>>> numSlots;
    /**
     * @brief Container with the number of slots of each transparent segment of
     * container subRoutes.
     */
    std::vector<std::vector<std::vector<std::vector<std::vector<
    unsigned int>>>>> numSlotsPerSubRoute;
    /**
     * @brief Container with the best modulation format used in every 
     * transparent segments of all traffics of all routes of all 
     * node pairs in the network.
     */
    std::vector<std::vector<std::vector<std::vector<std::vector<
                TypeModulation>>>>> subRoutesModulation;
    
    /**
     * @brief Vector that contain the RSA order, by node pair.
     */
    std::vector<ResAllocOrder> resourceAllocOrder;
    /**
     * @brief Container of the possible number of slots the simulation may
     * have. It is used in MSCL SA for find the capacity loss.
     */
    std::vector<unsigned int> numSlotsTraffic;
private:
    /**
     * @brief ResourceAlloc object that owns these resources.
     */
    ResourceAlloc* resourceAlloc;
    /**
     * @brief Pointer to the modulation object of the simulation.
     */
    Modulation* modulation;
};

#endif /* RESOURCES_H */

