/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Topology.h
 * Author: bruno
 *
 * Created on August 3, 2018, 12:00 AM
 */

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <vector>
#include <memory>
#include <cassert>
#include <iostream>

class SimulationType;
class Node;
class Link;
class Route;
class Call;
class CallDevices;
class Options;

#include "Devices/Device.h"

/**
 * @brief Class Topology represents the physical topology of the simulation.
 */
class Topology {
    
    friend std::ostream& operator<<(std::ostream& ostream, 
    const Topology* topology);
public:
    /**
     * @brief Standard constructor for a Topology object.
     * @param simulType SimulationType object that owns 
     * this topology.
     */
    Topology(SimulationType* simulType);
    /**
     * @brief Virtual destructor of a Topology object.
     */
    virtual ~Topology();
    
    /**
     * @brief Loads the topology from a .txt file.
     */
    void LoadFile();
    /**
     * @brief Initialize the topology, initializing all 
     * links and nodes.
     */
    void Initialize();
    /**
     * @brief Function used to create all network nodes.
     * @param ifstream File stream used to load the nodes.
     */
    void CreateNodes(std::ifstream& ifstream);
    /**
     * @brief Function used to create all network links.
     * @param ifstream File stream used to load the links.
     */
    void CreateLinks(std::ifstream& ifstream);
    
    /**
     * @brief Returns the number of nodes in this topology
     * @return The total number of nodes
     */
    unsigned int GetNumNodes() const;
    /**
     * @brief Sets the number of nodes in this topology
     * @param numNodes total number of nodes
     */
    void SetNumNodes(unsigned int numNodes);
    /**
     * @brief Returns the number of links in this topology
     * @return The total number of links
     */
    unsigned int GetNumLinks() const;
    /**
     * @brief Sets the number of links in this topology
     * @param numLinks total number of links
     */
    void SetNumLinks(unsigned int numLinks);
    /**
     * @brief Returns the number of slots in this topology
     * @return The total number of slots
     */
    unsigned int GetNumSlots() const;
    /**
     * @brief Sets the number of cores in the links
     * @param numSlots number of core in the links
     */
    void SetNumCores(unsigned int numCores);
    /**
     * @brief Returns the number of cores in the links
     * @return The number of cores in the links
     */
    unsigned int GetNumCores() const;
    /**
     * @brief Sets the number of slots in this topology
     * @param numSlots total number of slots
     */
    void SetNumSlots(unsigned int numSlots);
    /**
     * @brief Insert a created link in the topology
     * The ownership belongs to the vector
     * @param link pointer to a Link object
     */
    void InsertLink(std::shared_ptr<Link> link);
    /**
     * @brief Insert a created node in the topology
     * The ownership belongs to the vector
     * @param node pointer to a Node object
     */
    void InsertNode(std::shared_ptr<Node> node);
    /**
     * @brief Returns the length of the longest link
     * @return Length of the longest link
     */
    double GetMaxLength() const;
    
    /**
     * @brief Function to set the total number of a specified device type.
     * @param numDevices Total number of devices.
     * @param type Devices type.
     */
    void SetNumDevices(unsigned int numDevices, DeviceType type);

    /**
     * @brief Set additional settings of this topology
     */
    void SetAditionalSettings();
    /**
     * @brief Function to set as valid all links in the network.
     */
    void SetAllLinksWorking();
    
    /**
     * @brief Get a specified node in the topology.
     * @param index index of vector of nodes.
     * @return Pointer to a Node object.
     */
    Node* GetNode(unsigned int index) const;
    /**
     * @brief Get a specified link in the topology.
     * @param indexOrNode index of origin node.
     * @param indexDeNode index of destination node.
     * @return Pointer to a Link object.
     */
    Link* GetLink(unsigned int indexOrNode, 
    unsigned int indexDeNode) const;
    /**
     * @brief Return strong pointer to cast
     * @param indexOrNode Origin Node
     * @param indexDeNode Destination Node
     * @return Link pointer
     */
    std::shared_ptr<Link> GetLinkPointer(unsigned int indexOrNode, 
    unsigned int indexDeNode) const;
    /**
     * @brief Checks if an specified slot is free in an specified route.
     * @param route Route to be analyzed.
     * @param slot Slot index.
     * @return True if the slot is available.
     */
    bool CheckSlotDisp(Route* route, unsigned int slot) const;
    /**
     * @brief Checks if a block of slots is free in an specified route.
     * @param route Route to be analyzed.
     * @param iniSlot First slot index.
     * @param finSlot Last slot index.
     * @return True if the block of slots is available.
     */
    bool CheckSlotsDisp(Route* route, unsigned int iniSlot,
                                            unsigned int finSlot) const;
    /**
     * @brief Checks if a block of slots is free in an specified core in a route.
     * @param route Route to be analyzed.
     * @param iniSlot First slot index.
     * @param finSlot Last slot index.
     * @param core Index of the core.
     * @return True if the block of slots is available.
     */
    bool CheckSlotsDispCore(Route* route, unsigned int iniSlot,
                         unsigned int finSlot, unsigned int core) const;
    /**
     * @brief Checks if there is a contiguous block of free slots in
     * an specified route.
     * @param route Route to be analyzed.
     * @param numSlots Number of slots.
     * @return True if there is a block of avaliable slots.
     */
    bool CheckBlockSlotsDisp(Route* route, 
                         unsigned int numSlots) const;
    /**
     * @brief Check if the specified OSNR is larger than the signal OSNR for
     * the specified route.
     * @param route Route to be analyzed.
     * @param OSNRth OSNR value for comparison.
     * @return True if the OSNR is larger.
     */
    bool CheckOSNR(const Route* route, double OSNRth);
    /**
     * @brief Checks if a specified link is valid.
     * @param link Link to evaluate.
     * @return True if the link is valid
     */
    bool IsValidLink(const Link* link);
    /**
     * @brief Checks if a specified node is valid.
     * @param node Node to evaluate.
     * @return True if the node is valid
     */
    bool IsValidNode(const Node* node);
    /**
     * @brief Check if this is a valid route.
     * @param route Route to analyze.
     * @return True if the route is valid.
     */
    bool IsValidRoute(Route* route);
    /**
     * @brief Check if the slot is valid.
     * @param index Index of the slot.
     * @return True if the slot is valid.
     */
    bool IsValidSlot(unsigned int index);
    /**
     * @brief Check if the lightpath is valid. This function checks the route, 
     * the first and the last slot of the call.
     * @param call Call to analyze.
     * @return True if the lightpath is valid.
     */
    bool IsValidLigthPath(Call* call);
    /**
     * @brief Function to check if a lightpath with devices is valid. This 
     * function checks all transparent segments.
     * @param call Call with devices to check.
     * @return True if all transparent segments lightpaths are avalid.
     */
    bool IsValidLigthPath(CallDevices* call);
    /**
     * @brief Check if there is enough available regenerators to connect the 
     * call request. Set the list of regenerators, if found, in the call 
     * parameters.
     * @param call Call request with devices.
     * @return True if the function found a possible list of regenerators.
     */
    bool CheckInsertFreeRegenerators(CallDevices* call);
    /**
     * @brief Function to check if there is enough available BVTs to connect the 
     * call request. Set the list of BVTs, if found, in the call parameters.
     * @param call Call request with devices.
     * @return True if the function found a possible list of BVTs.
     */
    bool CheckInsertFreeBVTs(CallDevices* call);
    /**
     * @brief Function to get the number of occupied slots in all route links.
     * @param route Route to check.
     * @return Number of slots occupied in the route.
     */
    unsigned int GetNumUsedSlots(Route* route) const;
    /**
     * @brief Function to get the total number of slots in all route links.
     * @param route Route to check.
     * @return Number os slots in the route.
     */
    unsigned int GetNumSlots(Route* route) const;
    
