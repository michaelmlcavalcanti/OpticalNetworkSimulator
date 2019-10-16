/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Topology.cpp
 * Author: bruno
 * 
 * Created on August 3, 2018, 12:00 AM
 */

#include "../../include/Structure/Topology.h"

#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Data/Options.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Structure/Structures.h"
#include "../../include/GeneralClasses/Def.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/ResourceAllocation/Signal.h"
#include "../../include/Calls/Call.h"
#include "../../include/Calls/CallDevices.h"

std::ostream& operator<<(std::ostream& ostream, 
const Topology* topology) {
    ostream << "TOPOLOGY" << std::endl;
    
    ostream << "Number of nodes: " << topology->GetNumNodes() 
            << std::endl;
    ostream << "Number of links: " << topology->GetNumLinks()
            << std::endl;
    ostream << "Number of slots: " << topology->GetNumSlots()
            << std::endl;
    ostream << "Number of cores: " << topology->GetNumCores()
            << std::endl;
    
    for(auto it: topology->vecNodes){
        ostream << it << std::endl;
    }
    
    for(auto it : topology->vecLinks){
        if(it != nullptr)
            ostream << it << std::endl;
    }
    return ostream;
}

Topology::Topology(SimulationType* simulType) 
:simulType(simulType), options(nullptr), vecNodes(0), vecLinks(0), numNodes(0), 
numLinks(0), numSlots(0), numCores(0), maxLength(0.0), numRegenerators(0),
numTransponders(0) {

}

Topology::~Topology() {
    
    for(auto it : this->vecNodes){
        it.reset();
    }
    
    
    for(auto it : this->vecLinks){
        it.reset();
    }
}

void Topology::LoadFile() {
    std::ifstream auxIfstream;
    unsigned int auxInt;
    
    options = simulType->GetOptions();
    simulType->GetInputOutput()->LoadTopology(auxIfstream);
    
    auxIfstream >> auxInt;
    this->SetNumNodes(auxInt);
    auxIfstream >> auxInt;
    this->SetNumLinks(auxInt);
    this->SetNumSlots(simulType->GetParameters()->GetNumberSlots());
    this->SetNumCores(simulType->GetParameters()->GetNumberCores());
        
    this->CreateNodes(auxIfstream);
    
    this->CreateLinks(auxIfstream);
    
    if(options->GetDevicesOption() != DevicesDisabled)
        this->SetFixedNumberOfDevices();
}

void Topology::Initialize() {
    
    for(auto it : this->vecNodes){
        it->Initialize();
    }
    
    for(auto it : this->vecLinks){
        if(it != nullptr)
            it->Initialize();
    }
}

void Topology::CreateNodes(std::ifstream& ifstream) {
    std::shared_ptr<Node> node;
    
    for(unsigned int a = 0; a < this->GetNumNodes(); ++a){
        
        if(options->GetDevicesOption() == DevicesEnabled)
            node = std::make_shared<NodeDevices>(this, a);
        else
            node = std::make_shared<Node>(this, a);
        
        this->InsertNode(node);
        node.reset();
    }
}

void Topology::CreateLinks(std::ifstream& ifstream) {
    unsigned  int orNode, deNode, nSec;
    double length;
    std::shared_ptr<Link> link;
    unsigned int numCores = this->GetNumCores();
    unsigned int numSlots = this->GetNumSlots();
    double maxSecLength = simulType->GetParameters()->GetMaxSectionLegnth();
    
    for(unsigned int a = 0; a < this->GetNumLinks(); ++a){
        ifstream >> orNode;
        ifstream >> deNode;
        ifstream >> length;
        nSec = std::ceil(length / maxSecLength);
        
        link = std::make_shared<Link>(this, orNode, deNode, length, nSec, 
        numCores, numSlots);
                  
        this->InsertLink(link);
        link.reset();
    }
}

unsigned int Topology::GetNumNodes() const {
    return numNodes;
}

