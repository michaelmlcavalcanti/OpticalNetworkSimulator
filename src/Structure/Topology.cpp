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
#include "../../include/Structure/Node.h"
#include "../../include/Structure/NodeDevices.h"
#include "../../include/Structure/Link.h"
#include "../../include/Structure/Core.h"
#include "../../include/Structure/MultiCoreLink.h"
#include "../../include/Structure/Devices/Regenerator.h"
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
:simulType(simulType), vecNodes(0), vecLinks(0), numNodes(0), numLinks(0), 
numSlots(0), numCores(0), maxLength(0.0), numRegenerators(0) {

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
    
    this->simulType->GetInputOutput()->LoadTopology(auxIfstream);
    
    auxIfstream >> auxInt;
    this->SetNumNodes(auxInt);
    auxIfstream >> auxInt;
    this->SetNumLinks(auxInt);
    this->SetNumSlots(this->simulType->GetParameters()->GetNumberSlots());
    this->SetNumCores(this->simulType->GetParameters()->GetNumberCores());
        
    this->CreateNodes(auxIfstream);
    
    this->CreateLinks(auxIfstream);
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
        
        if(this->simulType->GetOptions()->GetDevicesOption() == DevicesEnabled)
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
    unsigned int numCores = this->GetNumCores();
    
    for(unsigned int a = 0; a < this->GetNumLinks(); ++a){
        ifstream >> orNode;
        ifstream >> deNode;
        ifstream >> length;
        ifstream >> nSec;
        std::shared_ptr<Link> link;
        
        if(numCores > 1){
            link = std::make_shared<MultiCoreLink>(this, orNode, deNode, 
            length, nSec, this->GetNumSlots());
        }
        else{
            link = std::make_shared<Link>(this, orNode, deNode, 
            length, nSec, this->GetNumSlots());
        }
                  
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

unsigned int Topology::GetNumRegenerators() const {
    return numRegenerators;
}

void Topology::SetNumRegenerators(unsigned int numRegenerators) {
    assert((this->simulType->GetOptions()->GetDevicesOption() != 
           DevicesDisabled) && this->simulType->GetOptions()->
           GetRegenerationOption() != RegenerationDisabled);
    
    this->numRegenerators = numRegenerators;
    this->DistributeRegenerators();
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
}

void Topology::SetAllLinksWorking() {
    
    for(auto it: this->vecLinks){
        if(it != nullptr){
            it->SetLinkState(true);
        }
    }
}

void Topology::SetLinksIniCost() {
    
    switch(this->simulType->GetOptions()->GetLinkCostType()){
        case MinHops:
            for(auto it : vecLinks){
                if(it == nullptr)
                    continue;
                it->SetCost(1.0);
            }
            break;
        case MinLength:
            for(auto it : vecLinks){
                if(it == nullptr)
                    continue;
                it->SetCost(it->GetLength());
            }
            break;
        case MinLengthNormalized:
            for(auto it : vecLinks){
                if(it == nullptr)
                    continue;
                it->SetCost(it->GetLength()/this->GetMaxLength());
            }
            break;
        default:
            for(auto it : vecLinks){
                if(it == nullptr)
                    continue;
                it->SetCost(Def::Max_Double);
            }
    }
}

Node* Topology::GetNode(unsigned int index) const {
    assert(index < this->GetNumNodes());
    return this->vecNodes.at(index).get();
}

Link* Topology::GetLink(unsigned int indexOrNode, 
unsigned int indexDeNode) const {
    assert(indexOrNode < this->GetNumNodes());
    assert(indexDeNode < this->GetNumNodes());
    
    return this->vecLinks.at(indexOrNode * this->numNodes + 
    indexDeNode).get();
}

std::shared_ptr<Link> Topology::GetLinkPointer(unsigned int indexOrNode, 
                                               unsigned int indexDeNode) const {
    assert(indexOrNode < this->GetNumNodes());
    assert(indexDeNode < this->GetNumNodes());
    
    return this->vecLinks.at(indexOrNode * this->numNodes + indexDeNode);
}

bool Topology::CheckSlotDisp(Route* route, unsigned int slot) 
const {
    Link* link;
    unsigned int numHops = route->GetNumHops();
    
    for(unsigned int a = 0; a < numHops; a++){
        link = route->GetLink(a);
        
        if(link->IsSlotOccupied(slot))
            return false;
    }
    return true;
}

bool Topology::CheckSlotsDisp(Route* route,unsigned int iniSlot, 
unsigned int finSlot) const {
    
    for(unsigned int a = iniSlot; a <= finSlot; a++){
        if(!this->CheckSlotDisp(route, a))
            return false;
    }
    
    return true;
}

bool Topology::CheckSlotsDispCore(Route* route, unsigned int iniSlot,
unsigned int finSlot, unsigned int core) const {
    
    unsigned int L_or, L_de, x = route->GetNumHops();
    L_or = route->GetNodeId(0);L_de = route->GetNodeId(1);
    bool flag = false;
    //Cast base pointer in derived pointer class
    std::shared_ptr<MultiCoreLink> link = 
    std::dynamic_pointer_cast<MultiCoreLink>(this->GetLinkPointer(L_or, L_de));
    //Check the availability of the set of slots in the core on the first hop
    for(unsigned int s = iniSlot; s <= finSlot; s++){
        // is link c->c+1 busy in slot s?
        if(link->GetCore(core)->IsSlotOccupied(s)){
            flag = true;
            break;
        }
        // found the core in the first link
        if(s == finSlot){                
            break;
        }
    }
        if(flag == true){
            return false;
        }
    //Check the availability of the set of slots in the core on the rest of the 
    //route
    for(unsigned int c = 1; c < route->GetNumHops(); c++){
        L_or = route->GetNodeId(c);L_de = route->GetNodeId(c+1);
        std::shared_ptr<MultiCoreLink> link = 
        std::dynamic_pointer_cast<MultiCoreLink>(this->GetLinkPointer(L_or,
        L_de));
        for(unsigned int s = iniSlot; s <= finSlot; s++){
            if(link->GetCore(core)->IsSlotOccupied(s))
                return false;
        }
    }
    return true;
}

bool Topology::CheckBlockSlotsDisp(Route* route, unsigned int numSlots) const {
    unsigned int numContiguousSlots = 0;

    for(unsigned int s = 0; s < this->numSlots; s++){
        if(this->CheckSlotDisp(route, s))
            numContiguousSlots++;
        else
            numContiguousSlots = 0;
        if(numContiguousSlots == numSlots)
            return true;
    }
    return false;
}

bool Topology::CheckOSNR(const Route* route, double OSNRth) {
    Link* link;
    unsigned int numHops = route->GetNumHops();
    std::shared_ptr<Signal> signal = std::make_shared<Signal>();
    
    for(unsigned int a = 0; a < numHops; a++){
        link = route->GetLink(a);
        link->CalcSignal(signal.get());
    }
    
    if(signal->GetOSNR() > OSNRth)
        return true;
    return false;
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

bool Topology::IsValidSlot(unsigned int index) {
    if(index >= 0 && index < this->numSlots)
        return true;
    return false;
}

bool Topology::IsValidLigthPath(Call* call) {
    if( (this->IsValidRoute(call->GetRoute())) && 
        (call->GetFirstSlot() <= call->GetLastSlot()) && 
        (this->IsValidSlot(call->GetFirstSlot())) && 
        (this->IsValidSlot(call->GetLastSlot())) ){
        return true;
    }
    
    return false;
}

bool Topology::IsValidLigthPath(CallDevices* call) {
    std::vector<Call*> calls = call->GetTranspSegments();
    
    for(auto it: calls){
        if(!this->IsValidLigthPath(it))
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
            auxVecReg = auxNode->GetFreeRegenenerators(calls.at(a)->GetBitRate());
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
    
    switch(simulType->GetOptions()->GetDevicesOption()){
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
    unsigned int numHops = route->GetNumHops(), core;
    
    for(unsigned int a = 0; a < numHops; a++){
        link = route->GetLink(a);
        if(this->IsValidLink(link)){
            //Condition to connect the call- MultiCore or SingleCore
            if(this->numCores == 1)
                for(unsigned int s = call->GetFirstSlot(); s <= call->
                GetLastSlot(); s++){
                    link->OccupySlot(s);
                }
            else{
                MultiCoreLink* mcLink = static_cast<MultiCoreLink *>(link);
                core = call->GetCore();
                for(unsigned int s = call->GetFirstSlot(); s <= call->
                GetLastSlot(); s++){
                    mcLink->OccupySlot(core, s);
                }
            }
        }
    }
}

void Topology::ConnectWithDevices(Call* call) {
    CallDevices* callDev = dynamic_cast<CallDevices*>(call);
    RegenerationOption regOption = simulType->GetOptions()
                                            ->GetRegenerationOption();
    
    if(regOption == RegenerationEnabled){
        //Connect transparent segments
        std::vector<Call*> transpSeg = callDev->GetTranspSegments();
        for(auto it: transpSeg){
            this->ConnectWithoutDevices(it);
        }

        //Connect Regenerators
        std::vector<std::shared_ptr<Regenerator>> vecReg = callDev->
                                                           GetRegenerators();
        for(auto it: vecReg){
            it->SetRegeneratorOn();
        }
    }
}

void Topology::Release(Call* call) {
    
    switch(simulType->GetOptions()->GetDevicesOption()){
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
    unsigned int numHops = route->GetNumHops(), core;
    
    for(unsigned int a = 0; a < numHops; a++){
        link = route->GetLink(a);
        
        if(this->IsValidLink(link)){
            //Condition to release the call- MultiCore or SingleCore
            if(this->numCores == 1)
                for(unsigned int s = call->GetFirstSlot(); s <= call->
                GetLastSlot(); s++){
                    link->ReleaseSlot(s);
                }
            else{
                MultiCoreLink* mcLink = static_cast<MultiCoreLink *>(link);
                core = call->GetCore();
                for(unsigned int s = call->GetFirstSlot(); s <= call->
                GetLastSlot(); s++){
                    mcLink->ReleaseSlot(core, s);
                }
            }
        }
    }
}

void Topology::ReleaseWithDevices(Call* call) {
    CallDevices* callDev = dynamic_cast<CallDevices*>(call);
    RegenerationOption regOption = simulType->GetOptions()
                                            ->GetRegenerationOption();
    
    if(regOption == RegenerationEnabled){
        //Release transparent segments
        std::vector<Call*> transpSeg = callDev->GetTranspSegments();
        for(auto it: transpSeg){
            this->ReleaseWithoutDevices(it);
        }

        //Release Regenerators
        std::vector<std::shared_ptr<Regenerator>> vecReg = callDev->
                                                           GetRegenerators();
        for(auto it: vecReg){
            it->SetRegeneratorOff();
        }
    }
}

SimulationType* Topology::GetSimulType() const {
    return simulType;
}

void Topology::DistributeRegenerators() {
    unsigned int numRegPerNode;
    
    switch(this->simulType->GetOptions()->GetRegPlacOption()){
        case RegPlacUniform:
            numRegPerNode = numRegenerators / numNodes;
            
            for(unsigned a = 0; a < numNodes; a++){
                NodeDevices* node = 
                dynamic_cast<NodeDevices*>(vecNodes.at(a).get());
                node->SetNumRegenerator(numRegPerNode);
            }
            break;
        default:
            std::cerr << "Invalid regenerators distribution" << std::endl;
            std::abort();
    }
}
