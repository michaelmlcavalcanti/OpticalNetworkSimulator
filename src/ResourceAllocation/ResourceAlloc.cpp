/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ResourceAlloc.cpp
 * Author: BrunoVinicius
 * 
 * Created on November 27, 2018, 8:33 PM
 */

#include "../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Structure/Topology.h"
#include "../../include/ResourceAllocation/Routing.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/ResourceAllocation/SA.h"
#include "../../include/ResourceAllocation/CSA.h"
#include "../../include/ResourceAllocation/Modulation.h"
#include "../../include/ResourceAllocation/Resources.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Data/Options.h"
#include "../../include/Calls/Call.h"
#include "../../include/Calls/Traffic.h"
#include "../../include/GeneralClasses/Def.h"

ResourceAlloc::ResourceAlloc(SimulationType *simulType)
:topology(nullptr), traffic(nullptr), options(nullptr), simulType(simulType),
routing(nullptr), specAlloc(nullptr) {
    
}

ResourceAlloc::~ResourceAlloc() {
    
    for(auto it1 : this->resources->allRoutes){
        for(auto it2 : it1){
            it2.reset();
        }
    }
    this->resources->allRoutes.clear();
    
    this->routing.reset();
    this->specAlloc.reset();
}

void ResourceAlloc::Load() {
    this->topology = this->simulType->GetTopology();
    this->traffic = this->simulType->GetTraffic();
    this->options = this->simulType->GetOptions();
    
    this->CreateRouting();
    this->CreateSpecAllocation();
    
    this->modulation = std::make_shared<Modulation>(this, 
    this->simulType->GetParameters()->GetSlotBandwidth());
    
    this->resources = std::make_shared<Resources>(this, modulation.get());
    
    this->resourAllocOption = this->options->GetResourAllocOption();
    this->phyLayerOption = this->options->GetPhyLayerOption();
    
    unsigned int numNodes = this->topology->GetNumNodes();
    this->resources->allRoutes.resize(numNodes*numNodes);
}

void ResourceAlloc::AdditionalSettings() {
    
    if(this->IsOfflineRouting()){
        this->RoutingOffline();
        this->UpdateRoutesCosts();
        
        if(this->CheckInterRouting()){
            this->SetInterferingRoutes();
            this->SetNumInterRoutesToCheck();
            this->SetNumSlotsTraffic();
        }
        
        if(this->options->GetRegenerationOption() != RegenerationDisabled){
            assert(options->GetPhyLayerOption() == PhyLayerEnabled);
            assert(options->GetResourAllocOption() == ResourAllocRMSA);
            this->resources->CreateRegResources();
        }
        else if(options->GetResourAllocOption() == ResourAllocRMSA){
            this->resources->CreateOfflineModulation();
            this->resources->Save(); //Retirar depois.
        }
    }
    this->CreateRsaOrder();
}

void ResourceAlloc::ResourAlloc(Call* call) {
    
    switch(this->resourAllocOption){
        case ResourAllocRSA:
            call->PushTrialModulation(FixedModulation);
            this->RSA(call);
            break;
        case ResourAllocRMSA:
            this->RMSA(call);
            break;
        default:
            std::cerr << "Invalid resource allocation option" << std::endl;
            std::abort();
    }
    
    if(call->GetStatus() == NotEvaluated)
        call->SetStatus(Blocked);
}

void ResourceAlloc::RSA(Call* call) {
    
    if(!this->CheckResourceAllocOrder(call))
        this->RoutingSpec(call);
    else
        this->SpecRouting(call);
}

void ResourceAlloc::RMSA(Call* call) {
    
    if(this->IsOfflineRouting())
        this->OfflineModulationRSA(call);
    else
        this->OnlineModulationRSA(call);
}

void ResourceAlloc::RoutingSpec(Call* call) {
    this->routing->RoutingCall(call);
    call->UpdateTrialModulations();
    
    if(call->IsThereTrialRoute()){
        do{
            call->SetRoute(call->PopTrialRoute());
            call->SetModulation(call->PopTrialModulation());
            this->modulation->SetModulationParam(call);
            
            if(!this->CheckOSNR(call))
                continue;
            
            this->specAlloc->SpecAllocation(call);
            
                if(this->topology->IsValidLigthPath(call)){
                    call->ClearTrialRoutes();
                    call->ClearTrialModulations();
                    call->SetStatus(Accepted);
                    break;
                }
        }while(call->IsThereTrialRoute());
    }
}

