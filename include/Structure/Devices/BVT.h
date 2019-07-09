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

enum TypeBVT {
    TypeInvalid,
    TypeBVT_TSS_ML
};

#include "Device.h"
#include "../../GeneralClasses/Def.h"
#include  "../../ResourceAllocation/Modulation.h"

class BVT : public Device {
public:
    
    BVT(Topology* topology, NodeDevices* node, TypeBVT typeBVT);
    
    virtual ~BVT();
    
    
    virtual void Initialize() override;
    
    virtual unsigned int GetNumberFreeSubCarriers() const = 0;
    
    virtual void SetCallToSubCarriers(CallDevices* call, 
                                      unsigned int numSubCarriers) = 0;
    
    virtual void ConnectSubCarriers(CallDevices* call) = 0;
    
    virtual void ReleaseSubCarriers(CallDevices* call) = 0;
private:
    
    NodeDevices* node;
    
    TypeBVT typeBVT;
    
    bool isActive;
};

#endif /* BVT_H */

