/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FirstLongestReach.cpp
 * Author: brunovacorreia
 * 
 * Created on August 25, 2019, 8:04 PM
 */

#include "../../../include/ResourceAllocation/RegeneratorAssignment/FirstLongestReach.h"

FirstLongestReach::FirstLongestReach(ResourceDeviceAlloc* resDevAlloc)
:RegeneratorAssignment(resDevAlloc) {
    
}

FirstLongestReach::~FirstLongestReach() {
    
}

void FirstLongestReach::ResourceAlloc(CallDevices* call) {
    if(resDevAlloc->options->GetRegAssOption() == RegAssFLR)
        resDevAlloc->RoutingOffVirtRegSpecAlloc(call);
    else if(resDevAlloc->options->GetRegAssOption() == RegAssFLRonline)
        resDevAlloc->RoutingOnVirtRegSpecAlloc(call);
}

void FirstLongestReach::OrderRegenerationOptions(CallDevices* call, 
std::vector<std::tuple<unsigned, unsigned> >& vec) {
    std::vector<std::vector<std::vector<std::shared_ptr<Route>>>> vecRoutes =
    resDevAlloc->resources->GetRoutesTranspSegments(call);
    
    unsigned int sizeRoutes = vecRoutes.size();
    unsigned int auxRegOpIndex;
    
    for(unsigned routeIndex = 0; routeIndex < sizeRoutes; routeIndex++){
        
        while(!vecRoutes.at(routeIndex).empty()){
            auxRegOpIndex = vecRoutes.at(routeIndex).size() - 1;
            vec.push_back(std::make_tuple(routeIndex, auxRegOpIndex));
            vecRoutes.at(routeIndex).pop_back();
        }
    }
}

bool FirstLongestReach::CreateRegOption(CallDevices* call, unsigned routeInd, 
std::vector<std::shared_ptr<Route> >& routes, 
std::vector<TypeModulation>& modulations) {
    std::shared_ptr<Route> orRoute = call->GetRoute(routeInd);
    unsigned numRouteNodes = orRoute->GetNumNodes();
    double bitRate = call->GetBitRate();
    std::vector<NodeDevices*> nodes(0);
    std::shared_ptr<Route> auxRoute;
    unsigned numReg = NodeDevices::GetNumRegRequired(call->GetBitRate());
    
    for(unsigned a = 0; a < numRouteNodes; a++)
        nodes.push_back(dynamic_cast<NodeDevices*>(orRoute->GetNode(a)));
    
    unsigned curNodeIndex = 0;
    
    for(unsigned sourIndex = 0; sourIndex < numRouteNodes; sourIndex++){
        for(unsigned testIndex = sourIndex + 1; testIndex < numRouteNodes; 
        testIndex++){
            auxRoute = orRoute->CreatePartialRoute(sourIndex, testIndex);
            
            if((nodes.at(testIndex)->GetNumFreeRegenerators() >= numReg) ||
            auxRoute->GetDeNode() == orRoute->GetDeNode()){
                
                if(this->CheckSpectrumAndOSNR(bitRate, auxRoute.get())){
                    
                    if(auxRoute->GetDeNode() == orRoute->GetDeNode()){
                        routes.push_back(auxRoute);
                        modulations.push_back(this->GetBestModulation(bitRate,
                        auxRoute.get()));
                        return true;
                    }
                    else
                        curNodeIndex = testIndex;
                }
                else{
                    if(curNodeIndex != sourIndex){
                        auxRoute = orRoute->CreatePartialRoute(sourIndex, 
                        curNodeIndex);
                        routes.push_back(auxRoute);
                        modulations.push_back(this->GetBestModulation(bitRate,
                        auxRoute.get()));
                        sourIndex = curNodeIndex;
                        testIndex = curNodeIndex;
                    }
                    else{
                        routes.clear();
                        modulations.clear();
                        return false;
                    }
                }
            }
        }
    }
    
    routes.clear();
    modulations.clear();
    return false;
}
