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
class Resources;

#include "../Structure/Link.h"
#include "EventGenerator.h"
#include "../ResourceAllocation/Modulation.h"

/**
 * @brief Lists the possible status of a Call.
 */
enum CallStatus{
    NotEvaluated,
    Accepted,
    Blocked
};

/**
 * @brief Class that represents a simulation call request.
 */
class Call {
    
    friend std::ostream& operator<<(std::ostream& ostream, 
    const Call* call);    
public:
    /**
     * @brief Standard constructor of a Call object.
     * @param orNode call origin node.
     * @param deNode call destination node.
     * @param bitRate call bit rate.
     */
    Call(Node* orNode, Node* deNode, double bitRate, TIME deacTime);
    /**
     * @brief Standard destructor of a simulation call request.
     */
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
    /**
     * @brief Function to get the first contiguous slot of the call request.
     * @return Call request first slot.
     */
    unsigned int GetFirstSlot() const;
    /**
     * @brief Function to set the first contiguous slot of the call request.
     * @param firstSlot Call request first slot.
     */
    void SetFirstSlot(unsigned int firstSlot);
    /**
     * @brief Function to get the last contiguous slot of the call request.
     * @return Call request last slot.
     */
    unsigned int GetLastSlot() const;
    /**
     * @brief Function to set the last contiguous slot of the call request.
     * @param lastSlot Call request last slot.
     */
    void SetLastSlot(unsigned int lastSlot);
    /**
     * @brief Function to reset the first and the last contiguous slot of the 
     * call request to its default values.
     * @param Call request.
     */
    void ResetFirstLastSlot(Call* call);
    /**
     * @brief Function to get the number of contiguous slots of the call 
     * request.
     * @return Number of contiguous slots.
     */
    unsigned int GetNumberSlots() const;
    /**
     * @brief Function to set the number of contiguous slots of the call 
     * request.
     * @param numberSlots Number of contiguous slots.
     */
    void SetNumberSlots(unsigned int numberSlots);
    /**
     * @brief Function to get the total number of slots (in the entire route) 
     * occupied by the call request.
     * @return Total number of slots.
     */
    virtual unsigned int GetTotalNumSlots() const;
    /**
     * @brief Function to calculate the total number of slots (in the entire 
     * route) occupied by the call request.
     */
    virtual void SetTotalNumSlots();
    /**
     * @brief Function to set the total number of slots (in the entire 
     * route) occupied by the call request.
     * @param numSlots Total number of slots
     */
    void SetTotalNumSlots(unsigned int numSlots);
    /**
     * @brief Function to get the core index used by the call request.
     * @return Core index.
     */
    unsigned int GetCore() const;
    /**
     * @brief Function to set the core index used by the call request.
     * @param core Core index.
     */
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
    /**
     * @brief Function to get the modulation format used by the call request.
     * @return Call request modulation format.
     */
    TypeModulation GetModulation() const;
    /**
     * @brief Function to set the modulation format used by the call request.
     * @param modulation Call request modulation format.
     */
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
    /**
     * @brief Function to get the route allocated for the call request.
     * @return Call request route.
     */
    Route* GetRoute() const;
    /**
     * @brief Function to get a specified route of the possible routes that
     * can allocate the call request.
     * @param index Route index.
     * @return Call request possible route.
     */
    std::shared_ptr<Route> GetRoute(unsigned int index) const;
    /**
     * @brief Function to get a specified protection route of the possible 
     * protection routes of a specified working route.
     * @param routeIndex working route index, protRouteIndex protection route
     * index.
     * @return Call request possible route.
     */
    std::shared_ptr<Route> GetProtRoute(unsigned int routeIndex , 
    unsigned int protRouteIndex) const;
    
    std::deque<std::shared_ptr<Route>> GetProtRoutes(unsigned int routeIndex) 
    const;
    
