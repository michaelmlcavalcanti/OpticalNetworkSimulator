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

//#include <vector>
//#include <memory>

class ResourceAlloc;
class Route;
class Call;

#include "Modulation.h"

class Resources {
public:
    
    Resources(ResourceAlloc* resourceAlloc, Modulation* modulation);
    
    virtual ~Resources();
    
    
    ResourceAlloc* GetResourceAlloc() const;
    
    void CreateRegResources();
    
    void CreateOfflineModulation();
    
    void Save();
    
    std::vector<TypeModulation> GetModulationFormat(Call* call);
    
    std::vector<unsigned int> GetNumRegenerators(Call* call);
    
    unsigned int GetNumRegenerators(Call* call, unsigned routeIndex, 
                                    unsigned subRouteIndex);
    
    std::vector<std::vector<unsigned>> GetNumAllRegPos(Call* call);
    
    std::vector<unsigned int> GetNumSlotsRegenerators(Call* call);
    
    std::vector<std::vector<unsigned>> GetNumSlotsAllRegPos(Call* call);
    
    unsigned int GetNumSlotsAllRegPos(Call* call, unsigned routeIndex, 
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
    
    void SetRegSubRoutes();
    
    void SetSubRoutesNumRegSlotsMod();
    
    void MakeSubRoutes(std::shared_ptr<Route> totalRoute, unsigned int curNode,
    unsigned index1, unsigned int index2);
    
    void TestBestModulation(unsigned trIndex, unsigned nodeIndex, 
    unsigned routeIndex, unsigned numSubRoutesIndex, unsigned subRouteIndex);
    
    void TestBestModulation(unsigned trIndex, unsigned nodeIndex, 
    unsigned routeIndex);
    
    void RemoveInvalidRegOptions();
    
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
     * @brief Vector that contain the RSA order, by node pair.
     */
    std::vector<bool> resourceAllocOrder;
    /**
     * @brief Container of the number of interfering routes the algorithm
     * will check, by node pair and route(s) of this pair.
     */
    std::vector<std::vector<unsigned int>> numInterRoutesToCheck;
    /**
     * @brief Container of the possible number of slots the simulation may
     * have. It is used in MSCL SA for find the capacity loss.
     */
    std::vector<unsigned int> numSlotsTraffic;
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
     * @brief Container with the best modulation format used in every 
     * transparent segments of all traffics of all routes of all 
     * node pairs in the network.
     */
    std::vector<std::vector<std::vector<std::vector<std::vector<
                TypeModulation>>>>> subRoutesModulation;
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