void ResourceAlloc::SpecRouting(Call* call) {
    this->routing->RoutingCall(call);
    
    bool allocFound = false;
    unsigned int lastPossibleSlot = this->topology->GetNumSlots() - 
                                    call->GetNumberSlots();
    unsigned int numRoutes = call->GetNumRoutes();
    std::vector<unsigned int> possibleSlots(0);
    //Corrigir essa parte, que depende da modulação já escolhida.
    possibleSlots = this->specAlloc->SpecAllocation(lastPossibleSlot);
    unsigned int auxSlot;
    
    for(unsigned int a = 0; a < possibleSlots.size(); a++){
        auxSlot = possibleSlots.at(a);
        
        for(unsigned int b = 0; b < numRoutes; b++){
            call->SetRoute(call->GetRoute(b));
            call->SetModulation(call->GetModulation(b));
            this->modulation->SetModulationParam(call);
            
            if(!this->CheckOSNR(call))
                continue;
            
            if(this->topology->CheckSlotsDisp(call->GetRoute(), auxSlot, 
            auxSlot + call->GetNumberSlots() - 1)){
                call->SetFirstSlot(auxSlot);
                call->SetLastSlot(auxSlot + call->GetNumberSlots() - 1);
                call->ClearTrialModulations();
                call->ClearTrialRoutes();
                call->SetStatus(Accepted);
                allocFound = true;
                break;
            }
        }
        
        if(allocFound)
            break;
    }
}

void ResourceAlloc::OnlineModulationRSA(Call* call) {
    TypeModulation mod;
    
    for(mod = LastModulation; mod >= FirstModulation; 
                              mod = TypeModulation(mod-1)){
        call->PushTrialModulation(mod);
        
        this->RSA(call);
        
        if(call->GetStatus() == Accepted)
            break;
    }
}

void ResourceAlloc::OfflineModulationRSA(Call* call) {
    
    call->PushTrialModulations(resources->GetModulationFormat(call));
    this->RSA(call);
}

void ResourceAlloc::SetRoute(unsigned int orN, unsigned int deN, 
std::shared_ptr<Route> route) {
    this->ClearRoutes(orN, deN);
    this->AddRoute(orN, deN, route);
}

void ResourceAlloc::SetRoutes(unsigned int orN, unsigned int deN, 
std::vector<std::shared_ptr<Route>> routes) {
    this->ClearRoutes(orN, deN);
    
    for(auto it : routes)
        this->AddRoute(orN, deN, it);
}

void ResourceAlloc::AddRoute(unsigned int orN, unsigned int deN, 
std::shared_ptr<Route> route) {
    this->resources->allRoutes.at(orN*this->topology->GetNumNodes() + deN)
                   .push_back(route);
}

void ResourceAlloc::AddRoutes(unsigned int orN, unsigned int deN, 
std::vector<std::shared_ptr<Route>> routes) {
    
    for(auto it : routes)
        this->AddRoute(orN, deN, it);
}

void ResourceAlloc::ClearRoutes(unsigned int orN, unsigned int deN) {
    
    for(auto it : this->resources->allRoutes.at(
    orN*this->topology->GetNumNodes() + deN))
        it.reset();
    
    this->resources->allRoutes.at(orN*this->topology->GetNumNodes() + deN).
                               clear();
}

std::vector<std::shared_ptr<Route>> ResourceAlloc::GetRoutes(unsigned int orN, 
unsigned int deN) {
    unsigned int numNodes = this->topology->GetNumNodes();
    
    return this->resources->allRoutes.at(orN*numNodes + deN);
}

bool ResourceAlloc::IsOfflineRouting() {
    switch(this->routing->GetRoutingOption()){
        case RoutingDJK:
        case RoutingYEN:
        case RoutingBSR:
            return true;
        default:
            return false;
    }
}

void ResourceAlloc::RoutingOffline() {
    switch(this->routing->GetRoutingOption()){
        case RoutingDJK:
            this->routing->Dijkstra();
            break;
        case RoutingYEN:
            this->routing->YEN();
            break;
        case RoutingBSR:
        default:
            std::cerr << "Invalid offline routing option" << std::endl;
    }
}

bool ResourceAlloc::CheckInterRouting() {
    
    switch(this->GetSimulType()->GetOptions()->GetSpecAllOption()){
        case SpecAllMSCL:
            return true;
        default:
            return false;
    }
}

bool ResourceAlloc::CheckOSNR(Call* call) {
    
    if(this->phyLayerOption == PhyLayerEnabled)
        if(!this->topology->CheckOSNR(call->GetRoute(), call->GetOsnrTh()))
            return false;
    
    return true;
}

bool ResourceAlloc::CheckResourceAllocOrder(Call* call) {
    return this->resources->resourceAllocOrder.at( call->GetOrNode()->
    GetNodeId()*this->topology->GetNumNodes()+call->GetDeNode()->GetNodeId() );
}

SimulationType* ResourceAlloc::GetSimulType() const {
    return simulType;
}

