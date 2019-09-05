/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CSA.h
 * Author: paulofilho
 * Core Spectrum Assignment class
 * Created on 7 de fevereiro de 2019, 15:19
 */

#ifndef CSA_H
#define CSA_H

#include "SA.h"

class CSA: public SA {
public:
    CSA(ResourceAlloc* rsa, SpectrumAllocationOption option, 
        Topology* topology);
    /**
     * @brief Spectrum allocation that chooses first core and first set of slots
     * available to allocate the call
     * @param call
     */    
    virtual ~CSA();
    
    
    void SpecAllocation(Call* call) override;

    void FirstFit(Call* call);        
    /**
     * @brief Return strong call class pointer to cast
     * @param call pointer of call class
     * @return Call pointer
     */    
    void MSCL(Call* call);
private:
    
};

#endif /* CSA_H */

