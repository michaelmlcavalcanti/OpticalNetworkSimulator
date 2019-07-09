/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SBVT_TSS_ML.h
 * Author: brunovacorreia
 *
 * Created on June 25, 2019, 7:57 PM
 */

#ifndef SBVT_TSS_ML_H
#define SBVT_TSS_ML_H

#include "BVT.h"

/**
 * @brief Class of an SBVT ML transponder sliceable in subcarriers (TSS).
 * This transponder presents a fixed number of subcarriers. Each one may have 
 * different modulation format and several of them can be used to allocate a 
 * single call request(or part of it).
 */
class SBVT_TSS_ML : public BVT {
public:
    /**
     * @brief Standard constructor of a SBVT TSS ML object.
     * @param topology Physical topology to which this SBVT belongs. 
     * @param node Node to which this SBVT belongs.
     */
    SBVT_TSS_ML(Topology* topology, NodeDevices* node);
    /**
     * @brief Standard destructor of this SBVT.
     */
    virtual ~SBVT_TSS_ML();
    
    /**
     * @brief Initialize the container of subcarriers, modulations and calls.
     * Also initialize the common BVT parameters.
     */
    void Initialize() override;
    /**
     * @brief Function to return the number of fre subcarriers.
     * @return Number of fre subcarriers.
     */
    unsigned int GetNumberFreeSubCarriers() const override;
    /**
     * @brief Function to set a specified call to the free subcarriers in this 
     * SBVT.
     * @param call Call request to set.
     * @param numSubCarriers Number of subcarriers to set.
     */
    void SetCallToSubCarriers(CallDevices* call, unsigned int numSubCarriers) 
    override;
    /**
     * @brief Occupy the subcarriers with this call request (or part of it).
     * @param call Call request to set.
     */
    void ConnectSubCarriers(CallDevices* call) override;
    /**
     * @brief Function to release the subcarriers occupied a specified call 
     * request.
     * @param call Call request to release;
     */
    void ReleaseSubCarriers(CallDevices* call) override;
private:
    /**
     * @brief Container that represent all subcarriers in this SBVT.
     */
    std::vector<CarrierState> subCarriers;
    /**
     * @brief Container of modulation format used by each subcarrier.
     */
    std::vector<TypeModulation> modulations;
    /**
     * @brief Container of call requests that occupies the subcarriers.
     */
    std::vector<CallDevices*> calls;
    /**
     * @brief Total number of subcarriers in this SBVT.
     */
    static unsigned int numSubCarriers;
};

#endif /* SBVT_TSS_ML_H */