void ResourceAlloc::SetSimulType(SimulationType* simulType) {
    this->simulType = simulType;
}

Topology* ResourceAlloc::GetTopology() const {
    return topology;
}

void ResourceAlloc::SetTopology(Topology* topology) {
    this->topology = topology;
}

Resources* ResourceAlloc::GetResources() const {
    return resources.get();
}

Modulation* ResourceAlloc::GetModulation() const {
    return modulation.get();
}

Traffic* ResourceAlloc::GetTraffic() const {
    return traffic;
}

ResourceAllocOption ResourceAlloc::GetResourAllocOption() const {
    return resourAllocOption;
}

std::vector<bool> ResourceAlloc::GetResourceAllocOrder() const {
    return resources->resourceAllocOrder;
}

void ResourceAlloc::SetResourceAllocOrder(std::vector<bool> resourceAllocOrder) {
    assert(resourceAllocOrder.size() == this->resources->
                                        resourceAllocOrder.size());
    
    this->resources->resourceAllocOrder = resourceAllocOrder;
}

void ResourceAlloc::SetResourceAllocOrder() {
    std::ifstream auxIfstream;
    std::vector<bool> vecBool;
    bool auxBool;
    unsigned int numNodes = this->topology->GetNumNodes();
    this->simulType->GetInputOutput()->LoadRsaOrderFirstSimul(auxIfstream);
    
    for(unsigned int a = 0; a < numNodes*numNodes; a++){
        auxIfstream >> auxBool;
        vecBool.push_back(auxBool);
    }
    this->SetResourceAllocOrder(vecBool);
}

void ResourceAlloc::SetResAllocOrderHeuristicsRing() {
    assert(this->simulType->GetOptions()->GetTopologyOption() == TopologyRing);
    unsigned int numNodes = this->topology->GetNumNodes();
    std::vector<bool> vecBool;
    Route* auxRoute;
    unsigned int maxNumHops = 4;
    
    for(unsigned int orNode = 0; orNode < numNodes; orNode++){
        for(unsigned int deNode = 0; deNode < numNodes; deNode++){
            
            if(orNode == deNode){
                vecBool.push_back(false);
                continue;
            }
            auxRoute = this->GetRoutes(orNode, deNode).front().get();
            
            if(auxRoute->GetNumHops() <= maxNumHops)
                vecBool.push_back(false);
            else
                vecBool.push_back(true);
        }
    }
    
    assert(vecBool.size() == numNodes*numNodes);
    this->SetResourceAllocOrder(vecBool);
}

std::vector<std::shared_ptr<Route>> ResourceAlloc::GetInterRoutes(int ori, 
int des, int pos) {
    return this->resources->interRoutes.at(ori*(this->topology->GetNumNodes()) 
    + des).at(pos);
}

std::vector<std::shared_ptr<Route>> ResourceAlloc::GetInterRoutes(
unsigned int orNode, unsigned int deNode, Route* route){
    std::vector<std::shared_ptr<Route>> routes = this->GetRoutes(orNode, 
                                                                 deNode);
    unsigned int numRoutes = routes.size();
    
    for(unsigned int pos = 0; pos < numRoutes; pos++){
        
        if(route == routes.at(pos).get())
            return this->GetInterRoutes(orNode, deNode, pos);
    }
    
    routes.clear();
    return routes;
}

