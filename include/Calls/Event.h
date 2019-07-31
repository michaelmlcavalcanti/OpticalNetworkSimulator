/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Event.h
 * Author: bruno
 *
 * Created on August 19, 2018, 6:12 PM
 */

#ifndef EVENT_H
#define EVENT_H

class EventGenerator;
class Call;

#include <memory>
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>

#include "Call.h"

/**
 * @brief Numerate the event types.
 */
enum EventType{
    CallInvalid,
    CallRequest,
    CallEnd,
    FirsEventType = CallRequest,
    LastEventType = CallEnd
};

/**
 * @brief Class that represents an event of a simulation.
 */
class Event : public std::enable_shared_from_this<Event> {
    
    friend std::ostream& operator<<(std::ostream& ostream, 
    const Event* event);
public:
    /**
     * @brief Standard constructor of a simulator event.
     * @param generator Event generator of this event.
     * @param call Call request of this event.
     * @param time Simulation time for event occurrence.
     */
    Event(EventGenerator* generator, std::shared_ptr<Call> call, 
          TIME time);
    /**
     * @brief Standard destructor of an simulation event.
     */
    virtual ~Event();
    
    /**
     * @brief Get the type of this event.
     * @return the Type of event.
     */
    EventType GetEventType() const;
    /**
     * @brief Returns the type of event name.
     * @return String with the type of event name.
     */
    std::string GetEventName() const;
    /**
     * @brief Set the type of this event.
     * @param eventType type of event.
     */
    void SetEventType(EventType eventType);
    /**
     * @brief Returns the time this event must occur.
     * @return Time of this event.
     */
    TIME GetEventTime() const;
    /**
     * @brief Sets the time of the event.
     * @param eventTime Time of the event.
     */
    void SetEventTime(TIME eventTime);
    /**
     * @brief Returns a pointer to a Call object of this Event.
     * Not owns the object.
     * @return Pointer to a Call object.
     */
    Call* GetCall() const;
    /**
     * @brief Inputs the Call object in this Event.
     * @param call Pointer to a Call object
     */
    void SetCall(std::shared_ptr<Call> call);
    
    /**
     * @brief Function to implement an event, depending on the type.
     */
    void ImplementEvent();
private:
    /**
     * @brief Implements a Call request, resulting in a accepted or
     * blocked call.
     */
    void ImplementCallRequest();
    /**
     * @brief Release a implemented call in the network.
     */
    void ImplementCallEnd();
private:
    /**
     * @brief Pointer to a CallGenerator object that owns this object.
     */
    EventGenerator* parGenerator;
    /**
     * @brief Type of event.
     * The event can be a call request or a call end.
     */
    EventType eventType;
    /**
     * @brief Time of the event must occur.
     */
    TIME eventTime;
    /**
     * @brief Pointer to an Call object belong to this Event.
     */
    std::shared_ptr<Call> call;
    /**
     * @brief Map that keeps the Events options 
     * and the name of each one.
     */
    static const boost::unordered_map<EventType, 
    std::string> mapEventType;
};

#endif /* EVENT_H */

