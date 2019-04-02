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
#include "../../include/Data/Parameters.h"
#include "../../include/Calls/Call.h"
#include "../../include/ResourceAllocation/Modulation.h"
#include "../../include/Data/InputOutput.h"

ResourceAlloc::ResourceAlloc(SimulationType *simulType)
:simulType(simulType), topology(nullptr), routing(nullptr), specAlloc(nullptr),
allRoutes(0), interRoutes(0), resourceAllocOrder(0), numInterRoutesToCheck(0) {
    
}

ResourceAlloc::~ResourceAlloc() {
    
    for(auto it1 : this->allRoutes){
        for(auto it2 : it1){
            it2.reset();
        }
    }
    this->allRoutes.clear();
    
    this->routing.reset();
    this->specAlloc.reset();
}

void ResourceAlloc::Load() {
    this->topology = this->simulType->GetTopology();
    unsigned int numNodes = this->topology->GetNumNodes();
    
    this->allRoutes.resize(numNodes*numNodes);
    
    //Create the RSA order vector based on the option set.
    switch(this->simulType->GetOptions()->GetOrderRSA()){
        case OrderRoutingSa:
            this->resourceAllocOrder.assign(numNodes*numNodes, false);
            break;
        case OrderSaRouting:
            this->resourceAllocOrder.assign(numNodes*numNodes, true);
            break;
        case MixedOrder:
            this->resourceAllocOrder.resize(numNodes*numNodes);
            this->SetResourceAllocOrder();
            break;
        default:
            std::cout << "Invalid RSA order" << std::endl;
    }
    
    this->routing = std::make_shared<Routing>(this,
        this->simulType->GetOptions()->GetRoutingOption(), this->topology);
    
    if(this->topology->GetNumCores() == 1){
        this->specAlloc = std::make_shared<SA>(this, this->simulType->
                          GetOptions()->GetSpecAllOption(), this->topology);
    }
    else{
        this->specAlloc = std::make_shared<CSA>(this, this->simulType->
                          GetOptions()->GetSpecAllOption(), this->topology);
    }
    
    this->modulation = std::make_shared<Modulation>(this, 
        this->simulType->GetParameters()->GetSlotBandwidth());
    
    this->resourAllocOption = this->simulType->GetOptions()->
                                               GetResourAllocOption();
    
    this->phyLayerOption = this->simulType->GetOptions()->GetPhyLayerOption();
}

void ResourceAlloc::ResourAlloc(Call* call) {
    
    switch(this->resourAllocOption){
        case ResourAllocRSA:
            call->SetModulation(FixedModulation);
            if(!this->CheckResourceAllocOrder(call))
                this->RSA(call);
            else
                this->SAR(call);
            break;
        case ResourAllocRMSA:
            this->RMSA(call);
            break;
        default:
            std::cerr << "Invalid resource allocation option" << std::endl;
    }
    
    if(call->GetStatus() == NotEvaluated)
        call->SetStatus(Blocked);
}

void ResourceAlloc::RSA(Call* call) {
    this->modulation->SetModulationParam(call);
    this->routing->RoutingCall(call);
    
    if(call->IsThereTrialRoute()){
        do{
            call->SetRoute(call->PopTrialRoute());
            
            if(!this->CheckOSNR(call))
                continue;
            
            this->specAlloc->SpecAllocation(call);
            
                if(this->topology->IsValidLigthPath(call)){
                    call->ClearTrialRoutes();
                    call->SetStatus(Accepted);
                    break;
                }
        }while(call->IsThereTrialRoute());
    }
}

void ResourceAlloc::RMSA(Call* call) {
    TypeModulation mod;
    
    for(mod = LastModulation; mod >= FirstModulation; 
                              mod = TypeModulation(mod-1)){
        call->SetModulation(mod);
        
        if(!this->CheckResourceAllocOrder(call))
            this->RSA(call);
        else
            this->SAR(call);
        
        if(call->GetStatus() == Accepted)
            break;
    }
}