    /**
     * @brief Function to get the number of routes the call can be allocated.
     * @return Number of routes.
     */
    unsigned int GetNumRoutes() const;
    /**
     * @brief Function to set a route to allocate the call request.
     * @param route Call request route.
     */
    void SetRoute(std::shared_ptr<Route> route);
    /**
     * @brief Function to set a route to allocate the call request from the set
     * of possible routes.
     * @param routeIndex Route index.
     */
    void SetRoute(unsigned int routeIndex);
    /**
     * @brief Function to add a route to the container of possible routes to 
     * allocate the call request.
     * @param route Call request possible route.
     */
    void PushTrialRoute(std::shared_ptr<Route> route);
    /**
     * @brief Function to set a container of possible routes to allocate the
     * call request.
     * @param routes Call request possible routes.
     */
    void PushTrialRoutes(std::vector<std::shared_ptr<Route>> routes);
    /**
     * @brief Function to set a container of possible protection routes for  
     * each working route to allocate the call request.
     * @param protection routes Call request possible routes.
     */
    void PushTrialProtRoutes(std::vector<std::shared_ptr<Route>> routes);  
    /**
     * @brief Function to clear the container of possible routes to allocate
     * the call request.
     */
    void ClearTrialRoutes();
    /**
     * @brief Function to clear the container of possible protection routes 
     * to allocate the call request.
     */
    void ClearTrialProtRoutes();
    /**
     * @brief Function to set the modulation format container of possible
     * modulation formats of the call request.
     * @param modulations Modulation format container.
     */
    void PushTrialModulations(std::vector<TypeModulation> modulations);
    /**
     * @brief Function to add a modulation format to the container of possible
     * modulation formats of the call requests.
     * @param modulation Modulation format to add.
     */
    void PushTrialModulation(TypeModulation modulation);
    /**
     * @brief Function to repeat the first trial modulation in the trial
     * modulation container, in order to keep this container with the same
     * size of the trial route container.
     */
    void RepeatModulation();
    /**
     * @brief Function to get a specified modulation format from the container 
     * of possible modulation formats.
     * @param index Modulation format index.
     * @return Modulation format.
     */
    TypeModulation GetModulation(unsigned int index);
    /**
     * @brief Function to clear the container of possible modulation formats.
     */
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
    /**
     * @brief First contiguous slot of the call request. If the request is not
     * allocated, this value is the maximum unsigned integer.
     */
    SlotIndex firstSlot;
    /**
     * @brief Last contiguous slot of the call request. If the request is not
     * allocated, this value is the maximum unsigned integer.
     */
    SlotIndex lastSlot;
    /**
     * @brief Number of slots occupied by this call.
     * This value is calculate based in modulation used 
     * and bitrate of this Call and the size of slots of 
     * the fibers.
     */
    unsigned int numberSlots;
    /**
     * @brief Total number of slots occupied by the call request. This value 
     * is calculated taking into account the entire route the call is allocated.
     */
    unsigned int totalNumSlots;
    /**
     * @brief Core index of the call request. If the call request is not 
     * allocated or if the simulation presents only one core, this value is
     * the maximum  integer possible value.
     */
    CoreIndex core;
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
    /**
     * @brief Modulation format used by the call request.
     */
    TypeModulation modulation;
    /**
     * @brief Container of possible modulation formats the call request can use.
     * The size of the container has to be the same size of the routes
     * container.
     */
    std::deque<TypeModulation> trialModulation;
    /**
     * @brief Deactivation time of this Call.
     * Not used if the call is blocked.
     */
    TIME deactivationTime;
    /**
     * @brief Route in which the call request is allocated.
     */
    std::shared_ptr<Route> route;
    /**
     * @brief Container of possible routes to allocate the call request.
     */
    std::deque<std::shared_ptr<Route>> trialRoutes;
    /**
     * @brief Container of possible protection routes to each working route to  
     * allocate the call request.
     */
    std::deque<std::deque<std::shared_ptr<Route>>> trialProtRoutes;
    Resources* resources;
    
    /**
     * @brief Map that keeps the Events options 
     * and the name of each one.
     */
    static const boost::unordered_map<CallStatus,
    std::string> mapCallStatus;
    
};

#endif /* CALL_H */

