/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Route.cpp
 * Author: BrunoVinicius
 * 
 * Created on November 15, 2018, 10:40 PM
 */

#include "../../include/ResourceAllocation/Route.h"
#include "../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Structure/Link.h"
#include "../../include/Structure/Node.h"

Route::Route(ResourceAlloc* rsaAlg, const std::vector<int>& path)
:resourceAlloc(rsaAlg), topology(rsaAlg->GetTopology()), path(path),
pathNodes(0), pathLinks(0), cost(0.0) {
    
    for(auto it: this->path){
        this->pathNodes.push_back(this->topology->GetNode(it));
    }
    
    for(unsigned int a = 0; a < this->pathNodes.size()-1; a++){
        pathLinks.push_back(topology->GetLink(pathNodes.at(a)->GetNodeId(), 
                                              pathNodes.at(a+1)->GetNodeId()));
    }
    
    this->SetCost();
}

Route::~Route() {
    
}

bool Route::operator==(const Route& right) const {
    
    if(right.path == this->path)
        return true;
    
    return false;
}

bool Route::checkShareLink(Route* route) const {
    
    for(unsigned int hop = 0; hop < this->GetNumHops(); hop++){
        
        for(unsigned int hop2 = 0; hop2 < route->GetNumHops(); hop2++){
            
            if(this->GetLink(hop) == route->GetLink(hop2))
                return true;
        }
    }
    return false;
}

bool Route::operator<(const Route& right) const {
    
    return right.GetCost() > this->GetCost();
}

bool Route::operator>(const Route& right) const {
    
    return right.GetCost() < this->GetCost(); 
}

int Route::GetOrNodeId() const {
    assert(this->path.size() > 0);
    
    return this->path.front();
}

Node* Route::GetOrNode() const {
    assert(this->pathNodes.size() > 0);
    
    return this->pathNodes.front();
}

int Route::GetDeNodeId() const {
    assert(this->path.size() > 0);
    
    return this->path.back();
}

Node* Route::GetDeNode() const {
    assert(this->pathNodes.size() > 0);
    
    return this->pathNodes.back();
}

int Route::GetNodeId(unsigned int index) const {
    assert(index >= 0 && index < this->path.size());
    
    return this->path.at(index);
}

Node* Route::GetNode(unsigned int index) const {
    assert(index >= 0 && index < this->pathNodes.size());
    
    return this->pathNodes.at(index);
}

std::vector<Node*> Route::GetNodes() const {
    return pathNodes;
}

unsigned int Route::GetNumHops() const {
    return this->path.size() - 1;
}

unsigned int Route::GetNumNodes() const {
    return this->path.size();
}

std::vector<int> Route::GetPath() const {
    return this->path;
}

double Route::GetCost() const {
    return this->cost;
}

void Route::SetCost(double cost) {
    assert(cost >= 0.0);
    
    this->cost = cost;
}

void Route::SetCost() {
    Link *link;
    double cost = 0.0;
    
    for(unsigned int a = 0; a < this->GetNumHops(); a++){
        link = this->topology->GetLink(this->path.at(a), this->path.at(a+1));
        cost += link->GetCost();
    }
    
    this->SetCost(cost);
}

Link* Route::GetLink(unsigned int index) const {
    assert(index < this->GetNumHops());
    
    return this->topology->GetLink(this->GetNodeId(index), 
                                   this->GetNodeId(index + 1));
}

void Route::SetAllNodesWorking() {
    
    for(auto it: this->pathNodes){
        it->SetNodeState(true);
    }
}

std::shared_ptr<Route> Route::CreatePartialRoute(unsigned int ind1, 
unsigned int ind2) {
    assert(ind2 >= ind1 && ind1 < path.size() && ind2 < path.size());
    std::vector<int> newPath(0);
    
    for(unsigned int a = ind1; a <= ind2; a++){
        newPath.push_back(this->GetNodeId(a));
    }
    
    return std::make_shared<Route>(this->resourceAlloc, newPath);
}

std::shared_ptr<Route> Route::AddRoute(std::shared_ptr<Route>& route) {
    assert(this->GetDeNode() == route->GetOrNode());
    std::vector<int> newPath = this->GetPath();
    
    for(unsigned int a = 1; a < route->GetNumNodes(); a++){
        newPath.push_back(route->GetNodeId(a));
    }
    
    return std::make_shared<Route>(this->resourceAlloc, newPath);
}
