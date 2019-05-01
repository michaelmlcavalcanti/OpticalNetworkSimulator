/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CallDevices.h
 * Author: BrunoVinicius
 *
 * Created on April 30, 2019, 2:52 PM
 */

#ifndef CALLDEVICES_H
#define CALLDEVICES_H

#include "Call.h"

class Regenerator;

class CallDevices : public Call {
public:
    
    CallDevices(Node* orNode, Node* deNode, double bitRate, TIME deacTime);
    
    virtual ~CallDevices();
private:
    
    std::vector<std::shared_ptr<Call>> transpSegments;
    
    std::vector<std::shared_ptr<Regenerator>> regenerators;
    
    unsigned int totalNumSlots;
};

#endif /* CALLDEVICES_H */

