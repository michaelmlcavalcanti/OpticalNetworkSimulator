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

class Node;

#include "Device.h"

/**
 * @brief Virtualized regenerator class.
 */
class Regenerator : public Device {
public:

    Regenerator(Topology* topology, Node* node, DeviceType type);

    virtual ~Regenerator();
    
    
    void SetRegeneratorOn();
    
    void SetRegeneratorOff();
private:
    
    Node* node;
    
    bool state;
    
    TIME onTime;
    
    TIME totalOnTime;
};

#endif /* REGENERATOR_H */

