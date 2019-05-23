/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Regenerator.h
 * Author: BrunoVinicius
 *
 * Created on April 28, 2019, 4:51 PM
 */

#ifndef REGENERATOR_H
#define REGENERATOR_H

class NodeDevices;

#include "Device.h"
#include "../../Calls/CallGenerator.h"

/**
 * @brief Class that represents a virtualized electronic regenerator.
 */
class Regenerator : public Device {
public:

    Regenerator(Topology* topology, NodeDevices* node);

    virtual ~Regenerator();
    
    
    void Initialize() override;

    
    void SetRegeneratorOn();
    
    void SetRegeneratorOff();
    
    bool IsActive() const;
    
    
    static const double GetTrafficSupported();
private:
    
    NodeDevices* node;
    
    bool isActive;
    
    TIME onTime;
    
    TIME totalOnTime;
    
    static const double trafficSupported;
};

#endif /* REGENERATOR_H */

