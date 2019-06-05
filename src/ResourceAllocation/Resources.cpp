/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Resources.cpp
 * Author: brunovacorreia
 * 
 * Created on May 21, 2019, 7:32 PM
 */

#include "../../include/ResourceAllocation/Resources.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../include/Calls/Traffic.h"
#include "../../include/Calls/Call.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Structure/Node.h"
#include "../../include/Structure/Devices/Regenerator.h"
#include "../../include/GeneralClasses/Def.h"

Resources::Resources(ResourceAlloc* resourceAlloc, Modulation* modulation)
:allRoutes(0), interRoutes(0), resourceAllocOrder(0), numInterRoutesToCheck(0), 
numSlotsTraffic(0), subRoutes(0), numReg(0), numSlots(0), 
subRoutesModulation(0), resourceAlloc(resourceAlloc), modulation(modulation) {
    
}

Resources::~Resources() {
    
}

ResourceAlloc* Resources::GetResourceAlloc() const {
    return resourceAlloc;
}

void Resources::CreateRegResources() {
    this->SetRegSubRoutes();
    this->SetSubRoutesNumRegSlotsMod();
    this->RemoveInvalidRegOptions();
}

void Resources::CreateOfflineModulation() {
    unsigned int sizeTraffic = resourceAlloc->GetTraffic()->
                                              GetVecTraffic().size();
    unsigned int sizeNumNodes = this->allRoutes.size();
    unsigned int sizeRoutes;
    subRoutesModulation.resize(sizeTraffic);
    
    for(unsigned trIndex = 0; trIndex < sizeTraffic; trIndex++){
        subRoutesModulation.at(trIndex).resize(sizeNumNodes);
        
        for(unsigned nodeIndex = 0; nodeIndex < sizeNumNodes; nodeIndex++){
            sizeRoutes = allRoutes.at(nodeIndex).size();
            subRoutesModulation.at(trIndex).at(nodeIndex).resize(sizeRoutes);
            
            for(unsigned rouIndex = 0; rouIndex < sizeRoutes; rouIndex++){
                
                if(allRoutes.at(nodeIndex).at(rouIndex) == nullptr)
                    continue;
                subRoutesModulation.at(trIndex).at(nodeIndex).at(rouIndex)
                                   .resize(1);
                subRoutesModulation.at(trIndex).at(nodeIndex).at(rouIndex)
                                   .front().resize(1, InvalidModulation);
                
                this->TestBestModulation(trIndex, nodeIndex, rouIndex);
            }
        }
    }
}

std::vector<TypeModulation> Resources::GetModulationFormat(Call* call) {
    std::vector<TypeModulation> vecMod(0);
    unsigned trIndex = this->resourceAlloc->GetTraffic()->GetTrafficIndex(
                       call->GetBitRate());
    unsigned nodeIndex = (call->GetOrNode()->GetNodeId() * 
    resourceAlloc->GetTopology()->GetNumNodes()) + 
    call->GetDeNode()->GetNodeId();
    unsigned sizeRoute = subRoutesModulation.at(trIndex).at(nodeIndex).size();
    
    for(unsigned int routeIndex = 0; routeIndex < sizeRoute; routeIndex++){
        vecMod.push_back(subRoutesModulation.at(trIndex).at(nodeIndex)
        .at(routeIndex).front().front());
    }
    
    return vecMod;
}

std::vector<unsigned int> Resources::GetNumRegenerators(Call* call) {
    unsigned trIndex = resourceAlloc->GetTraffic()->GetTrafficIndex(call->
                       GetBitRate());
    unsigned nodeIndex = (call->GetOrNode()->GetNodeId() * resourceAlloc->
    GetTopology()->GetNumNodes()) + call->GetDeNode()->GetNodeId();
    unsigned routeIndex = this->GetRouteIndex(call->GetRoute(), call->
    GetOrNode()->GetNodeId(), call->GetDeNode()->GetNodeId());
    
    return numReg.at(trIndex).at(nodeIndex).at(routeIndex);
}

unsigned int Resources::GetNumRegenerators(Call* call, unsigned routeIndex, 
unsigned subRouteIndex) {
    unsigned trIndex = resourceAlloc->GetTraffic()->GetTrafficIndex(call->
                       GetBitRate());
    unsigned nodeIndex = (call->GetOrNode()->GetNodeId() * resourceAlloc->
    GetTopology()->GetNumNodes()) + call->GetDeNode()->GetNodeId();
    
    return numReg.at(trIndex).at(nodeIndex).at(routeIndex).at(subRouteIndex);
}

std::vector<std::vector<unsigned> > Resources::GetNumAllRegPos(Call* call) {
    unsigned trIndex = resourceAlloc->GetTraffic()->GetTrafficIndex(call->
                       GetBitRate());
    unsigned nodeIndex = (call->GetOrNode()->GetNodeId() * resourceAlloc->
    GetTopology()->GetNumNodes()) + call->GetDeNode()->GetNodeId();
    
    return numReg.at(trIndex).at(nodeIndex);
}