void ResourceAlloc::SAR(Call* call) {
    this->modulation->SetModulationParam(call);
    this->routing->RoutingCall(call);
    bool allocFound = false;
    
    unsigned int size = this->topology->GetNumSlots() - call->GetNumberSlots();
    unsigned int numRoutes = call->GetNumRoutes();
    
    for(unsigned int a = 0; a <= size; a++){
    
        for(unsigned int b = 0; b < numRoutes; b++){
            call->SetRoute(call->GetRoute(b));
            
            if(!this->CheckOSNR(call))
                continue;
        
            if(this->topology->CheckSlotsDisp(call->GetRoute(), a, a + 
               call->GetNumberSlots() - 1)){
                call->SetFirstSlot(a);
                call->SetLastSlot(a + call->GetNumberSlots() - 1);
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
    this->allRoutes.at(orN*this->topology->GetNumNodes() + deN)
                   .push_back(route);
}

void ResourceAlloc::AddRoutes(unsigned int orN, unsigned int deN, 
std::vector<std::shared_ptr<Route>> routes) {
    
    for(auto it : routes)
        this->AddRoute(orN, deN, it);
}

void ResourceAlloc::ClearRoutes(unsigned int orN, unsigned int deN) {
    
    for(auto it : this->allRoutes.at(orN*this->topology->GetNumNodes() + deN))
        it.reset();
    
    this->allRoutes.at(orN*this->topology->GetNumNodes() + deN).clear();
}

std::vector<std::shared_ptr<Route>> ResourceAlloc::GetRoutes(unsigned int orN, 
unsigned int deN) {
    unsigned int numNodes = this->topology->GetNumNodes();
    
    return this->allRoutes.at(orN*numNodes + deN);
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
    switch(this->resourAllocOption){
        case SpecAllMSCL:
            return true;
            break;
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
    return this->resourceAllocOrder.at(call->GetOrNode()->GetNodeId()*
    this->topology->GetNumNodes()+call->GetDeNode()->GetNodeId());
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

std::vector<bool> ResourceAlloc::GetResourceAllocOrder() const {
    return resourceAllocOrder;
}

void ResourceAlloc::SetResourceAllocOrder(std::vector<bool> resourceAllocOrder) {
    assert(resourceAllocOrder.size() == this->resourceAllocOrder.size());
    
    this->resourceAllocOrder = resourceAllocOrder;
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

std::vector<std::shared_ptr<Route>> ResourceAlloc::GetInterRoutes(int ori, 
int des, int pos) {
    return this->interRoutes.at(ori*(this->topology->GetNumNodes()) + des)
           .at(pos);
}

void ResourceAlloc::SetInterferingRoutes() {
    std::shared_ptr<Route> routeAux, routeAux2;
    int nodeRoute[2], nodeRouteInt[2], countRoutes = 0;
    bool flag = true;
    
    this->interRoutes.resize(this->allRoutes.size());
    /*Initialize vector of vector of route pointer for Interfering Routes*/
    for(unsigned int r = 1; r < this->allRoutes.size() - 1; r++){
        if(r%(this->topology->GetNumNodes() + 1) == 0)
            r += 1;
        this->interRoutes.at(r).resize(this->allRoutes.at(r).size());
    }
    //this->interRoutes.resize(totalRoutes); 
    /*Vary the first position of allRoutes*/
    for(unsigned int a = 1; a < allRoutes.size() - 1; a++){
      if(a%(this->topology->GetNumNodes() + 1) == 0)
            a += 1;
      /*Vary second position of allRoutes*/
      for(unsigned int e = 0;e < allRoutes.at(a).size();e++){  
        routeAux = this->allRoutes.at(a).at(e);
        /*Search links to verify interfering routes*/
        for(unsigned int b = 0; b < routeAux->GetNumHops() - 1; b++){
            nodeRoute[0] = routeAux->GetNodeId(b);
            nodeRoute[1] = routeAux->GetNodeId(b+1);
            /*Search in allRoutes to extract all interfering routes*/
            for(unsigned int c = 1; c < allRoutes.size() - 1; c++){
              if(c%(this->topology->GetNumNodes() + 1) == 0)
                 c += 1;
              for(unsigned int f = 0;f < allRoutes.at(c).size();f++){
                routeAux2 = allRoutes.at(c).at(f);
                if(routeAux == routeAux2)
                   continue;
                for(unsigned int d = 0; d < (routeAux2->GetNumHops()-1); d++){
                   nodeRouteInt[0] = routeAux2->GetNodeId(d);
                   nodeRouteInt[1] = routeAux2->GetNodeId(d+1);
                   if(nodeRoute[0]==nodeRouteInt[0] && 
                          nodeRoute[1]==nodeRouteInt[1]){
                      /*Verify if interfering route is already in interRoutes*/
                       for(unsigned int p = 0; p < this->interRoutes.at(a).at(e)
                               .size(); p++){
                          if(this->interRoutes.at(a).at(e).at(p) == routeAux2){
                              flag = false;
                              d = routeAux2->GetNumHops()-1;
                              break;
                          }
                       }
                       if(flag){
                         this->interRoutes.at(a).at(e).push_back(routeAux2);
                         break;
                       }
                       flag = true;
                   }
                }
              }
            }
        }
        countRoutes++;
      }
    }
}

void ResourceAlloc::SetInterferingRoutes2() {
    std::shared_ptr<Route> routeAux, interRoute;
    Link *auxLink, *interLink;
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int auxIndex;
    bool addroute = true;
    
    this->interRoutes.resize(this->allRoutes.size());
    
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            if(orN == deN)
                continue;
            auxIndex = (orN * numNodes) + deN;
            this->interRoutes.at(auxIndex).resize(
            this->allRoutes.at(auxIndex).size());
        }
    }
    
    for(unsigned int a = 0; a < this->allRoutes.size(); a++){
        for(unsigned int b = 0; b < this->allRoutes.at(a).size(); b++){
            routeAux = this->allRoutes.at(a).at(b);
            
            if(routeAux == nullptr)
                continue;
            
            for(unsigned int c = 0; c < routeAux->GetNumHops(); c++){
                auxLink = routeAux->GetLink(c);
                
                for(unsigned int d = 0; d < this->allRoutes.size(); d++){
                    for(unsigned int e = 0; e < this->allRoutes.at(d).size(); 
                    e++){
                        interRoute = this->allRoutes.at(d).at(e);
                        
                        if(interRoute == nullptr || interRoute == routeAux)
                            continue;
                        
                        for(unsigned int f = 0; f < interRoute->GetNumHops(); 
                        f++){
                            interLink = interRoute->GetLink(f);
                            
                            if(auxLink == interLink){
                                
                                for(unsigned int g = 0; g < 
                                this->interRoutes.at(a).at(b).size(); g++){
                                    
                                    if(this->interRoutes.at(a).at(b).at(g) == 
                                    interRoute){
                                        addroute = false;
                                        break;
                                    }
                                }
                                if(addroute){
                                    this->interRoutes.at(a).at(b).push_back(
                                    interRoute);
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
}

std::vector<std::vector<unsigned int> > 
ResourceAlloc::GetNumInterRoutesToCheck() {
    return numInterRoutesToCheck;
}

void ResourceAlloc::SetNumInterRoutesToCheck() {
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int numRoutes;
    this->numInterRoutesToCheck.resize(this->interRoutes.size());
    
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            numRoutes = this->interRoutes.at(orN*numNodes+deN).size();
            
            for(unsigned int pos = 0; pos < numRoutes; pos++){
                this->numInterRoutesToCheck.at(orN*numNodes+deN).push_back(
                this->interRoutes.at(orN*numNodes+deN).at(pos).size());
            }
        }
    }
}

void ResourceAlloc::SetNumInterRoutesToCheck(std::vector<
std::vector<unsigned int>> numInterRoutesToCheck) {
    this->numInterRoutesToCheck = numInterRoutesToCheck;
}