    /**
     * @brief Function to set a lightpath in the network of a specified request.
     * @param call Call request used.
     */
    void Connect(Call* call);
    /**
     * @brief Function to disconnect a specified lightpath from the network.
     * @param call Call request to disconnect.
     */
    void Release(Call* call);
    /**
     * @brief Gets the SimulationType object that owns this Toopology.
     * @return Pointer to a SimulationType object.
     */
    SimulationType* GetSimulType() const;
private:
    /**
     * @brief Function to connect a call request without devices usage.
     * @param call Call request.
     */
    void ConnectWithoutDevices(Call* call);
    /**
     * @brief Function to connect a call request with devices usage.
     * @param call Call request.
     */
    void ConnectWithDevices(Call* call);
    /**
     * @brief Function to release a call request without devices usage.
     * @param call Call request.
     */
    void ReleaseWithoutDevices(Call* call);
    /**
     * @brief Function to release a call request with devices usage.
     * @param call Call request.
     */
    void ReleaseWithDevices(Call* call);
    /**
     * @brief Calculate the maximum link in this topology
     * and sets the maxLength value.
     */
    void SetMaxLength();
    /**
     * @brief Set the  initial cost for all links in this
     * topology, based in the selected option.
     */
    void SetLinksIniCost();
    /**
     * @brief Function to set the neighbors nodes for all nodes in the topology.
     */
    void SetNodesNeighbors();
    
    /**
     * @brief Function to set the number of devices in the simulation topology.
     */
    void SetFixedNumberOfDevices();
    /**
     * @brief Function to set the number of virtualized regenerators in the 
     * network.
     * @param numRegenerators Number of virtualized regenerators.
     */
    void SetNumRegenerators(unsigned int numRegenerators);
    /**
     * @brief Function to set the number of BVTs in the network.
     * @param numTransponders Number of BVTs.
     */
    void SetNumTransponders(unsigned int numTransponders);
    /**
     * @brief Function to distribute the device among the network.
     * @param type Device type.
     */
    void DistributeDevices(DeviceType type);
    /**
     * @brief Function to distribute the regenerators among the network nodes.
     */
    void DistributeRegenerators();
    /**
     * @brief Function to distribute the BVTs among the network nodes.
     */
    void DistributeTransponders();
private:
    /**
     * @brief A pointer to the simulation this object belong
     */
    SimulationType* simulType;
    /**
     * @brief Options of this simulation.
     */
    Options* options;
    /**
     * @brief Vector with all topology nodes 
     */
    std::vector<std::shared_ptr<Node>> vecNodes;
    /**
     * @brief Vector with all topology links 
     */
    std::vector<std::shared_ptr<Link>> vecLinks;
    /**
     * @brief Total number of nodes in the topology
     */
    unsigned int numNodes;
    /**
     * @brief Total number of links in the topology
     */
    unsigned int numLinks;
    /**
     * @brief Total number of slots in the topology
     */
    unsigned int numSlots;
    /**
     * @brief Total number of cores in the links of the topology
     */
    unsigned int numCores;
    /**
     * @brief Length of the longest link
     */
    double maxLength;
    /**
     * @brief Number of virtualized regenerators in the network.
     */
    unsigned int numRegenerators;
    /**
     * @brief Number of transponders in the network.
     */
    unsigned int numTransponders;
};

#endif /* TOPOLOGY_H */