void Topology::SetNumNodes(unsigned int numNodes) {
    assert(this->numNodes == 0);
    this->numNodes = numNodes;
    
    for(unsigned int a = 0; a < this->numNodes; ++a){
        this->vecNodes.push_back(nullptr);
        
        for(unsigned int b = 0; b < this->numNodes; ++b){
            this->vecLinks.push_back(nullptr);
        }
    }
}

unsigned int Topology::GetNumLinks() const {
    return numLinks;
}

void Topology::SetNumLinks(unsigned int numLinks) {
    assert(this->numLinks == 0);
    this->numLinks = numLinks;
}

unsigned int Topology::GetNumCores() const {
    return numCores;
}

void Topology::SetNumCores(unsigned int numCores) {
    //assert(this->numCores == 0);
    this->numCores = numCores;
}

unsigned int Topology::GetNumSlots() const {
    return numSlots;
}

void Topology::SetNumSlots(unsigned int numSlots) {
    assert(this->numSlots == 0);
    this->numSlots = numSlots;
}

void Topology::InsertLink(std::shared_ptr<Link> link) {
    assert( this->vecLinks.at(link->GetOrigimNode() * this->GetNumNodes()
    + link->GetDestinationNode()) == nullptr );
    
    this->vecLinks.at(link->GetOrigimNode() * this->GetNumNodes()
    + link->GetDestinationNode()) = link;
}

void Topology::InsertNode(std::shared_ptr<Node> node) {
    assert(node.get()->GetNodeId() < this->vecNodes.size());
    
    this->vecNodes.at(node->GetNodeId()) = node;
}

double Topology::GetMaxLength() const {
    return maxLength;
}

void Topology::SetNumDevices(unsigned int numDevices, DeviceType type) {
    
    switch(type){
        case DeviceRegenerator:
            this->SetNumRegenerators(numDevices);
            break;
        case DeviceTransponder:
            this->SetNumTransponders(numDevices);
            break;
        default:
            std::cerr << "Invalid device type" << std::endl;
            std::abort();
    }
    this->DistributeDevices(type);
}

void Topology::SetMaxLength() {
    
    for(auto it : this->vecLinks){
        
        if(it == nullptr)
            continue;
        
        if(this->maxLength < it->GetLength())
            this->maxLength = it->GetLength();
    }
}

void Topology::SetAditionalSettings() {
    this->SetMaxLength();
    this->SetLinksIniCost();
    this->SetNodesNeighbors();
}

void Topology::SetAllLinksWorking() {
    
    for(auto it: this->vecLinks){
        if(it != nullptr){
            it->SetLinkState(true);
        }
    }
}

void Topology::SetLinksIniCost() {
    
    switch(options->GetLinkCostType()){
        case LinkCostHop:
            for(auto it : vecLinks){
                if(it == nullptr)
                    continue;
                it->SetCost(1.0);
            }
            break;
        case LinkCostLength:
            for(auto it : vecLinks){
                if(it == nullptr)
                    continue;
                it->SetCost(it->GetLength());
            }
            break;
        case LinkCostNormLength:
            for(auto it : vecLinks){
                if(it == nullptr)
                    continue;
                it->SetCost(it->GetLength()/this->GetMaxLength());
            }
            break;
        default:
            std::cerr << "Invalid link cost type" << std::endl;
            std::abort();
    }
}

void Topology::SetNodesNeighbors() {
    Link* auxLink;
    
    for(auto node1: vecNodes){
        
        for(auto node2: vecNodes) {
            
            if(node1 == node2)
                continue;
            auxLink = this->GetLink(node1->GetNodeId(), node2->GetNodeId());
            
            if(auxLink == nullptr)
                continue;
            node1->AddNeighborNode(node2.get());
        }
    }
}

Node* Topology::GetNode(NodeIndex index) const {
    assert(index < this->GetNumNodes());
    return this->vecNodes.at(index).get();
}

