/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Routing.h
 * Author: BrunoVinicius
 *
 * Created on November 19, 2018, 11:43 PM
 */

#ifndef ROUTING_H
#define ROUTING_H

#include <memory>

class ResourceAlloc;
class Call;
class Route;
class Topology;
class Data;
class Parameters;

#include "../Data/Options.h"
#include "../../include/Structure/Node.h"
#include "Resources.h"

/**
 * @brief Structure to compare the routes cost.
 * Used to organize the priority queue used in the k-shortest path 
 * algorithms.
 */
struct RouteCompare{
    /**
 * @brief Compare the cost of two routes.
 * @param routeA Route to be compared with.
 * @param routeB Route to compare.
 * @return true if the cost of routeA is larger than routeB.
 * False otherwise.
 */
    bool operator()(const std::shared_ptr<Route>& routeA,
                    const std::shared_ptr<Route>& routeB);
};

/**
 * @brief Routing class, responsible for find the routes among all node pairs
 * or the route(s) for a specified node pair.
 */
class Routing {
public:
    /**
     * @brief Standard constructor for a Routing object.
     * @param rsa ResourceAlloc pointer that this object is contained.
     * @param option Routing option chosen for routing function selection.
     * @param topology Topology object used to routing.
     */
    Routing(ResourceAlloc* rsa, RoutingOption option, Data* data, 
            Parameters* parameters);
    /**
     * @brief Standard destructor for a Routing object.
     */
    virtual ~Routing();
    
    void Load();
    /**
     * @brief Apply the routing algorithm for a call. If it is a offline 
     * routing, this function set the routes already chosen to the call.
     * @param call Request for routing.
     */
    void RoutingCall(Call* call);
    /**
     * @brief Sets the set of routes to the call.
     * @param call Request that will receive the set of routes.
     */
    void SetOfflineRouting(Call* call);
    
    /**
     * @brief Dijkstra(shortest path) routing algorithm for all 
     * source/destination node pairs in the topology used.
     */
    void Dijkstra();
    /**
     * @brief Dijkstra routing algorithm for an especified source/destination 
     * node pairs in the topology used.
     * @param orNode Source node Id.
     * @param deNode Destination node Id.
     * @return Route of this node pair.
     */
    std::shared_ptr<Route> Dijkstra(NodeId orNode, NodeId deNode);
    /**
     * @brief YEN(k-shortest path) routing algorithm for all source/destination 
     * node pairs in the topology used.
     */
    void YEN();
    /**
     * @brief YEN routing algorithm for an especified source/destination 
     * node pairs in the topology used.
     * @param orNode Source node Id.
     * @param deNode Destination node Id.
     * @return Routes of this node pair.
     */
    std::vector<std::shared_ptr<Route>> YEN(NodeId orNode, NodeId deNode);
    
    void BSR();
    
    void UpdateLinksUtiCosts(const double alpha);
    
    /**
     * @brief Get a pointer to the ResourceAlloc object that own this routing.
     * @return ResourceAlloc pointer.
     */
    ResourceAlloc* GetResourceAlloc() const;
    /**
     * @brief Set a pointer to the ResourceAlloc object that own this routing.
     * @param rsaAlgorithm ResourceAlloc pointer.
     */
    void SetResourceAlloc(ResourceAlloc* rsaAlgorithm);
    /**
     * @brief Get a pointer to the Routing option chosen.
     * @return Routing option.
     */
    RoutingOption GetRoutingOption() const;
    /**
     * @brief Set a pointer to the Routing option chosen.
     * @param routingOption Routing option.
     */
    void SetRoutingOption(RoutingOption routingOption);
    /**
     * @brief Get the topology used in this routing.
     * @return Topology object.
     */
    Topology* GetTopology() const;
    /**
     * @brief Set the topology used in this routing.
     * @param topology Topology object.
     */
    void SetTopology(Topology* topology);
private:
    /**
     * @brief Get the number k of shortest routes used in YEN.
     * @return Number of routes.
     */
    unsigned int GetK() const;
    /**
     * @brief Set the number k of shortest routes used in YEN.
     * @param K Number of routes.
     */
    void SetK(unsigned int K);
private:
    /**
     * @brief ResourceAlloc object that owns this routing.
     */
    ResourceAlloc* resourceAlloc;
    /**
     * @brief Routing option chosen for this routing.
     */
    RoutingOption routingOption;
    /**
     * @brief Topology used in this routing
     */
    Topology* topology;
    
    Data* data;
    
    Parameters* parameters;
    
    Resources* resources;
    /**
     * @brief Number of routes for the k-shortest path algorithms.
     */
    unsigned int K;
};

#endif /* ROUTING_H */

