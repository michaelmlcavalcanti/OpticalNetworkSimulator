/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Event.cpp
 * Author: bruno
 * 
 * Created on August 19, 2018, 6:12 PM
 */

#include "../../include/Calls/Event.h"
#include "../../include/Calls/CallGenerator.h"
#include "../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../include/Data/Data.h"
#include "../../include/Structure/Topology.h"
#include "../../include/SimulationType/SimulationType.h"

const boost::unordered_map<EventType, std::string> 
Event::mapEventType = boost::assign::map_list_of
    (CallInvalid, "Invalid")
    (CallRequest, "Call request")
    (CallEnd, "Call end");

std::ostream& operator<<(std::ostream& ostream, const Event* event){
    
    ostream << "Event" << std::endl;
    ostream << "Time: " << event->GetEventTime() << std::endl;
    ostream << "Type: " << event->GetEventName() << std::endl << std::endl;
    
    if(event->GetCall() != nullptr)
        ostream << event->GetCall() << std::endl;
    
    return ostream;
}

Event::Event(CallGenerator* generator, std::shared_ptr<Call> call, TIME time)
:parGenerator(generator), eventType(CallRequest), eventTime(time),
call(call) {

}

Event::~Event() {
    this->call.reset();
}

EventType Event::GetEventType() const {
    return eventType;
}

std::string Event::GetEventName() const {
    return mapEventType.at(this->eventType);
}

void Event::SetEventType(EventType eventType) {
    assert(eventType >= FirsEventType && eventType <= LastEventType);
    this->eventType = eventType;
}

TIME Event::GetEventTime() const {
    return eventTime;
}

void Event::SetEventTime(TIME eventTime) {
    assert(this->eventTime <= eventTime);
    this->eventTime = eventTime;
}

Call* Event::GetCall() const {
    return this->call.get();
}

void Event::SetCall(std::shared_ptr<Call> call) {
    this->call = call;
}

void Event::ImplementEvent() {
    
    switch(this->eventType){
        case CallRequest:
            this->ImplementCallRequest();
            this->parGenerator->GenerateCall();
            break;
        case CallEnd:
            this->ImplementCallEnd();
            break;
        default:
            std::cerr << "Invalid event" << std::endl;
            std::abort();
    }
}

void Event::ImplementCallRequest() {
    parGenerator->GetSimulType()->numberRequests++;
    //Function to try call allocation.
    this->parGenerator->GetResourceAlloc()->ResourAlloc(this->call.get());
    
    //Function to update data values
    this->parGenerator->GetData()->StorageCall(this->call.get());

    switch(this->call->GetStatus()){
        case Accepted:
            this->parGenerator->GetTopology()->Connect(this->call.get());
            this->SetEventType(CallEnd);
            this->SetEventTime(this->parGenerator->GetSimulationTime() + 
                               this->call->GetDeactivationTime());
            this->parGenerator->PushEvent(shared_from_this());
            break;
        case Blocked:
            call.reset();
            break;
        default:
            std::cerr << "Invalid Call status" << std::endl;
            std::abort();
    }
}

void Event::ImplementCallEnd() {
    assert(this->call->GetStatus() == Accepted);
    
    this->parGenerator->GetTopology()->Release(this->call.get());
}
