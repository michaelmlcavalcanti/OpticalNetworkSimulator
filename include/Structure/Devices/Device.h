/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Device.h
 * Author: BrunoVinicius
 *
 * Created on April 28, 2019, 3:19 PM
 */

#ifndef DEVICE_H
#define DEVICE_H

class Topology;

enum DeviceType {
    DeviceUndefined,
    DeviceRegenerator,
    DeviceAmplifier,
    DeviceTransponder
};

class Device {
public:
    
    Device(Topology* topology, DeviceType type = DeviceUndefined);
    
    virtual ~Device();
    
    
    virtual void Initialize() = 0;
    
    
    Topology* GetTopology() const;
    
    DeviceType GetType() const;
private:
    
    Topology* topology;
    
    DeviceType type;
};

#endif /* DEVICE_H */

