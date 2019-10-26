/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProtectionScheme.h
 * Author: henrique
 *
 * Created on October 26, 2019, 9:31 AM
 */

#ifndef PROTECTIONSCHEME_H
#define PROTECTIONSCHEME_H

class ResourceAlloc;

class ProtectionScheme {
public:
    
    ProtectionScheme(ResourceAlloc* rsa);
    
    virtual ~ProtectionScheme();
protected:
    
    ResourceAlloc* rsa;
};

#endif /* PROTECTIONSCHEME_H */

