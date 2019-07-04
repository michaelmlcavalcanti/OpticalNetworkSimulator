/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BVT.h
 * Author: brunovacorreia
 *
 * Created on June 24, 2019, 6:44 PM
 */

#ifndef BVT_H
#define BVT_H

typedef bool CarrierState;

class NodeDevices;

#include "Device.h"
#include "../../Calls/CallGenerator.h"
#include "../../GeneralClasses/Def.h"

class BVT : public Device {
public:
    
    BVT(Topology* topology, NodeDevices* node);
    
    virtual ~BVT();
    
    
    virtual void Initialize() override;
    
    virtual bool IsPossibleAllocate(Call* call) = 0;
private:
    
    NodeDevices* node;
    
    bool isActive;
    
    std::vector<CarrierState> carriers;
};

#endif /* BVT_H */