void ResourceAlloc::SetInterferingRoutes() {
    std::shared_ptr<Route> routeAux, interRoute;
    Link *auxLink, *interLink;
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int auxIndex;
    bool addroute = true;
    
    this->resources->interRoutes.resize(this->resources->allRoutes.size());
    
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            if(orN == deN)
                continue;
            auxIndex = (orN * numNodes) + deN;
            this->resources->interRoutes.at(auxIndex).resize(
            this->resources->allRoutes.at(auxIndex).size());
        }
    }
    
    for(unsigned int a = 0; a < this->resources->allRoutes.size(); a++){
        for(unsigned int b = 0; b < this->resources->allRoutes.at(a).size(); 
        b++){
            routeAux = this->resources->allRoutes.at(a).at(b);
            
            if(routeAux == nullptr)
                continue;
            
            for(unsigned int c = 0; c < routeAux->GetNumHops(); c++){
                auxLink = routeAux->GetLink(c);
                
                for(unsigned int d = 0; d < this->resources->allRoutes.size(); 
                d++){
                    for(unsigned int e = 0; e < this->resources->allRoutes.
                    at(d).size(); e++){
                        interRoute = this->resources->allRoutes.at(d).at(e);
                        
                        if(interRoute == nullptr || interRoute == routeAux)
                            continue;
                        
                        for(unsigned int f = 0; f < interRoute->GetNumHops();
                        f++){
                            interLink = interRoute->GetLink(f);
                            
                            if(auxLink == interLink){
                                
                                for(unsigned int g = 0; g < this->resources->
                                interRoutes.at(a).at(b).size(); g++){
                                    
                                    if(this->resources->interRoutes.at(a).at(b).
                                    at(g) == interRoute){
                                        addroute = false;
                                        break;
                                    }
                                }
                                if(addroute){
                                    this->resources->interRoutes.at(a).at(b).
                                    push_back(interRoute);
                                }
                                addroute = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    
    for(unsigned int a = 0; a < this->resources->interRoutes.size(); a++){
        for(unsigned int b = 0; b < this->resources->interRoutes.at(a).size(); 
        b++){
            std::sort(this->resources->interRoutes.at(a).at(b).begin(), 
                      this->resources->interRoutes.at(a).at(b).end(), 
                      RouteCompare());
        }
    }
}

std::vector<std::vector<unsigned int> > 
ResourceAlloc::GetNumInterRoutesToCheck() {
    return resources->numInterRoutesToCheck;
}

unsigned int ResourceAlloc::GetNumInterRoutesToCheck(unsigned int orNode, 
unsigned int deNode, Route* route) {
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int vecIndex = orNode*numNodes + deNode;
    unsigned int numRoutes = 0;
    std::vector<unsigned int> vecNumInterRoutes = 
    this->resources->numInterRoutesToCheck.at(vecIndex);
    
    for(unsigned int pos = 0; pos < vecNumInterRoutes.size(); pos++){
        
        if(route == this->resources->allRoutes.at(vecIndex).at(pos).get()){
            numRoutes = this->resources->numInterRoutesToCheck.at(vecIndex).
            at(pos);
            break;
        }
    }
    
    return numRoutes;
}

void ResourceAlloc::SetNumInterRoutesToCheck() {
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int numRoutes;
    this->resources->numInterRoutesToCheck.resize(this->resources->
                                                  interRoutes.size());
    
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            numRoutes = this->resources->interRoutes.at(orN*numNodes+deN)
                                                    .size();
            
            for(unsigned int pos = 0; pos < numRoutes; pos++){
                this->resources->numInterRoutesToCheck.at( orN*numNodes+deN)
                .push_back(this->resources->interRoutes.at(orN*numNodes+deN)
                .at(pos).size() );
            }
        }
    }
}

void ResourceAlloc::SetNumInterRoutesToCheck(
std::vector<std::vector<unsigned int>> numInterRoutesToCheck) {
    this->resources->numInterRoutesToCheck = numInterRoutesToCheck;
}

std::vector<unsigned int> ResourceAlloc::GetNumSlotsTraffic() const {
    return resources->numSlotsTraffic;
}

void ResourceAlloc::UpdateRoutesCosts() {
    
    for(auto it: this->resources->allRoutes){
        for(auto it2: it){
            
            if(it2 == nullptr)
                continue;
            it2->SetCost();
        }
    }
}

void ResourceAlloc::SetNumSlotsTraffic() {
    this->resources->numSlotsTraffic = this->modulation->GetPossibleSlots(
    this->traffic->GetVecTraffic());
}

void ResourceAlloc::CreateRsaOrder() {
    unsigned int numNodes = this->topology->GetNumNodes();
    this->resources->resourceAllocOrder.resize(numNodes * numNodes);
    
    switch(this->simulType->GetOptions()->GetOrderRSA()){
        case OrderRoutingSa:
            this->resources->resourceAllocOrder.assign(numNodes*numNodes, r_sa);
            break;
        case OrderSaRouting:
            this->resources->resourceAllocOrder.assign(numNodes*numNodes, sa_r);
            break;
        case MixedOrder:
            this->SetResourceAllocOrder();
            break;
        case HeuristicsOrder:
            this->SetResAllocOrderHeuristicsRing();
            break;
        default:
            std::cout << "Invalid RSA order" << std::endl;
            abort();
    }
}

void ResourceAlloc::CreateRouting() {
    RoutingOption option = this->simulType->GetOptions()->GetRoutingOption();
            
    this->routing = std::make_shared<Routing>(this, option, this->topology);
    
    if(option == RoutingYEN){
        this->routing->SetK(this->simulType->GetParameters()->
                            GetNumberRoutes());
    }
}

void ResourceAlloc::CreateSpecAllocation() {
    SpectrumAllocationOption option = this->simulType->GetOptions()->
                                      GetSpecAllOption();
    
    if(this->topology->GetNumCores() == 1){
        this->specAlloc = std::make_shared<SA>(this, option, this->topology);
    }
    else{
        this->specAlloc = std::make_shared<CSA>(this, option, this->topology);
    }
}
