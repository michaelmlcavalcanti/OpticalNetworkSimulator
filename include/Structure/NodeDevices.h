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
class BVT;
class Call;

#include "Node.h"
#include "Devices/Regenerator.h"

class NodeDevices : public Node {
public:
    
    NodeDevices(Topology* topPointer, NodeId nodeId);
    
    virtual ~NodeDevices();
    
    
    void Initialize() override;
    
    
    unsigned int GetNumRegenerator() const;

    void SetNumRegenerator(unsigned int numRegenerator);
    
    unsigned int GetNumFreeRegenerators() const;
    
    unsigned int GetNumOccRegenerators() const;
    
    void IncreaseNumFreeRegenerators();
    
    void DecreaseNumFreeRegenerators();
    
    bool isThereFreeRegenerators(double bitRate) const;
    
    std::vector<std::shared_ptr<Regenerator>> GetFreeRegenenerators(
                                              double bitRate) const;
    
    
    unsigned int GetNumTransponder() const;

    void SetNumTransponder(unsigned int numTransponder);

private:
    
    void InitializeRegnerators();
    
    void InitializeTransponders();
    
    void CreateRegenerators();
    
    void DeleteRegenerators();
    
    void CreateTransponders();
    
    void DeleteTransponders();
private:
    
    unsigned int numRegenerator;
    
    unsigned int numFreeRegenerators;
    
    std::vector<std::shared_ptr<Regenerator>> regenerators;
    
    
    unsigned int numTransponder;
    
    std::vector<std::shared_ptr<BVT>> transponders;
};

#endif /* NODEDEVICES_H */

