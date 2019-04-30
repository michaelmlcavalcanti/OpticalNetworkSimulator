/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Node.cpp
 * Author: bruno
 * 
 * Created on August 4, 2018, 1:14 AM
 */

#include "../../include/Structure/Node.h"

std::ostream& operator<<(std::ostream& ostream,
Node* node) {
    ostream  << "Node Id: " << node->GetNodeId() << std::endl;
    
    return ostream;
}

Node::Node(Topology* topPointer, NodeId nodeId) :
topology(topPointer), nodeId(nodeId), nodeWorking(true) {
    
}

Node::~Node() {

}

void Node::Initialize() {
    
}

NodeId Node::GetNodeId() const {
    return nodeId;
}

void Node::SetNodeId(NodeId nodeId) {
    this->nodeId = nodeId;
}

void Node::SetNodeState(bool NodeWorking) {
    this->nodeWorking = NodeWorking;
}

bool Node::IsNodeWorking() const {
    return nodeWorking;
}

Topology* Node::GetTopology() const {
    return topology;
}
