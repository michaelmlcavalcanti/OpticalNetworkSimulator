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
    this->SetSubRoutes();
    this->SetSubRoutesNumRegSlotsMod();
}

void Resources::SetSubRoutes() {
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
    numReg.resize(sizeTraffic);
    numSlots.resize(sizeTraffic);
    subRoutesModulation.resize(sizeTraffic);
    
    //Loop for amount of traffics.
    for(unsigned trIndex = 0; trIndex < sizeTraffic; trIndex++){
        sizeNodes = subRoutes.size();
        numReg.at(trIndex).resize(sizeNodes);
        numSlots.at(trIndex).resize(sizeNodes);
        subRoutesModulation.at(trIndex).resize(sizeNodes);
        
        //Loop for all node pairs in network.
        for(unsigned nodePairIndex = 0; nodePairIndex < sizeNodes; nodePairIndex++){
            sizeRoutes = subRoutes.at(nodePairIndex).size();
            numReg.at(trIndex).at(nodePairIndex).resize(sizeRoutes);
            numSlots.at(trIndex).at(nodePairIndex).resize(sizeRoutes);
            subRoutesModulation.at(trIndex).at(nodePairIndex).resize(sizeRoutes);
            
            //Loop for the number of routes of each node pair.
            for(unsigned rouIndex = 0; rouIndex < sizeRoutes; rouIndex++){
                sizeSubRoutes = subRoutes.at(nodePairIndex).at(rouIndex).size();
                numReg.at(trIndex).at(nodePairIndex).at(rouIndex).resize(sizeSubRoutes);
                numSlots.at(trIndex).at(nodePairIndex).at(rouIndex).resize(sizeSubRoutes);
                subRoutesModulation.at(trIndex).at(nodePairIndex).at(rouIndex)
                                   .resize(sizeSubRoutes);
                
                //Loop for the number of sub-routes sets.
                for(unsigned numSubRoIndex = 0; numSubRoIndex < sizeSubRoutes; 
                numSubRoIndex++){
                    sizeSubRo = subRoutes.at(nodePairIndex).at(rouIndex).at(numSubRoIndex)
                                       .size();
                    numReg.at(trIndex).at(nodePairIndex).at(rouIndex)
                          .at(numSubRoIndex) = sizeSubRo - 1;
                    numSlots.at(trIndex).at(nodePairIndex).at(rouIndex)
                            .at(numSubRoIndex) = 0;
                    subRoutesModulation.at(trIndex).at(nodePairIndex).at(rouIndex)
                    .at(numSubRoIndex).resize(sizeSubRo, InvalidModulation);
                    
                    //Loop for all routes in this sub-route set.
                    for(unsigned subRouteIndex = 0; subRouteIndex < sizeSubRo;
                    subRouteIndex++){
                        this->function(trIndex, nodePairIndex, rouIndex, numSubRoIndex, 
                                       subRouteIndex);
                    }
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

void Resources::function(unsigned trIndex, unsigned nodeIndex, 
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
                    .at(numSubRoutesIndex) += (testCall->GetNumberSlots() * 
                    testCall->GetRoute()->GetNumHops());
            subRoutesModulation.at(trIndex).at(nodeIndex).at(routeIndex)
            .at(numSubRoutesIndex).at(subRouteIndex) = testCall->GetModulation();
            break;
        }
    }
}
 