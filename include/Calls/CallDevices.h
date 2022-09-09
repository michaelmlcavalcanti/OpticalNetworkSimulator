/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CallDevices.h
 * Author: BrunoVinicius
 *
 * Created on April 30, 2019, 2:52 PM
 */

#ifndef CALLDEVICES_H
#define CALLDEVICES_H

#include "Call.h"
#include "../ResourceAllocation/Modulation.h"

class Regenerator;
class BVT;

/**
 * @brief Class that represents a call request that makes use of devices.
 */
class CallDevices : public Call {
public:
    /**
     * @brief Standard constructor for a call request which makes use of 
     * devices.
     * @param orNode Call request source node.
     * @param deNode Call request destination node.
     * @param bitRate Call request bit rate.
     * @param deacTime Call request deactivation time.
     */
    CallDevices(Node* orNode, Node* deNode, double bitRate, TIME deacTime);

    CallDevices(Node *orNode, Node *deNode, double bitRate, TIME deacTime, bool protectionCall);

/**
     * @brief Standard destructor of a call request with devices.
     */
    virtual ~CallDevices();
    
    /**
     * @brief Function to create the transparent segments of the call based on
     * the set of routes that will compose the total lightpath. It will create a
     * container of Calls between regenerators.
     * @param subroutes Set of routes indicating the segments.
     */
    void CreateTranspSegments(std::vector<std::shared_ptr<Route> > subroutes);
    /**
     * @brief Function to set the modulation format for each transparent segment 
     * of the call request.
     * @param modulations Container of modulation formats.
     */
    void SetTranspSegModulation(std::vector<TypeModulation> modulations);
    /**
     * @brief Function to get the container of the transparent segments, in 
     * which each call represents a lightpath between regenerators.
     * @return 
     */
    std::vector<Call*> GetTranspSegments();
    /**
     * @brief Function to get the container of calls pointers to transparent 
     * segments wich represents a lightpath between regenerators.
     * @return transpsegments
     */
     std::vector<std::shared_ptr<Call>> GetTranspSegmentsVec();
    /**
     * @brief Function to set the container of calls pointers to transparent 
     * segments wich represents a lightpath between regenerators.
     * @return transpsegments
     */
    void SetTranspSegments(std::vector<std::shared_ptr<Call>> transpSeg);
    /**
     * @brief Function to get the total number of slots occupied by the 
     * lightpath.
     * @return Call request total number of slots.
     */
    unsigned int GetTotalNumSlots() const override;
    /**
     * @brief Function to set the total number of slots. It will sum the number
     * of slots of each transparent segment.
     */
    void SetTotalNumSlots() override;
    /**
     * @brief Function to get the container of regenerators used to allocate
     * this call request.
     * @return Container of virtualized regenerators.
     */
    std::vector<std::shared_ptr<Regenerator> > GetRegenerators() const;
    /**
     * @brief Function to get the container of regenerators that will be used to 
     * allocate this call request.
     * @param regenerators Container of virtualized regenerators.
     */
    void SetRegenerators(std::vector<std::shared_ptr<Regenerator> > 
    regenerators);
    /**
     * @brief Function to set the use of regeneration by this call request.
     */
    void SetUseRegeneration();
    /**
     * @brief Function to check if this call makes use of regeneration.
     * @return True if the call makes use of regeneration.
     */
    bool CheckUseRegeneration() const;
    
    /**
     * @brief Function to get the container of BVTs used to allocate this call 
     * request. 
     * @return Container of BVTs.
     */
    std::vector<std::shared_ptr<BVT>> GetTransponders() const;
    /**
     * @brief Function to get the container of BVTs that will be used to 
     * allocate this call request.
     * @param transponders Container of BVTs.
     */
    void SetTransponders(std::vector<std::shared_ptr<BVT> > transponders);
    /**
     * @brief Function to clear the container of BVTs used to allocate this 
     * call request.
     */
    void ClearTransponders();

    bool isProtected() const;

    void setProtected(bool protectType);

private:
    /**
     * @brief Container of call request transparent segments. Used if the
     * simulation make use of regeneration.
     */
    std::vector<std::shared_ptr<Call>> transpSegments;
    /**
     * @brief Container of virtualized regenerators used by the call request.
     */
    std::vector<std::shared_ptr<Regenerator>> regenerators;
    /**
     * @brief Information about the regeneration use.
     */
    UseRegeneration useRegeneration;
    
    /**
     * @brief Container of BVTs used by the call request.
     */
    std::vector<std::shared_ptr<BVT>> transponders;

    bool protectionCall;
};

#endif /* CALLDEVICES_H */

