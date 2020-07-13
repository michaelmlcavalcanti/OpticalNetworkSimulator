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

class ResourceDeviceAlloc;
class CallDevices;

class ProtectionScheme {
public:
    
    ProtectionScheme(ResourceDeviceAlloc* rsa);
    
    virtual ~ProtectionScheme();
    
    virtual void ResourceAlloc(CallDevices* call) = 0;
    
 //   virtual void CreateProtectionRoutes();
protected:
    
    ResourceDeviceAlloc* resDevAlloc;
};

#endif /* PROTECTIONSCHEME_H */

