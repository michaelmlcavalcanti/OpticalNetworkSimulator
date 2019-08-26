/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FirstNarrowestSpectrum.cpp
 * Author: brunovacorreia
 * 
 * Created on August 25, 2019, 11:38 PM
 */

#include "../../../include/ResourceAllocation/RegeneratorAssignment/FirstNarrowestSpectrum.h"

FirstNarrowestSpectrum::FirstNarrowestSpectrum(ResourceDeviceAlloc* resDevAlloc)
:RegeneratorAssignment(resDevAlloc) {

}

FirstNarrowestSpectrum::~FirstNarrowestSpectrum() {

}

void FirstNarrowestSpectrum::ResourceAlloc(CallDevices* call) {
    resDevAlloc->RoutingOnVirtRegSpecAlloc(call);
}

bool FirstNarrowestSpectrum::CreateRegOption(CallDevices* call, 
unsigned routeInd, std::vector<std::shared_ptr<Route> >& routes, 
std::vector<TypeModulation>& modulations) {
    std::shared_ptr<Route> orRoute = call->GetRoute(routeInd);
    unsigned numRouteNodes = orRoute->GetNumNodes();
    double bitRate = call->GetBitRate();
    std::vector<NodeDevices*> nodes(0);
    std::shared_ptr<Route> auxRoute;
    std::vector<TypeModulation> auxVecModulations = 
    Modulation::GetModulationFormats();
    TypeModulation scheme = auxVecModulations.front();
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
                
                if(this->CheckSpectrumAndOSNR(bitRate, auxRoute.get(),scheme)){
                    
                    if(auxRoute->GetDeNode() == orRoute->GetDeNode()){
                        routes.push_back(auxRoute);
                        modulations.push_back(scheme);
                        return true;
                    }
                    else{
                        if(scheme != auxVecModulations.front()){
                            routes.push_back(auxRoute);
                            modulations.push_back(scheme);
                            sourIndex = testIndex;
                            curNodeIndex = testIndex;
                            scheme = auxVecModulations.front();
                        }
                        else{
                            curNodeIndex = testIndex;
                        }
                        //Colocar a expressão "curNodeIndex = testIndex" aqui?
                    }
                }
                else{
                    if(curNodeIndex != sourIndex){
                        auxRoute = orRoute->CreatePartialRoute(sourIndex, 
                        curNodeIndex);
                        routes.push_back(auxRoute);
                        modulations.push_back(scheme);
                        sourIndex = curNodeIndex;
                        testIndex = curNodeIndex;
                    }
                    else{
                        testIndex--;
                        scheme = TypeModulation(scheme - 1);
                        
                        if(scheme == auxVecModulations.back()){
                            routes.clear();
                            modulations.clear();
                            return false;
                        }
                    }
                }
            }
        }
    }
    
    routes.clear();
    modulations.clear();
    return false;
}
