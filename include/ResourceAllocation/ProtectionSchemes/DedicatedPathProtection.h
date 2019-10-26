/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DedicatedPathProtection.h
 * Author: henrique
 *
 * Created on October 26, 2019, 9:36 AM
 */

#ifndef DEDICATEDPATHPROTECTION_H
#define DEDICATEDPATHPROTECTION_H

#include "ProtectionScheme.h"

class DedicatedPathProtection : public ProtectionScheme {
public:
    
    DedicatedPathProtection(ResourceAlloc* rsa);

    virtual ~DedicatedPathProtection();
private:

};

#endif /* DEDICATEDPATHPROTECTION_H */

