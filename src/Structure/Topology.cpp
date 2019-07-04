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
#include "../../include/Structure/Devices/BVT.h"
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
    unsigned int numCores = this->GetNumCores();
    double maxSecLength = simulType->GetParameters()->GetMaxSectionLegnth();
    
    for(unsigned int a = 0; a < this->GetNumLinks(); ++a){
        ifstream >> orNode;
        ifstream >> deNode;
        ifstream >> length;
        nSec = std::ceil(length / maxSecLength);
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
        auxVecBVT = orNode->GetBVTs(numSlots);
        vecBVT.insert(vecBVT.end(), auxVecBVT.begin(), auxVecBVT.end());
        auxVecBVT = deNode->GetBVTs(numSlots);
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
            //Connect BVT
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
    RegenerationOption regOption = options->GetRegenerationOption();
    TransponderOption transOption = options->GetTransponderOption();
    
    if(regOption != RegenerationDisabled){
        //Release transparent segments
        std::vector<Call*> transpSeg = callDev->GetTranspSegments();
        for(auto it: transpSeg){
            this->ReleaseWithoutDevices(it);
        }

        if(regOption == RegenerationVirtualized){
            //Release Regenerators
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
            //Release BVT
        }
    }
}

SimulationType* Topology::GetSimulType() const {
    return simulType;
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
