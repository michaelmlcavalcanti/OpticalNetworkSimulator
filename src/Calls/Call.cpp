/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Call.cpp
 * Author: bruno
 * 
 * Created on August 17, 2018, 11:40 PM
 */

#include "../../include/Calls/Call.h"
#include "../../include/Structure/Node.h"
#include "../../include/GeneralClasses/Def.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/ResourceAllocation/Resources.h"
#include "../../include/Structure/Topology.h"
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/ResourceAllocation/ResourceAlloc.h"

const boost::unordered_map<CallStatus, std::string> 
Call::mapCallStatus = boost::assign::map_list_of
    (NotEvaluated, "Not evaluated")
    (Accepted, "Accepted call")
    (Blocked, "Blocked call");

std::ostream& operator<<(std::ostream& ostream, const Call* call) {
    
    ostream << "Status: " << call->GetStatusName() << std::endl;
    ostream << "OrNode: " << call->GetOrNode()->GetNodeId() << std::endl;
    ostream << "DeNode: " << call->GetDeNode()->GetNodeId() << std::endl;
    ostream << "Bit Rate(Gbps): " << call->GetBitRate()/1E9 << std::endl;
    ostream << "Bandwidth: " << call->GetBandwidth()/1E9 << std::endl;
    ostream << "OSNR: " << call->GetOsnrTh() << std::endl;
    ostream << "DeacTime: " << call->GetDeactivationTime() << std::endl;
    ostream << "Number of Slots: " << call->GetNumberSlots() << std::endl;
    
    return ostream;
}

Call::Call(Node* orNode, Node* deNode, double bitRate, TIME deacTime)
:status(NotEvaluated), orNode(orNode), deNode(deNode), 
firstSlot(Def::Max_UnInt), lastSlot(Def::Max_UnInt), numberSlots(0), 
totalNumSlots(0), core(Def::Max_UnInt), osnrTh(0.0), bandwidth(0.0), 
bitRate(bitRate), modulation(InvalidModulation), trialModulation(0), 
deactivationTime(deacTime), route(nullptr), trialRoutes(0), trialProtRoutes(0) {
    resources = orNode->GetTopology()->GetSimulType()->GetResourceAlloc()->
    GetResources();
}

Call::~Call() {
    this->route.reset();
    
    for(auto it : trialRoutes){
       it.reset(); 
    }
    this->trialRoutes.clear();
    
   // for(auto it : trialProtRoutes){
   //    it.reset(); 
   // }
    this->trialProtRoutes.clear();
    
}

CallStatus Call::GetStatus() const {
    return this->status;
}

std::string Call::GetStatusName() const {
    return this->mapCallStatus.at(this->status);
}

void Call::SetStatus(CallStatus status) {
    this->status = status;
}

Node* Call::GetOrNode() const {
    return this->orNode;
}

void Call::SetOrNode(Node* orNode) {
    this->orNode = orNode;
}

Node* Call::GetDeNode() const {
    return this->deNode;
}

void Call::SetDeNode(Node* deNode) {
    this->deNode = deNode;
}

unsigned int Call::GetFirstSlot() const {
    return firstSlot;
}

void Call::SetFirstSlot(unsigned int firstSlot) {
    this->firstSlot = firstSlot;
}

unsigned int Call::GetLastSlot() const {
    return lastSlot;
}

void Call::SetLastSlot(unsigned int lastSlot) {
    this->lastSlot = lastSlot;
}

unsigned int Call::GetNumberSlots() const {
    return this->numberSlots;
}

void Call::SetNumberSlots(unsigned int numberSlots) {
    assert(numberSlots > 0);
    this->numberSlots = numberSlots;
}

unsigned int Call::GetTotalNumSlots() const {
    return totalNumSlots;
}

void Call::SetTotalNumSlots() {
    totalNumSlots = numberSlots * route->GetNumHops();
}

void Call::SetTotalNumSlots(unsigned int numSlots) {
    assert(numSlots > 0);
    this->totalNumSlots = numSlots;
}

unsigned int Call::GetCore() const {
    return core;
}

void Call::SetCore(unsigned int core) {
    this->core = core;
}

double Call::GetOsnrTh() const {
    return osnrTh;
}