Link* Topology::GetLink(NodeIndex indexOrNode, NodeIndex indexDeNode) const {
    assert(indexOrNode < this->GetNumNodes());
    assert(indexDeNode < this->GetNumNodes());
    
    return this->vecLinks.at(indexOrNode * this->numNodes + 
    indexDeNode).get();
}

std::shared_ptr<Link> Topology::GetLinkPointer(NodeIndex indexOrNode, 
                                               NodeIndex indexDeNode) const {
    assert(indexOrNode < this->GetNumNodes());
    assert(indexDeNode < this->GetNumNodes());
    
    return this->vecLinks.at(indexOrNode * this->numNodes + indexDeNode);
}

bool Topology::IsValidLink(const Link* link) {
    Node* sourceNode = this->GetNode(link->GetOrigimNode());
    Node* destNode = this->GetNode(link->GetDestinationNode());
    
    if(link != nullptr && this->IsValidNode(sourceNode) && 
       this->IsValidNode(destNode))
        return true;
    return false;
}

bool Topology::IsValidNode(const Node* node) {
    if(node != nullptr && node->GetNodeId() < this->numNodes)
        return true;
    return false;
}

bool Topology::IsValidRoute(Route* route) {
    
    if(route != nullptr){
        Link* link;
        for(unsigned int a = 0; a < route->GetNumHops(); a++){
            link = route->GetLink(a);
            if(link == nullptr)
                return false;   
        }
        return true;
    }
    return false;
}
    
bool Topology::IsValidSlot(SlotIndex index) {
    if(index < numSlots)
        return true;
    return false;
}

bool Topology::IsValidCore(CoreIndex index) {
    if(index < numCores)
        return true;
    return false;
}

bool Topology::IsValidLigthPath(Call* call) {
    if( (this->IsValidRoute(call->GetRoute())) && 
        (call->GetFirstSlot() <= call->GetLastSlot()) && 
        (this->IsValidSlot(call->GetFirstSlot())) && 
        (this->IsValidSlot(call->GetLastSlot())) &&
        this->IsValidCore(call->GetCore())){
        return true;
    }
    
    return false;
}

bool Topology::IsValidLigthPath(CallDevices* call) {
    std::vector<Call*> calls = call->GetTranspSegments();
    
    if(!calls.empty()){
        for(auto it: calls){
            if(!this->IsValidLigthPath(it))
                return false;
        }
    }
    else{
        Call* baseCall = dynamic_cast<Call*>(call);
        
        if(!this->IsValidLigthPath(baseCall))
            return false;
    }
    
    return true;
}

bool Topology::CheckInsertFreeRegenerators(CallDevices* call) {
    NodeDevices* auxNode;
    std::vector<std::shared_ptr<Regenerator>> vecReg(0);
    std::vector<std::shared_ptr<Regenerator>> auxVecReg(0);
    std::vector<Call*> calls = call->GetTranspSegments();
    
    for(unsigned int a = 0; a < calls.size()-1; a++){
        auxNode = dynamic_cast<NodeDevices*>(calls.at(a)->GetDeNode());
        
        if(auxNode->isThereFreeRegenerators(calls.at(a)->GetBitRate())){
            auxVecReg = auxNode->GetFreeRegenenerators(calls.at(a)->
                                 GetBitRate());
            vecReg.insert(vecReg.end(), auxVecReg.begin(), auxVecReg.end());
        }
        else{
            auxVecReg.clear();
            vecReg.clear();
            return false;
        }
    }
    
    if(!vecReg.empty())
        call->SetRegenerators(vecReg);
    return true;
}