std::vector<unsigned int> Resources::GetNumSlotsRegenerators(Call* call) {
    unsigned trIndex = resourceAlloc->GetTraffic()->GetTrafficIndex(call->
                       GetBitRate());
    unsigned nodeIndex = (call->GetOrNode()->GetNodeId() * resourceAlloc->
    GetTopology()->GetNumNodes()) + call->GetDeNode()->GetNodeId();
    unsigned routeIndex = this->GetRouteIndex(call->GetRoute(), call->
    GetOrNode()->GetNodeId(), call->GetDeNode()->GetNodeId());
    
    return numSlots.at(trIndex).at(nodeIndex).at(routeIndex);
}

std::vector<std::vector<unsigned> > Resources::GetNumSlotsAllRegPos(Call* call) {
    unsigned trIndex = resourceAlloc->GetTraffic()->GetTrafficIndex(call->
                       GetBitRate());
    unsigned nodeIndex = (call->GetOrNode()->GetNodeId() * resourceAlloc->
    GetTopology()->GetNumNodes()) + call->GetDeNode()->GetNodeId();
    
    return numSlots.at(trIndex).at(nodeIndex);
}

unsigned int Resources::GetNumSlotsAllRegPos(Call* call, unsigned routeIndex, 
unsigned subRouteIndex) {
    unsigned trIndex = resourceAlloc->GetTraffic()->GetTrafficIndex(call->
                       GetBitRate());
    unsigned nodeIndex = (call->GetOrNode()->GetNodeId() * resourceAlloc->
    GetTopology()->GetNumNodes()) + call->GetDeNode()->GetNodeId();
    
    return numSlots.at(trIndex).at(nodeIndex).at(routeIndex).at(subRouteIndex);
}

std::vector<std::shared_ptr<Route>> Resources::GetVecSubRoute(Call* call, 
unsigned int auxIndex) {
    unsigned nodeIndex = (call->GetOrNode()->GetNodeId() * resourceAlloc->
    GetTopology()->GetNumNodes()) + call->GetDeNode()->GetNodeId();
    unsigned routeIndex = this->GetRouteIndex(call->GetRoute(), call->
    GetOrNode()->GetNodeId(), call->GetDeNode()->GetNodeId());
    
    return subRoutes.at(nodeIndex).at(routeIndex).at(auxIndex);
}

std::vector<std::shared_ptr<Route> > Resources::GetVecSubRoute(Call* call, 
unsigned int routeIndex, unsigned int subRouteIndex) {
    unsigned nodeIndex = (call->GetOrNode()->GetNodeId() * resourceAlloc->
    GetTopology()->GetNumNodes()) + call->GetDeNode()->GetNodeId();
    
    return subRoutes.at(nodeIndex).at(routeIndex).at(subRouteIndex);
}

std::vector<std::vector<std::vector<std::shared_ptr<Route> > > > 
Resources::GetVecSubRoute(Call* call) {
    unsigned nodeIndex = (call->GetOrNode()->GetNodeId() * resourceAlloc->
    GetTopology()->GetNumNodes()) + call->GetDeNode()->GetNodeId();
    
    return subRoutes.at(nodeIndex);
}

std::vector<TypeModulation> Resources::GetSubRoutesMod(Call* call, 
unsigned int auxIndex) {
    unsigned trIndex = resourceAlloc->GetTraffic()->GetTrafficIndex(call->
                       GetBitRate());
    unsigned nodeIndex = (call->GetOrNode()->GetNodeId() * resourceAlloc->
    GetTopology()->GetNumNodes()) + call->GetDeNode()->GetNodeId();
    unsigned routeIndex = this->GetRouteIndex(call->GetRoute(), call->
    GetOrNode()->GetNodeId(), call->GetDeNode()->GetNodeId());
    
    return subRoutesModulation.at(trIndex).at(nodeIndex).at(routeIndex)
                              .at(auxIndex);
}

void Resources::SetRegSubRoutes() {
    unsigned int sizeNumNodes = this->allRoutes.size();
    this->subRoutes.resize(sizeNumNodes*sizeNumNodes);
    unsigned int sizeNumRoutes;
    
    for(unsigned a = 0; a < sizeNumNodes; a++){
        sizeNumRoutes = this->allRoutes.at(a).size();
        this->subRoutes.at(a).resize(sizeNumRoutes);
        
        for(unsigned b = 0; b < sizeNumRoutes; b++){
            
            if(this->allRoutes.at(a).at(b) == nullptr)
                continue;
            
            this->MakeSubRoutes(this->allRoutes.at(a).at(b), 0, a, b);
            this->subRoutes.at(a).at(b).pop_back();
        }
    }
}

