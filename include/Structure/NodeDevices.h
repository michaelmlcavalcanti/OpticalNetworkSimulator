/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NodeDevices.h
 * Author: BrunoVinicius
 *
 * Created on April 29, 2019, 3:08 PM
 */

#ifndef NODEDEVICES_H
#define NODEDEVICES_H

#include <memory>
#include <vector>

class Regenerator;
class Call;

#include "Node.h"

class NodeDevices : public Node {
public:
    
    NodeDevices(Topology* topPointer, NodeId nodeId);
    
    virtual ~NodeDevices();
    
    
    void Initialize() override;
    
    void ConnectRegenerator(Call* call);
    
    void ReleaseRegenerator(Call* call);
    
    
    unsigned int GetNumRegenerator() const;

    void SetNumRegenerator(unsigned int numRegenerator);
    
    unsigned int GetNumFreeRegenerators() const;
private:
    
    void CreateRegenerators();
    
    void DeleteRegenerators();
private:
    
    unsigned int numRegenerator;
    
    unsigned int numFreeRegenerators;
    
    std::vector<std::shared_ptr<Regenerator>> regenerators;
};

#endif /* NODEDEVICES_H */

