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

typedef bool UseRegeneration;

#include "Call.h"
#include "../ResourceAllocation/Modulation.h"

class Regenerator;

class CallDevices : public Call {
public:
    
    CallDevices(Node* orNode, Node* deNode, double bitRate, TIME deacTime);
    
    virtual ~CallDevices();
    
    
    void CreateTranspSegments(std::vector<std::shared_ptr<Route> > subroutes);
    
    void SetTranspSegModulation(std::vector<TypeModulation> modulations);
    
    std::vector<Call*> GetTranspSegments();
    
    unsigned int GetTotalNumSlots() const override;
    
    void SetTotalNumSlots() override;
    
    std::vector<std::shared_ptr<Regenerator> > GetRegenerators() const;

    void SetRegenerators(std::vector<std::shared_ptr<Regenerator> > 
    regenerators);
    
    void SetUseRegeneration();
    
    bool CheckUseRegeneration() const;
private:
    
    std::vector<std::shared_ptr<Call>> transpSegments;
    
    std::vector<std::shared_ptr<Regenerator>> regenerators;
    
    UseRegeneration useRegeneration;
};

#endif /* CALLDEVICES_H */