void Resources::SetSubRoutesNumRegSlotsMod() {
    unsigned int sizeTraffic = resourceAlloc->GetTraffic()->
                                              GetVecTraffic().size();
    unsigned int sizeNodes, sizeRoutes, sizeSubRoutes, sizeSubRo;
    double auxTraffic, multiplier;
    numReg.resize(sizeTraffic);
    numSlots.resize(sizeTraffic);
    subRoutesModulation.resize(sizeTraffic);
    
    //Loop for amount of traffics.
    for(unsigned trIndex = 0; trIndex < sizeTraffic; trIndex++){
        auxTraffic = resourceAlloc->GetTraffic()->GetTraffic(trIndex);
        multiplier = std::ceil(auxTraffic / Regenerator::GetTrafficSupported());
        sizeNodes = subRoutes.size();
        numReg.at(trIndex).resize(sizeNodes);
        numSlots.at(trIndex).resize(sizeNodes);
        subRoutesModulation.at(trIndex).resize(sizeNodes);
        
        //Loop for all node pairs in network.
        for(unsigned nodePairIndex = 0; nodePairIndex < sizeNodes; 
        nodePairIndex++){
            sizeRoutes = subRoutes.at(nodePairIndex).size();
            numReg.at(trIndex).at(nodePairIndex).resize(sizeRoutes);
            numSlots.at(trIndex).at(nodePairIndex).resize(sizeRoutes);
            subRoutesModulation.at(trIndex).at(nodePairIndex)
                               .resize(sizeRoutes);
            
            //Loop for the number of routes of each node pair.
            for(unsigned rouIndex = 0; rouIndex < sizeRoutes; rouIndex++){
                sizeSubRoutes = subRoutes.at(nodePairIndex).at(rouIndex).size();
                numReg.at(trIndex).at(nodePairIndex).at(rouIndex)
                      .resize(sizeSubRoutes);
                numSlots.at(trIndex).at(nodePairIndex).at(rouIndex)
                        .resize(sizeSubRoutes);
                subRoutesModulation.at(trIndex).at(nodePairIndex).at(rouIndex)
                                   .resize(sizeSubRoutes);
                
                //Loop for the number of sub-routes sets.
                for(unsigned numSubRoIndex = 0; numSubRoIndex < sizeSubRoutes; 
                numSubRoIndex++){
                    sizeSubRo = subRoutes.at(nodePairIndex).at(rouIndex)
                                         .at(numSubRoIndex).size();
                    numReg.at(trIndex).at(nodePairIndex).at(rouIndex)
                          .at(numSubRoIndex) = (sizeSubRo - 1) * multiplier;
                    numSlots.at(trIndex).at(nodePairIndex).at(rouIndex)
                            .at(numSubRoIndex) = 0;
                    subRoutesModulation.at(trIndex).at(nodePairIndex)
                                       .at(rouIndex).at(numSubRoIndex)
                                       .resize(sizeSubRo, InvalidModulation);
                    
                    //Loop for all transparent segments that compose the route.
                    for(unsigned subRouteIndex = 0; subRouteIndex < sizeSubRo;
                    subRouteIndex++)
                        this->TestBestModulation(trIndex, nodePairIndex,
                        rouIndex, numSubRoIndex, subRouteIndex);
                }
            }
        }
    }
}

void Resources::MakeSubRoutes(std::shared_ptr<Route> totalRoute, 
unsigned int curNode, unsigned index1, unsigned int index2) {
    std::shared_ptr<Route> auxRoute;
    
    for(unsigned int a = curNode; a < totalRoute->GetNumHops(); a++){
        auxRoute = totalRoute->CreatePartialRoute(curNode, a+1);
        std::vector<std::shared_ptr<Route>> auxVecRoute(0);
        auxVecRoute.push_back(auxRoute);
        
        if(this->subRoutes.at(index1).at(index2).empty())
            this->subRoutes.at(index1).at(index2)
                                      .push_back(auxVecRoute);
        else
            this->subRoutes.at(index1).at(index2).back()
                                      .push_back(auxRoute);
        
        if(a+1 < totalRoute->GetNumHops())
            this->MakeSubRoutes(totalRoute, a+1, index1, index2);
        else{
            auxVecRoute = this->subRoutes.at(index1).at(index2)
                                                    .back();
            this->subRoutes.at(index1).at(index2)
                                      .push_back(auxVecRoute);
        }
        this->subRoutes.at(index1).at(index2).back().pop_back();
    }
}