void Call::SetOsnrTh(double osnrTh) {
    this->osnrTh = osnrTh;
}

double Call::GetBandwidth() const {
    return bandwidth;
}

void Call::SetBandwidth(double bandwidth) {
    this->bandwidth = bandwidth;
}

double Call::GetBitRate() const {
    return bitRate;
}

void Call::SetBitRate(double bitRate) {
    this->bitRate = bitRate;
}

void Call::SetModulation(TypeModulation modulation) {
    assert(modulation >= FirstModulation && 
           modulation <= LastModulation);
    
    this->modulation = modulation;
}

TypeModulation Call::GetModulation() const {
    return modulation;
}

TIME Call::GetDeactivationTime() const {
    return deactivationTime;
}

void Call::SetDeactivationTime(TIME deactivationTime) {
    this->deactivationTime = deactivationTime;
}

Route* Call::GetRoute() const {
    return this->route.get();
}

std::shared_ptr<Route> Call::GetRoute(unsigned int index) const {
    assert(index < this->trialRoutes.size());
    
    return this->trialRoutes.at(index);
}

std::shared_ptr<Route> Call::GetProtRoute(unsigned int routeIndex, 
unsigned int protRouteIndex) const {
    assert(routeIndex < this->trialRoutes.size());
    assert(protRouteIndex < this->trialProtRoutes.at(routeIndex).size());
    
    return this->trialProtRoutes.at(routeIndex).at(protRouteIndex); 
}

std::deque<std::shared_ptr<Route> > Call::GetProtRoutes(unsigned int routeIndex) 
const {
    assert(routeIndex < this->trialRoutes.size());
    
    return this->trialProtRoutes.at(routeIndex);
}

unsigned int Call::GetNumRoutes() const {
    return this->trialRoutes.size();
}

void Call::SetRoute(std::shared_ptr<Route> route) {
    this->route = route;
}

void Call::SetRoute(unsigned int routeIndex) {
    assert(routeIndex < trialRoutes.size());
    this->route = trialRoutes.at(routeIndex);
}

void Call::PushTrialRoute(std::shared_ptr<Route> route) {
    this->trialRoutes.push_back(route);
}

void Call::PushTrialRoutes(std::vector<std::shared_ptr<Route> > routes) {
    
    for(auto it : routes)
        if(it != nullptr)
            this->trialRoutes.push_back(it);
    routes.clear();
}

void Call::PushTrialProtRoutes(std::vector<std::shared_ptr<Route>> routes) {
    NodeIndex orNode = this->GetOrNode()->GetNodeId();
    NodeIndex deNode = this->GetDeNode()->GetNodeId();
    std::vector<std::shared_ptr<Route>> protRoutes;
    unsigned int numRoutes = routes.size();
    this->trialProtRoutes.resize(numRoutes);
    
    for(unsigned int a = 0; a < routes.size(); a++){
        protRoutes = resources->GetProtRoutes(orNode, deNode, a);
        
        for(auto it : protRoutes)
            this->trialProtRoutes.at(a).push_back(it);
                     
    }
    routes.clear();
}

void Call::ClearTrialRoutes() {
    
    while(!this->trialRoutes.empty()){
        this->trialRoutes.front().reset();
        this->trialRoutes.pop_front();
    }
}

void Call::ClearTrialProtRoutes() {
    for(unsigned int a = 0; a < trialProtRoutes.size(); a++){
        
        while(!this->trialProtRoutes.at(a).empty()){
            this->trialProtRoutes.at(a).front().reset();
            this->trialProtRoutes.at(a).pop_front();
        }
    }
}


void Call::PushTrialModulations(std::vector<TypeModulation> modulations) {
    
    for(auto it: modulations){
        this->trialModulation.push_back(it);
    }
}

void Call::PushTrialModulation(TypeModulation modulation) {
    trialModulation.push_back(modulation);
}

void Call::RepeatModulation() {
    
    while(trialModulation.size() < trialRoutes.size()){
        trialModulation.push_back(trialModulation.front());
    }
}

void Call::ClearTrialModulations() {
    this->trialModulation.clear();
}

TypeModulation Call::GetModulation(unsigned int index) {
    return this->trialModulation.at(index);
}
