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

Resources::Resources(ResourceAlloc* resourceAlloc)
:allRoutes(0), interRoutes(0), resourceAllocOrder(0), numInterRoutesToCheck(0), 
numSlotsTraffic(0), subRoutes(0), numReg(0), numSlots(0), 
subRoutesModulation(0), resourceAlloc(resourceAlloc) {
    
}

Resources::~Resources() {
    
}

ResourceAlloc* Resources::GetResourceAlloc() const {
    return resourceAlloc;
}

void Resources::CreateRegResources() {
    this->SetSubRoutes();
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
    unsigned int auxSize;
    double auxBitRate;
    numReg.resize(sizeTraffic);
    numSlots.resize(sizeTraffic);
    subRoutesModulation.resize(sizeTraffic);
    
    //Loop for amount of traffics.
    for(unsigned trSize = 0; trSize < sizeTraffic; trSize++){
        auxBitRate = resourceAlloc->GetTraffic()->GetTraffic(trSize);
        auxSize = allRoutes.size();
        numReg.at(trSize).resize(auxSize);
        numSlots.at(trSize).resize(auxSize);
        subRoutesModulation.at(trSize).resize(auxSize);
        
        //Loop for all node pairs in network.
        for(unsigned nodeSize = 0; nodeSize < allRoutes.size(); nodeSize++){
            auxSize = allRoutes.at(nodeSize).size();
            numReg.at(trSize).at(nodeSize).resize(auxSize);
            numSlots.at(trSize).at(nodeSize).resize(auxSize);
            subRoutesModulation.at(trSize).at(nodeSize).resize(auxSize);
            
            for(unsigned rouSize = 0; rouSize < allRoutes.at(nodeSize).size();
            rouSize++){
                auxSize = subRoutes.at(nodeSize).at(rouSize).size();
                
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
