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
    topology = simulType->GetTopology();
    traffic = simulType->GetTraffic();
    options = simulType->GetOptions();
    
    this->CreateRouting();
    this->CreateSpecAllocation();
    
    modulation = std::make_shared<Modulation>(this, simulType->GetParameters()
    ->GetSlotBandwidth(), simulType->GetParameters()->GetNumberPolarizations());
    
    resources = std::make_shared<Resources>(this, modulation.get());
    
    resourAllocOption = this->options->GetResourAllocOption();
    phyLayerOption = this->options->GetPhyLayerOption();
    
    unsigned int numNodes = this->topology->GetNumNodes();
    resources->allRoutes.resize(numNodes*numNodes);
    
    routing->Load();
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
            this->resources->CreateRegenerationResources();
        }
        else if(options->GetResourAllocOption() == ResourAllocRMSA){
            this->resources->CreateOfflineModulation();
            this->resources->Save(); //Retirar depois (Markov)
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
    
    if(this->CheckResourceAllocOrder(call) == r_sa)
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
    call->RepeatModulation();
    unsigned int numRoutes = call->GetNumRoutes();
    
    for(unsigned int a = 0; a < numRoutes; a++){
        call->SetRoute(call->GetRoute(a));
        call->SetModulation(call->GetModulation(a));
        this->modulation->SetModulationParam(call);
        
        if(!this->CheckOSNR(call))
            continue;
            
        this->specAlloc->SpecAllocation(call);
            
        if(this->topology->IsValidLigthPath(call)){
            call->SetStatus(Accepted);
            break;
        }
    }
    call->ClearTrialRoutes();
    call->ClearTrialModulations();
}

void ResourceAlloc::SpecRouting(Call* call) {
    this->routing->RoutingCall(call);
    call->RepeatModulation();
    
    bool allocFound = false;
    unsigned int numRoutes = call->GetNumRoutes();
    const unsigned int topNumSlots = topology->GetNumSlots();
    std::vector<unsigned int> possibleSlots(0);
    possibleSlots = this->specAlloc->SpecAllocation();
    unsigned int auxSlot;
    
    for(unsigned int a = 0; a < possibleSlots.size(); a++){
        auxSlot = possibleSlots.at(a);
        
        for(unsigned int b = 0; b < numRoutes; b++){
            call->SetRoute(call->GetRoute(b));
            call->SetModulation(call->GetModulation(b));
            this->modulation->SetModulationParam(call);
            
            if(auxSlot + call->GetNumberSlots() - 1 >= topNumSlots)
                continue;
            
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
    
    call->PushTrialModulations(resources->GetOfflineModulationFormats(call));
    this->RSA(call);
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
            this->CreateRsaOrder();
            this->routing->BSR();
            break;
        default:
            std::cerr << "Invalid offline routing option" << std::endl;
            std::abort();
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

Topology* ResourceAlloc::GetTopology() const {
    return topology;
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

std::vector<ResAllocOrder> ResourceAlloc::GetResourceAllocOrder() const {
    return resources->resourceAllocOrder;
}

void ResourceAlloc::SetResourceAllocOrder(std::vector<ResAllocOrder> 
resourceAllocOrder) {
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
                vecBool.push_back(r_sa);
                continue;
            }
            auxRoute = resources->GetRoutes(orNode, deNode).front().get();
            
            if(auxRoute->GetNumHops() <= maxNumHops)
                vecBool.push_back(r_sa);
            else
                vecBool.push_back(sa_r);
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
    std::vector<std::shared_ptr<Route>> routes = resources->GetRoutes(orNode, 
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

void ResourceAlloc::SetNumSlotsTraffic() {
    this->resources->numSlotsTraffic = this->modulation->GetPossibleSlots(
    this->traffic->GetVecTraffic());
}

void ResourceAlloc::CreateRouting() {
    RoutingOption option = simulType->GetOptions()->GetRoutingOption();
            
    routing = std::make_shared<Routing>(this, option, simulType->GetData(), 
                                        simulType->GetParameters());
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

void ResourceAlloc::CreateRsaOrder() {
    unsigned int numNodes = this->topology->GetNumNodes();
    resources->resourceAllocOrder.resize(numNodes * numNodes);
    
    switch(this->simulType->GetOptions()->GetOrderRSA()){
        case OrderRoutingSa:
            resources->resourceAllocOrder.assign(numNodes*numNodes, r_sa);
            break;
        case OrderSaRouting:
            resources->resourceAllocOrder.assign(numNodes*numNodes, sa_r);
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

void ResourceAlloc::UpdateRoutesCosts() {
    
    for(auto it: this->resources->allRoutes){
        for(auto it2: it){
            
            if(it2 == nullptr)
                continue;
            it2->SetCost();
        }
    }
}