bool Topology::CheckInsertFreeBVTs(CallDevices* call) {
    NodeDevices* orNode = dynamic_cast<NodeDevices*>(call->GetRoute()
                                                         ->GetOrNode());
    NodeDevices* deNode = dynamic_cast<NodeDevices*>(call->GetRoute()
                                                         ->GetDeNode());
    unsigned int numSlots = call->GetNumberSlots();
    std::vector<std::shared_ptr<BVT>> vecBVT(0);
    std::vector<std::shared_ptr<BVT>> auxVecBVT(0);
    
    if(orNode->isThereFreeBVT(numSlots) && deNode->isThereFreeBVT(numSlots)){
        auxVecBVT = orNode->GetBVTs(call);
        vecBVT.insert(vecBVT.end(), auxVecBVT.begin(), auxVecBVT.end());
        auxVecBVT = deNode->GetBVTs(call);
        vecBVT.insert(vecBVT.end(), auxVecBVT.begin(), auxVecBVT.end());
        call->SetTransponders(vecBVT);
        
        return true;
    }
    
    return false;
}

unsigned int Topology::GetNumUsedSlots(Route* route) const {
    Link* link;
    unsigned int numUsedSlots = 0;
    
    for(unsigned int a = 0; a < route->GetNumHops(); a++){
        link = route->GetLink(a);
        numUsedSlots += link->GetNumberOccupiedSlots();
    }
    
    return numUsedSlots;
}

unsigned int Topology::GetNumSlots(Route* route) const {
    Link* link;
    unsigned int numSlots = 0;
    
    for(unsigned int a = 0; a < route->GetNumHops(); a++){
        link = route->GetLink(a);
        numSlots += link->GetNumSlots();
    }
    
    return numSlots;
}

void Topology::Connect(Call* call) {
    
    switch(options->GetDevicesOption()){
        case DevicesDisabled:
            this->ConnectWithoutDevices(call);
            break;
        case DevicesEnabled:
            this->ConnectWithDevices(call);
            break;
        default:
            std::cerr << "Invalid connection used" << std::endl;
            std::abort();
    }
}

void Topology::ConnectWithoutDevices(Call* call) {
    Link* link;
    Route* route = call->GetRoute();
    unsigned int numHops = route->GetNumHops();
    unsigned int core = call->GetCore();
    
    for(unsigned int a = 0; a < numHops; a++){
        link = route->GetLink(a);
        
        if(this->IsValidLink(link)){
            
            for(unsigned int slot = call->GetFirstSlot(); 
            slot <= call->GetLastSlot(); slot++){
                link->OccupySlot(core, slot);
            }
        }
    }
}

void Topology::ConnectWithDevices(Call* call) {
    CallDevices* callDev = dynamic_cast<CallDevices*>(call);
    RegenerationOption regOption = options->GetRegenerationOption();
    TransponderOption transOption = options->GetTransponderOption();
    
    if(regOption != RegenerationDisabled){
        //Connect transparent segments
        std::vector<Call*> transpSeg = callDev->GetTranspSegments();
        for(auto it: transpSeg){
            this->ConnectWithoutDevices(it);
        }

        if(regOption == RegenerationVirtualized){
            std::vector<std::shared_ptr<Regenerator>> vecReg = 
            callDev->GetRegenerators();
            
            for(auto it: vecReg){
                it->SetRegeneratorOn();
            }
        }
        //else{}
        //Implement the option for back-to-back regeneration
    }
    else if(transOption == TransponderEnabled){
        this->ConnectWithoutDevices(call);
        
        std::vector<std::shared_ptr<BVT>> bvts = callDev->GetTransponders();
        
        for(auto it: bvts){
            it->ConnectSubCarriers(callDev);
        }
    }
}

void Topology::Release(Call* call) {
    
    switch(options->GetDevicesOption()){
        case DevicesDisabled:
            this->ReleaseWithoutDevices(call);
            break;
        case DevicesEnabled:
            this->ReleaseWithDevices(call);
            break;
        default:
            std::cerr << "Invalid connection used" << std::endl;
            std::abort();
    }
}

