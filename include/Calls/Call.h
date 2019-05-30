/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Call.h
 * Author: bruno
 *
 * Created on August 17, 2018, 11:40 PM
 */

#ifndef CALL_H
#define CALL_H

#include <vector>
#include <memory>
#include <deque>

class Node;
class SimulationType;
class Route;
class Topology;

#include "../Structure/Link.h"
#include "CallGenerator.h"
#include "../ResourceAllocation/Modulation.h"

/**
 * @brief Lists the possible status of a Call.
 */
enum CallStatus{
    NotEvaluated,
    Accepted,
    Blocked
};

class Call {
    
    friend std::ostream& operator<<(std::ostream& ostream, 
    const Call* call);
    
public:
    
    Call();
    /**
     * @brief Standard constructor of a Call object.
     * @param orNode call origin node.
     * @param deNode call destination node.
     * @param bitRate call bit rate.
     */
    Call(Node* orNode, Node* deNode, double bitRate, TIME deacTime);
    
    Call(const Call& orig);
    
    virtual ~Call();
    
    /**
     * @brief Return the status of this Call.
     * @return Call status.
     */
    CallStatus GetStatus() const;
    /**
     * @brief Returns the name of the call status.
     * @return String with the name of the call status.
     */
    std::string GetStatusName() const;
    /**
     * @brief Sets the status of this Call.
     * @param status Status of the call.
     */
    void SetStatus(CallStatus status);

    /**
     * @brief Get a pointer to a Node object
     * representing the origin node of this call.
     * @return pointer to an Node object.
     */
    Node* GetOrNode() const;
    /**
     * @brief Set a pointer to a Node object
     * representing the origin node of this Call.
     * @param orNode pointer to an Node object.
     */
    void SetOrNode(Node* orNode);
    /**
     * @brief Get a pointer to a Node object
     * representing the destination node of this Call.
     * @return pointer to an Node object.
     */
    Node* GetDeNode() const;
    /**
     * @brief Set a pointer to a Node object
     * representing the destination node of this Call.
     * @param deNode pointer to an Node object.
     */
    void SetDeNode(Node* deNode);
    
    unsigned int GetFirstSlot() const;

    void SetFirstSlot(unsigned int firstSlot);

    unsigned int GetLastSlot() const;

    void SetLastSlot(unsigned int lastSlot);
    /**
     * @brief Return the number of slots of this Call.
     * @return Total amount of slots.
     */
    unsigned int GetNumberSlots() const;
    /**
     * @brief Inputs the number of slots of this Call.
     * @param numberSlots Amount of slots to input.
     */
    void SetNumberSlots(unsigned int numberSlots);
    
    virtual unsigned int GetTotalNumSlots() const;
    
    virtual void SetTotalNumSlots();
    
    void SetTotalNumSlots(unsigned int numSlots);
    
    unsigned int GetCore() const;

    void SetCore(unsigned int core);

    /**
     * @brief Returns the value of OSNR of this Call.
     * @return OSNR value.
     */
    double GetOsnrTh() const;
    /**
     * @brief Inputs the value of OSNR of this Call.
     * @param osnrTh OSNR value.
     */
    void SetOsnrTh(double osnrTh);
    /**
     * @brief Returns the value of bandwidth of this Call.
     * @return Bandwidth value.
     */
    double GetBandwidth() const;
    /**
     * @brief Inputs the amount of bandwidth of this Call.
     * @param bandwidth Bandwidth value.
     */
    void SetBandwidth(double bandwidth);
    /**
     * @brief Return the bitrate value of this Call.
     * @return Bitrate value.
     */
    double GetBitRate() const;
    /**
     * @brief Inputs the bitrate value of this Call.
     * @param bitRate Bitrate value.
     */
    void SetBitRate(double bitRate); 
    
    TypeModulation GetModulation() const;

    void SetModulation(TypeModulation modulation);

    /**
     * @brief Returns the deactivation time of this Call.
     * @return Value of deactivation time.
     */
    TIME GetDeactivationTime() const;
    /**
     * @brief Inputs the deactivation time value.
     * @param deactivationTime Value of deactivation time.
     */
    void SetDeactivationTime(TIME deactivationTime);
    
    Route* GetRoute() const;
    
    std::shared_ptr<Route> GetRoute(unsigned int index) const;
    
    unsigned int GetNumRoutes() const;

    void SetRoute(std::shared_ptr<Route> route);

    void PushTrialRoute(std::shared_ptr<Route> route);
    
    void PushTrialRoutes(std::vector<std::shared_ptr<Route>> routes);
    
    std::shared_ptr<Route> PopTrialRoute();
    
    bool IsThereTrialRoute() const;
    
    void ClearTrialRoutes();
    
    void PushTrialModulations(std::vector<TypeModulation> modulations);
    
    void PushTrialModulation(TypeModulation modulation);
    
    TypeModulation PopTrialModulation();
    
    TypeModulation GetModulation(unsigned int index);
    
    void UpdateTrialModulations();
    
    void ClearTrialModulations();
private:
    /**
     * @brief Status of this Call.
     */
    CallStatus status;
    /**
     * @brief Pointer of a Node object
     * representing the origin node of this call.
     */
    Node* orNode;
    /**
     * @brief Pointer of a Node object
     * representing the destination node of this call.
     */
    Node* deNode;
    
    unsigned int firstSlot;
    
    unsigned int lastSlot;
    /**
     * @brief Number of slots occupied by this call.
     * This value is calculate based in modulation used 
     * and bitrate of this Call and the size of slots of 
     * the fibers.
     */
    unsigned int numberSlots;
    
    unsigned int totalNumSlots;
    
    unsigned int core;
    /**
     * @brief OSNr of the call calculated based 
     * in the distance of origin and destination node,
     * modulation used and the bitrate of this Call.
     */
    double osnrTh;
    /**
     * @brief Bandwidth occupied by this call, based 
     * in the bitrate and modulation used.
     */
    double bandwidth;
    /**
     * @brief Bitrate of this call.
     */
    double bitRate;
    
    TypeModulation modulation;
    
    std::deque<TypeModulation> trialModulation;
    /**
     * @brief Deactivation time of this Call.
     * Not used if the call is blocked.
     */
    TIME deactivationTime;
    
    std::shared_ptr<Route> route;
    
    std::deque<std::shared_ptr<Route>> trialRoutes;
    
    /**
     * @brief Map that keeps the Events options 
     * and the name of each one.
     */
    static const boost::unordered_map<CallStatus,
    std::string> mapCallStatus;
};

#endif /* CALL_H */

