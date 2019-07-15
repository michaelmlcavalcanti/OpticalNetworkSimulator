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

enum TypeBVT {
    TypeInvalid,
    TypeBVT_TSS_ML
};

#include "Device.h"
#include "../../GeneralClasses/Def.h"
#include  "../../ResourceAllocation/Modulation.h"

/**
 * @brief Basic BVT class. This class has the parameters common to every BVT
 * device.
 */
class BVT : public Device {
public:
    /**
     * @brief Standard constructor of a basic BVT object.
     * @param topology Physical topology to which this SBVT belongs.
     * @param node Node to which this SBVT belongs.
     * @param typeBVT Type that specifies the BVT.
     */
    BVT(Topology* topology, NodeDevices* node, TypeBVT typeBVT);
    /**
     * @brief Standard destructor of a basic BVT object.
     */
    virtual ~BVT();
    
    /**
     * @brief Initialize the basic BVt parameters.
     */
    virtual void Initialize() override;
    /**
     * @brief Function to return the number of fre subcarriers.
     * @return Number of fre subcarriers.
     */
    virtual unsigned int GetNumberFreeSubCarriers() const = 0;
    /**
     * @brief Function to set a specified call to the free subcarriers in this 
     * SBVT.
     * @param call Call request to set.
     * @param numSubCarriers Number of subcarriers to set.
     */
    virtual void SetCallToSubCarriers(CallDevices* call, 
                                      unsigned int numSubCarriers) = 0;
    /**
     * @brief Occupy the subcarriers with this call request (or part of it).
     * @param call Call request to set.
     */
    virtual void ConnectSubCarriers(CallDevices* call) = 0;
    /**
     * @brief Function to release the subcarriers occupied a specified call 
     * request.
     * @param call Call request to release;
     */
    virtual void ReleaseSubCarriers(CallDevices* call) = 0;
private:
    /**
     * @brief Type that specifies the BVT.
     */
    TypeBVT typeBVT;
    /**
     * @brief BVT state indicator.
     */
    bool isActive;
};

#endif /* BVT_H */

