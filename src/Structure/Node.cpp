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

Node::Node(Topology* topPointer, NodeIndex nodeId) :
topology(topPointer), nodeId(nodeId), nodeWorking(true), neighborNodes(0) {
    
}

Node::~Node() {

}

bool Node::operator==(const Node& right) const {
    
    if(this->nodeId == right.nodeId)
        return true;
    
    return false;
}

void Node::Initialize() {
    
}

NodeIndex Node::GetNodeId() const {
    return nodeId;
}

void Node::SetNodeState(bool NodeWorking) {
    this->nodeWorking = NodeWorking;
}

bool Node::IsNodeWorking() const {
    return nodeWorking;
}

void Node::AddNeighborNode(Node* node) {
    neighborNodes.push_back(node);
}

unsigned int Node::GetNodeDegree() {
    return neighborNodes.size();
}

Topology* Node::GetTopology() const {
    return topology;
}
