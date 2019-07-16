/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NodeDevices.h
 * Author: BrunoVinicius
 *
 * Created on April 29, 2019, 3:08 PM
 */

#ifndef NODEDEVICES_H
#define NODEDEVICES_H

#include <memory>
#include <vector>

class Regenerator;
class BVT;
class Call;
class CallDevices;

#include "Node.h"
#include "Devices/Regenerator.h"

/**
 * Class that represents a optical node which contain devices. 
 */
class NodeDevices : public Node {
public:
    /**
     * @brief Standard constructor of a network optical node with devices. 
     * @param topPointer Topology in which this node is inserted.
     * @param nodeId Node index.
     */
    NodeDevices(Topology* topPointer, NodeId nodeId);
    /**
     * @brief Standard destructor of a optical node with devices.
     */
    virtual ~NodeDevices();
    
    /**
     * @brief Function to initialize the devices present in this node.
     */
    void Initialize() override;
    
    /**
     * @brief Function to get the total number of virtualized regenerators in 
     * this node.
     * @return Total number of regenerators.
     */
    unsigned int GetNumRegenerator() const;
    /**
     * @brief Function to set the total number of virtualized regenerators in 
     * this node.
     * @param numRegenerator Total number of regenerators.
     */
    void SetNumRegenerator(unsigned int numRegenerator);
    /**
     * @brief Function to get the number of free regenerators in this node.
     * @return Number of free regenerators.
     */
    unsigned int GetNumFreeRegenerators() const;
    /**
     * @brief Function to get the number of occupied regenerators in this node.
     * @return Number of occupied regenerators.
     */
    unsigned int GetNumOccRegenerators() const;
    /**
     * @brief Function to increase the number of free regenerators.
     */
    void IncreaseNumFreeRegenerators();
    /**
     * @brief Function to decrease the number of free regenerators.
     */
    void DecreaseNumFreeRegenerators();
    /**
     * @brief Function to check if there is free regenerators in this node for a 
     * specified bit rate value.
     * @param bitRate Traffic bit rate value.
     * @return True if there is free regenerators.
     */
    bool isThereFreeRegenerators(double bitRate) const;
    /**
     * @brief Function to get the container of free regenerators for a specified
     * bit rate value.
     * @param bitRate Traffic bit rate value.
     * @return Container of free regenerators.
     */
    std::vector<std::shared_ptr<Regenerator>> GetFreeRegenenerators(
                                              double bitRate) const;
    
    /**
     * @brief Function to get the total number of transponders contained in this
     * node.
     * @return Total number of transponders.
     */
    unsigned int GetNumTransponder() const;
    /**
     * @brief Function to set the total number of transponders contained in this
     * node.
     * @param numTransponder Total number of transponders.
     */
    void SetNumTransponder(unsigned int numTransponder);
    /**
     * @brief Function to check if there is free BVT for a specified slots
     * amount.
     * @param numSlots Number of slots to allocate.
     * @return True if there is enough free BVT in this node.
     */
    bool isThereFreeBVT(unsigned int numSlots) const;
    /**
     * @brief Function to get free BVTs used to allocate a specified call 
     * request.
     * @param call Call request with devices to allocate.
     * @return Container of BVTs that will be used to allocate the call 
     * request.
     */
    std::vector<std::shared_ptr<BVT>> GetBVTs(CallDevices* call) const;
private:
    /**
     * @brief Function to initialize the virtualized regenerators in this node. 
     * If the number of regenerators is greater than total, it will create
     * additional regenerators. If less, will delete spare regenerators.
     */
    void InitializeRegnerators();
    /**
     * @brief Function to initialize the transponders in this node. 
     * If the number of transponders is greater than total, it will create
     * additional transponders. If less, will delete spare transponders.
     */
    void InitializeTransponders();
    /**
     * @brief Function to create new virtualized regenerators.
     */
    void CreateRegenerators();
    /**
     * @brief Function to delete spare virtualized regenerators.
     */
    void DeleteRegenerators();
    /**
     * @brief Function to create new transponders.
     */
    void CreateTransponders();
    /**
     * @brief Function to delete spare transponders.
     */
    void DeleteTransponders();
private:
    /**
     * @brief Total number of regenerators in this node.
     */
    unsigned int numRegenerator;
    /**
     * @brief Number of free regenerators in this node.
     */
    unsigned int numFreeRegenerators;
    /**
     * @brief Container of regenerators in this node.
     */
    std::vector<std::shared_ptr<Regenerator>> regenerators;
    
    /**
     * @brief Total number of transponders in this node.
     */
    unsigned int numTransponder;
    /**
     * @brief Container of transponders in this node.
     */
    std::vector<std::shared_ptr<BVT>> transponders;
};

#endif /* NODEDEVICES_H */

