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

class SBVT_TSS_ML : public BVT {
public:
    
    SBVT_TSS_ML(Topology* topology, NodeDevices* node);
    
    virtual ~SBVT_TSS_ML();
    
    
    void Initialize() override;
    
    unsigned int GetNumberFreeSubCarriers() const override;
    
    void SetCallToSubCarriers(CallDevices* call, unsigned int numSubCarriers) 
    override;
    
    void ConnectSubCarriers(CallDevices* call) override;
    
    void ReleaseSubCarriers(CallDevices* call) override;
private:
    
    std::vector<CarrierState> subCarriers;
    
    std::vector<TypeModulation> modulations;
    
    std::vector<CallDevices*> calls;
    
    static unsigned int numSubCarriers;
};

#endif /* SBVT_TSS_ML_H */

