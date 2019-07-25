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

class Resources {
public:
    
    Resources(ResourceAlloc* resourceAlloc, Modulation* modulation);
    
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
    
    
    
    
    std::vector<std::vector<std::shared_ptr<Route>>> GetRoutes() const;

    void SetRoutes(std::vector<std::vector<std::shared_ptr<Route> > > 
                   allRoutes);
    
    std::vector<TypeModulation> GetModulationFormat(Call* call);
    
    std::vector<unsigned int> GetNumRegenerators(Call* call);
    
    unsigned int GetNumRegenerators(Call* call, unsigned routeIndex, 
                                    unsigned subRouteIndex);
    
    std::vector<std::vector<unsigned>> GetNumAllRegPos(Call* call);
    
    std::vector<unsigned int> GetNumSlotsRegenerators(Call* call);
    
    std::vector<std::vector<unsigned>> GetNumSlotsAllRegPos(Call* call);
    
    unsigned int GetNumSlotsAllRegPos(Call* call, unsigned routeIndex, 
                                      unsigned subRouteIndex);
    
    std::vector<unsigned> GetVecNumSlots(Call* call, unsigned routeIndex,
                                         unsigned subRouteIndex);
    
    std::vector<std::shared_ptr<Route>> GetVecSubRoute(Call* call, 
                                                       unsigned int auxIndex);
    
    std::vector<std::shared_ptr<Route>> GetVecSubRoute(Call* call, 
    unsigned int routeIndex, unsigned int subRouteIndex);
    
    std::vector<std::vector<std::vector<std::shared_ptr<Route>>>> 
    GetVecSubRoute(Call* call);
    
    std::vector<TypeModulation> GetSubRoutesMod(Call* call,
                                                unsigned int auxIndex);
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