void Topology::ReleaseWithoutDevices(Call* call) {
    Link* link;
    Route* route = call->GetRoute();
    unsigned int numHops = route->GetNumHops();
    unsigned int core = call->GetCore();
    
    for(unsigned int a = 0; a < numHops; a++){
        link = route->GetLink(a);
        
        if(this->IsValidLink(link)){
            
            for(unsigned int slot = call->GetFirstSlot(); 
            slot <= call->GetLastSlot(); slot++){
                link->ReleaseSlot(core, slot);
            }
        }
    }
}

void Topology::ReleaseWithDevices(Call* call) {
    CallDevices* callDev = dynamic_cast<CallDevices*>(call);
    RegenerationOption regOption = options->GetRegenerationOption();
    TransponderOption transOption = options->GetTransponderOption();
    
    if(regOption != RegenerationDisabled){
        //Release transparent segments
        std::vector<Call*> transpSeg = callDev->GetTranspSegments();
        for(auto it: transpSeg){
            this->ReleaseWithoutDevices(it);
        }

        if(regOption == RegenerationVirtualized){
            std::vector<std::shared_ptr<Regenerator>> vecReg = 
            callDev->GetRegenerators();
            
            for(auto it: vecReg){
                it->SetRegeneratorOff();
            }
        }
        //else{}
        //Implement the option for back-to-back regeneration
        
    }
    else if(transOption == TransponderEnabled){
        this->ReleaseWithoutDevices(call);
        
        std::vector<std::shared_ptr<BVT>> bvts = callDev->GetTransponders();
        
        for(auto it: bvts){
            it->ReleaseSubCarriers(callDev);
        }
    }
}

SimulationType* Topology::GetSimulType() const {
    return simulType;
}

void Topology::SetFixedNumberOfDevices() {
    std::ifstream devicesFile;
    unsigned int auxInt;
    simulType->GetInputOutput()->LoadDevicesFile(devicesFile);
    
    devicesFile >> auxInt;
    if(options->GetRegenerationOption() == RegenerationVirtualized)
        this->SetNumDevices(auxInt, DeviceRegenerator);
    
    devicesFile >> auxInt;
    if(options->GetTransponderOption() != TransponderDisabled)
        this->SetNumDevices(auxInt, DeviceTransponder);
}

void Topology::SetNumRegenerators(unsigned int numRegenerators) {
    assert((options->GetDevicesOption() != DevicesDisabled) && 
            options->GetRegenerationOption() != RegenerationDisabled);
    
    this->numRegenerators = numRegenerators;
}

void Topology::SetNumTransponders(unsigned int numTransponders) {
    assert((options->GetDevicesOption() != DevicesDisabled) && 
           (options->GetTransponderOption() != TransponderDisabled));
    
    this->numTransponders = numTransponders;
}

void Topology::DistributeDevices(DeviceType type) {
    
    switch(type){
        case DeviceRegenerator:
            this->DistributeRegenerators();
            break;
        case DeviceTransponder:
            this->DistributeTransponders();
            break;
        default:
            std::cerr << "Invalid device type" << std::endl;
            std::abort();
    }
}

void Topology::DistributeRegenerators() {
    unsigned int numRegPerNode;
    
    switch(options->GetRegPlacOption()){
        case RegPlacUniform:
            numRegPerNode = numRegenerators / numNodes;
            
            for(unsigned a = 0; a < numNodes; a++){
                NodeDevices* node = dynamic_cast<NodeDevices*>(vecNodes.at(a)
                                                                       .get());
                node->SetNumRegenerator(numRegPerNode);
            }
            break;
        default:
            std::cerr << "Invalid regenerators distribution" << std::endl;
            std::abort();
    }
}

void Topology::DistributeTransponders() {
    unsigned int numTransPerNode;
    
    numTransPerNode = numTransponders / numNodes;
    
    for(unsigned a = 0; a < numNodes; a++){
        NodeDevices* node = dynamic_cast<NodeDevices*>(vecNodes.at(a).get());
        node->SetNumTransponder(numTransPerNode);
    }
}