void Resources::TestBestModulation(unsigned trIndex, unsigned nodeIndex, 
unsigned routeIndex, unsigned numSubRoutesIndex, unsigned subRouteIndex) {
    std::shared_ptr<Route> route = subRoutes.at(nodeIndex).at(routeIndex)
    .at(numSubRoutesIndex).at(subRouteIndex);
    double bitRate = this->resourceAlloc->GetTraffic()->GetTraffic(trIndex);
    
    std::shared_ptr<Call> testCall = std::make_shared<Call>(route->GetOrNode(),
    route->GetDeNode(), bitRate, 0.0);
    testCall->SetRoute(route);
    
    for(TypeModulation mod = LastModulation; mod >= FirstModulation; 
    mod = TypeModulation(mod-1)){
        testCall->SetModulation(mod);
        modulation->SetModulationParam(testCall.get());
        
        if(resourceAlloc->CheckOSNR(testCall.get())){
            numSlots.at(trIndex).at(nodeIndex).at(routeIndex)
                    .at(numSubRoutesIndex) += (testCall->GetTotalNumSlots());
            subRoutesModulation.at(trIndex).at(nodeIndex).at(routeIndex)
            .at(numSubRoutesIndex).at(subRouteIndex) = testCall->GetModulation();
            break;
        }
    }
}

void Resources::TestBestModulation(unsigned trIndex, unsigned nodeIndex, 
unsigned routeIndex) {
    std::shared_ptr<Route> route = allRoutes.at(nodeIndex).at(routeIndex);
    double bitRate = this->resourceAlloc->GetTraffic()->GetTraffic(trIndex);
    
    std::shared_ptr<Call> testCall = std::make_shared<Call>(route->GetOrNode(),
    route->GetDeNode(), bitRate, 0.0);
    testCall->SetRoute(route);
    
    for(TypeModulation mod = LastModulation; mod >= FirstModulation; 
    mod = TypeModulation(mod-1)){
        testCall->SetModulation(mod);
        modulation->SetModulationParam(testCall.get());
        
        if(resourceAlloc->CheckOSNR(testCall.get())){
            subRoutesModulation.at(trIndex).at(nodeIndex).at(routeIndex).front()
                               .front() = testCall->GetModulation();
            break;
        }
    }
}

void Resources::RemoveInvalidRegOptions() {
    unsigned int trSize, nodeSize, routeSize, subRouteSize, segSize;
    TypeModulation auxMod;
    unsigned int index;
    std::vector<unsigned int> invalidIndexes(0);
    
    trSize = subRoutesModulation.size();
    
    for(unsigned trIndex = 0; trIndex < trSize; trIndex++){
        nodeSize = subRoutesModulation.at(trIndex).size();
        
        for(unsigned nodeIndex = 0; nodeIndex < nodeSize; nodeIndex++){
            routeSize = subRoutesModulation.at(trIndex).at(nodeIndex).size();
            
            for(unsigned routeIndex = 0; routeIndex < routeSize; routeIndex++){
                subRouteSize = subRoutesModulation.at(trIndex).at(nodeIndex)
                                                  .at(routeIndex).size();
                
                for(unsigned subIndex = 0; subIndex < subRouteSize; subIndex++){
                    segSize = subRoutesModulation.at(trIndex).at(nodeIndex)
                              .at(routeIndex).at(subIndex).size();
                    
                    for(unsigned segIndex = 0; segIndex < segSize; segIndex++){
                        auxMod = subRoutesModulation.at(trIndex).at(nodeIndex)
                                 .at(routeIndex).at(subIndex).at(segIndex);
                        
                        if(auxMod == InvalidModulation){
                            invalidIndexes.push_back(subIndex);
                            break;
                        }
                    }
                }
                
                while(!invalidIndexes.empty()){
                    index = invalidIndexes.back();
                    invalidIndexes.pop_back();
                    
                    numReg.at(trIndex).at(nodeIndex).at(routeIndex).erase(
                    numReg.at(trIndex).at(nodeIndex).at(routeIndex).begin() + 
                    index);
                    
                    numSlots.at(trIndex).at(nodeIndex).at(routeIndex).erase(
                    numSlots.at(trIndex).at(nodeIndex).at(routeIndex).begin() + 
                    index);
                    
                    subRoutesModulation.at(trIndex).at(nodeIndex).at(routeIndex)
                    .erase(subRoutesModulation.at(trIndex).at(nodeIndex)
                    .at(routeIndex).begin() + index);
                }
                invalidIndexes.clear();
            }
        }
    }
}

unsigned Resources::GetRouteIndex(Route* route, unsigned orNode, 
unsigned deNode) {
    unsigned numNodes = this->resourceAlloc->GetTopology()->GetNumNodes();
    unsigned sizeRoutes = allRoutes.at(orNode*numNodes + deNode).size();
    
    for(unsigned a = 0; a < sizeRoutes; a++){
        
        if(route == allRoutes.at(orNode*numNodes + deNode).at(a).get())
            return a;
    }
    
    return Def::Max_UnInt;
}
