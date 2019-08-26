/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RegeneratorAssignment.h
 * Author: brunovacorreia
 *
 * Created on August 25, 2019, 7:43 PM
 */

#ifndef REGENERATORASSIGNMENT_H
#define REGENERATORASSIGNMENT_H

class ResourceDeviceAlloc;

#include "../ResourceDeviceAlloc.h"
#include "../Modulation.h"
#include "../Route.h"
#include "../Resources.h"
#include "../../GeneralClasses/Def.h"
#include "../../Calls/CallDevices.h"
#include "../../Structure/NodeDevices.h"
#include "../../Structure/Topology.h"
#include "../../Data/Options.h"

class RegeneratorAssignment {
public:
    
    RegeneratorAssignment(ResourceDeviceAlloc* resDevAlloc);
    
    virtual ~RegeneratorAssignment();
    
    
    virtual void ResourceAlloc(CallDevices* call) = 0;
    /**
     * @brief Function to order the tuples of route and regeneration combination
     * for a call request.
     * @param call Call request to order.
     * @param vec Container of tuples of route and regeneration combination 
     * indexes.
     */
    virtual void OrderRegenerationOptions(CallDevices* call, 
    std::vector<std::tuple<unsigned, unsigned>>& vec);
    
    virtual bool CreateRegOption(CallDevices* call, unsigned routeInd, 
    std::vector<std::shared_ptr<Route>> &routes, 
    std::vector<TypeModulation> &modulations);
    /**
     * @brief Function to calculate the cost of the tuple, that is composed by
     * a route and a regeneration option. The cost depends on the cost metric
     * chosen.
     * @param call Call request to evaluate.
     * @param routeIndex Route index of the container of possible routes.
     * @param subRouteIndex Regeneration option index.
     * @return Tuple cost.
     */
    virtual double CalcTupleCost(CallDevices* call, unsigned routeIndex,
    unsigned subRouteIndex);
protected:
    /**
     * @brief Function to get the call request number of slots using the 
     * least efficient modulation format.
     * @param call Call request.
     * @return Number of slots.
     */
    unsigned int GetN(CallDevices* call);
    
    bool CheckSpectrumAndOSNR(const double bitRate, Route* route, 
    TypeModulation modulation);
    
    bool CheckSpectrumAndOSNR(const double bitRate, Route* route);
    
    TypeModulation GetBestModulation(const double bitRate, Route* route);
protected:
    
    ResourceDeviceAlloc* resDevAlloc;
};

#endif /* REGENERATORASSIGNMENT_H */

