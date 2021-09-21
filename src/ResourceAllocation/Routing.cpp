/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Routing.cpp
 * Author: BrunoVinicius
 * 
 * Created on November 19, 2018, 11:43 PM
 */

#include "../../include/ResourceAllocation/Routing.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/ResourceAllocation/Resources.h"
#include "../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Structure/Link.h"
#include "../../include/GeneralClasses/Def.h"
#include "../../include/Calls/Call.h"
#include "../../include/Data/Data.h"
#include "../../include/Data/Parameters.h"

bool RouteCompare::operator()(const std::shared_ptr<Route>& routeA,
                              const std::shared_ptr<Route>& routeB) {
    
    return (routeA->GetCost() > routeB->GetCost());
}

Routing::Routing(ResourceAlloc* rsa, RoutingOption option, Data* data, 
Parameters* parameters)
:resourceAlloc(rsa), routingOption(option), topology(nullptr), 
data(data), parameters(parameters), resources(nullptr), K(0) {
    
}

Routing::~Routing() {
}

void Routing::Load() {
    topology = resourceAlloc->GetTopology();
    resources = resourceAlloc->GetResources();

    if(routingOption == RoutingYEN || routingOption == RoutingBSR_YEN)
        this->SetK(parameters->GetNumberRoutes());
}

void Routing::RoutingCall(Call* call) {
    
    switch(this->routingOption){
        case RoutingDJK:
        case RoutingYEN:
        case RoutingBSR:
        case RoutingBSR_YEN:
        case RoutingMP:
            this->SetOfflineRouting(call);
            break;
        default:
            std::cerr << "Invalid routing option" << std::endl;
            std::abort();
    }
}

void Routing::SetOfflineRouting(Call* call) {
    NodeIndex orNode = call->GetOrNode()->GetNodeId();
    NodeIndex deNode = call->GetDeNode()->GetNodeId();

    call->PushTrialRoutes(resources->GetRoutes(orNode, deNode));
    
    if(resourceAlloc->options->GetProtectionOption() != ProtectionDisable)
        call->PushTrialProtRoutes(resources->GetRoutes(orNode, deNode));
}

void Routing::Dijkstra() {
    std::shared_ptr<Route> route;
    unsigned int numNodes = this->topology->GetNumNodes();
    
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            if(orN != deN){
                route = this->Dijkstra(orN, deN);
            }
            else{
                route = nullptr;
            }
            resources->SetRoute(orN, deN, route);
            route.reset();
        }
    }
}

std::shared_ptr<Route> Routing::Dijkstra(NodeIndex orNode, NodeIndex deNode) {
    assert(orNode != deNode);
    
    int k = -1, h, hops;
    unsigned int i, j, setVertexes;
    long double min;
    unsigned int numNodes = this->topology->GetNumNodes();
    std::vector<int> path(0);
    std::vector<int> invPath(0);
    Link* auxLink;
    std::shared_ptr<Route> routeDJK = nullptr;
    bool networkDisconnected = false;
    
    std::vector<double> custoVertice(numNodes);
    std::vector<int> precedente(numNodes);
    std::vector<int> pathRev(numNodes);
    std::vector<bool> status(numNodes);
    
    //Initializes all vertices with infinite cost
    //and the source vertice with cost zero
    for(i = 0; i < numNodes; i++){
        if(i != orNode)
            custoVertice.at(i) = Def::Max_Double;
        else
            custoVertice.at(i) = 0.0;
        precedente.at(i) = -1;
        status.at(i) = 0;
    }
    setVertexes = numNodes;

    while(setVertexes > 0 && !networkDisconnected){

        min = Def::Max_Double;
        
        for(i = 0; i < numNodes; i++)
            if((status.at(i) == 0) && (custoVertice.at(i) < min)){
                min = custoVertice.at(i);
                k = i;
            }

        if(k == (int) deNode)
            break;
        
        status.at(k) = 1;
        setVertexes--;
        bool outputLinkFound = false;

        for(j = 0; j < numNodes; j++){
            auxLink = this->topology->GetLink((unsigned int) k, 
                                           (unsigned int) j);
            //(link->GetCost() < Def::Max_Double)
            if((auxLink != NULL) && (auxLink->IsLinkWorking()) && 
               (this->topology->GetNode(auxLink->GetOrigimNode())->
               IsNodeWorking()) &&
               (this->topology->GetNode(auxLink->GetDestinationNode())->
               IsNodeWorking())){
                outputLinkFound = true;
                
                if( (status.at(j) == 0) && (custoVertice.at(k) + 
                  auxLink->GetCost() < custoVertice.at(j)) ){
                   custoVertice.at(j) = custoVertice.at(k) + auxLink->GetCost();
                   precedente.at(j) = k;
                }
            }
        }
        
        if(!outputLinkFound)
            networkDisconnected = true;
    }
    
    if(!networkDisconnected){
        path.push_back(deNode);
        hops = 0;
        j = deNode;
        
        while(j != orNode){
            hops++;
            if(precedente.at(j) != -1){
                path.push_back(precedente.at(j));
                j = precedente.at(j);
            }
            else{
                networkDisconnected = true;
                break;
            }
        }
        if(!networkDisconnected){     
            
            for(h = 0; h <= hops; h++)
                invPath.push_back(path.at(hops-h));

            routeDJK = std::make_shared<Route>(this->GetResourceAlloc(), 
                                               invPath);
        }
    }
    
    return routeDJK;
}

void Routing::YEN() {
    std::vector<std::shared_ptr<Route>> routes;
    unsigned int numNodes = this->topology->GetNumNodes();
    
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            if(orN != deN){
                routes = this->YEN(orN, deN);
            }
            else{
                routes.resize(this->GetK(), nullptr);
            }
            resources->SetRoutes(orN, deN, routes);
            routes.clear();
        }
    }
}

std::vector<std::shared_ptr<Route> > Routing::YEN(NodeIndex orNode, 
                                                  NodeIndex deNode) {
    assert(orNode != deNode);
    std::vector<std::shared_ptr<Route>> routesYEN;
    std::priority_queue<std::shared_ptr<Route>, 
                        std::vector<std::shared_ptr<Route>>,
                        RouteCompare> candidateRoutes;
    Node* spurNode;
    std::shared_ptr<Route> spurPath;
    std::shared_ptr<Route> rootPath;
    std::shared_ptr<Route> totalPath;
    
    // Determine the shortest path from the source to the destination.
    std::shared_ptr<Route> newRoute = this->Dijkstra(orNode, deNode);
    routesYEN.push_back(newRoute);
    
    for(unsigned int k = 1; k < this->K; k++){
        unsigned int auxSize = routesYEN.at(k-1)->GetNumNodes() - 2;
        
        //The spurNode ranges from the first node to the next to last node 
        //in the previous k-shortest path.
        for(unsigned int i = 0; i <= auxSize; i++){
            //spurNode is retrieved from the previous k-shortest path, k − 1.
            spurNode = routesYEN.at(k-1)->GetNode(i);
            // The sequence of nodes from the source to the spurNode of the 
            //previous k-shortest path.
            rootPath = routesYEN.at(k-1)->CreatePartialRoute(0, i);
            
            for(auto it: routesYEN){
                //Remove the links that are part of the previous shortest 
                //paths which share the same rootPath.
                if(i < it->GetNumNodes()){
                    newRoute = it->CreatePartialRoute(0, i);
                    if(rootPath->GetPath() == newRoute->GetPath()){
                        it->GetLink(i)->SetLinkState(false);
                    }
                }
            }
            
            for(unsigned int  a = 0; a < rootPath->GetNumNodes(); a++){
                if(rootPath->GetNode(a)->GetNodeId() == spurNode->GetNodeId())
                    continue;
                rootPath->GetNode(a)->SetNodeState(false);
            }
            
            // Calculate the spurPath from the spurNode to the destination.
            spurPath = this->Dijkstra(spurNode->GetNodeId(), deNode);
            
            if(spurPath != nullptr){
                // Entire path is made up of the rootPath and spurPath.
                totalPath = rootPath->AddRoute(spurPath);
                // Add the potential k-shortest path to the queue.
                candidateRoutes.push(totalPath);
            }
            
            // Add back the edges and nodes that were removed from the graph.
            this->topology->SetAllLinksWorking();
            rootPath->SetAllNodesWorking();
        }
        if(candidateRoutes.empty())
            break;    
        
        //Get the first route and store it in vector candidateRoutes
        routesYEN.push_back(candidateRoutes.top());
        candidateRoutes.pop();
    }
    
    while(routesYEN.size() < this->K){
        routesYEN.push_back(nullptr);
    }
    
    return routesYEN;
}

void Routing::BSR() {
    unsigned int numIt = 100;
    const double alpha = 0.9999;
    double bestBP = Def::Max_Double;
    Resources* resources = this->resourceAlloc->GetResources();
    std::vector<std::vector<std::shared_ptr<Route>>> bestRoutes(0);
    data->SetActualIndex(0);
    resourceAlloc->GetSimulType()->GetCallGenerator()->SetNetworkLoad(
    parameters->GetMaxLoadPoint());
    double numMaxReq = parameters->GetNumberReqMax();
    parameters->SetNumberReqMax(1E5);
    
    for(unsigned int it = 1; it <= numIt; it++){
        
        //Update the links utilization and costs
        if(it != 1)
            this->UpdateLinksUtiCosts(alpha);
        
        this->Dijkstra();
        resources->CreateOfflineModulation();
        
        //Run a simulation with the routes of this iteration
        resourceAlloc->GetSimulType()->RunBase();
        
        //Calc the BP and keep the routes if it is better then the 
        //previous best BP.
        if(data->GetReqBP() < bestBP){
            bestBP = data->GetReqBP();
            bestRoutes = resources->GetRoutes();
        }
        data->Initialize();
    }
    
    //Set the best set of routes
    resources->SetRoutes(bestRoutes);
    parameters->SetNumberReqMax(numMaxReq);
}

void Routing::UpdateLinksUtiCosts(const double alpha) {
    unsigned int numNodes = topology->GetNumNodes();
    std::vector<std::shared_ptr<Route>> auxRoute;
    Link* auxLink;
    double auxCost;
    
    //Clear all links utilization
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            
            if(orN == deN)
                continue;
            auxLink = topology->GetLink(orN, deN);
            
            if(auxLink != nullptr)
                auxLink->SetUtilization(0);
        }
    }
    
    //Update links utilization
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            
            if(orN == deN)
                continue;
            auxRoute = resources->GetRoutes(orN, deN);
            
            for(unsigned int a = 0; a < auxRoute.front()->GetNumHops(); a++){
                auxLink = auxRoute.front()->GetLink(a);
                auxLink->SetUtilization(auxLink->GetUtilization() + 1);
            }
        }
    }
    
    //Update links costs
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            
            if(orN == deN)
                continue;
            auxLink = topology->GetLink(orN, deN);
            
            if(auxLink != nullptr){
                auxCost = (alpha * (auxLink->GetCost())) +
                          ((1-alpha) * ((double) auxLink->GetUtilization()));
                auxLink->SetCost(auxCost);
            }
        }
    }
}

void Routing::BSR_YEN() {
unsigned int numIt = 100;
    const double alpha = 0.9999;
    double bestBP = Def::Max_Double;
    Resources* resources = this->resourceAlloc->GetResources();
    std::vector<std::vector<std::shared_ptr<Route>>> bestRoutes(0);
    data->SetActualIndex(0);
    resourceAlloc->GetSimulType()->GetCallGenerator()->SetNetworkLoad(
    parameters->GetMaxLoadPoint());
    double numMaxReq = parameters->GetNumberReqMax();
    parameters->SetNumberReqMax(1E5);
    
    for(unsigned int it = 1; it <= numIt; it++){
        
        //Update the links utilization and costs
        if(it != 1)
            this->UpdateLinksUtiCostsBSR_YEN(alpha);
        
        this->YEN();
        resources->CreateOfflineModulation();
        
        //Run a simulation with the routes of this iteration
        resourceAlloc->GetSimulType()->RunBase();
        
        //Calc the BP and keep the routes if it is better then the 
        //previous best BP.
        if(data->GetReqBP() < bestBP){
            bestBP = data->GetReqBP();
            bestRoutes = resources->GetRoutes();
        }
        data->Initialize();
    }
    
    //Set the best set of routes
    resources->SetRoutes(bestRoutes);
    parameters->SetNumberReqMax(numMaxReq);
}

void Routing::UpdateLinksUtiCostsBSR_YEN(const double alpha) {
unsigned int numNodes = topology->GetNumNodes();
    std::vector<std::shared_ptr<Route>> auxRoute;
    Link* auxLink;
    double auxCost;
    
    //Clear all links utilization
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            
            if(orN == deN)
                continue;
            auxLink = topology->GetLink(orN, deN);
            
            if(auxLink != nullptr)
                auxLink->SetUtilization(0);
        }
    }
    
    //Update links utilization
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            
            if(orN == deN)
                continue;
            auxRoute = resources->GetRoutes(orN, deN);
            
            for(unsigned int k = 0; k < auxRoute.size(); k++) {
                for(unsigned int a = 0; a < auxRoute.at(k)->GetNumHops(); a++){
                    auxLink = auxRoute.at(k)->GetLink(a);
                    auxLink->SetUtilization(auxLink->GetUtilization() + 1);
                }
            }
        }
    }
    
    //Update links costs
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            
            if(orN == deN)
                continue;
            auxLink = topology->GetLink(orN, deN);
            
            if(auxLink != nullptr){
                auxCost = (alpha * (auxLink->GetCost())) +
                          ((1-alpha) * ((double) auxLink->GetUtilization()));
                auxLink->SetCost(auxCost);
            }
        }
    }
}

void Routing::ProtectionDisjointYEN() {
    
    std::vector<std::shared_ptr<Route>> routes;
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int numRoutes;
    unsigned int nodePairIndex;
    resources->protectionAllRoutes.resize(resources->allRoutes.size());
   // Kd = parameters->GetNumberProtectionRoutes();
    
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            nodePairIndex = orN * numNodes + deN;
            numRoutes = resources->allRoutes.at(nodePairIndex).size();
            resources->protectionAllRoutes.at(nodePairIndex).resize(numRoutes);
            
            for (unsigned int routeIndex = 0;  routeIndex < numRoutes; 
            routeIndex++){
                if(orN != deN){
                    routes = this->ProtectionDisjointYEN(orN, deN, routeIndex);                
                }
                else {
                    routes.assign(1, nullptr);
                }
                resources->protectionAllRoutes.at(nodePairIndex).at(routeIndex) =  routes;
                routes.clear();                
            }
        }
    }
}

std::vector<std::shared_ptr<Route> > Routing::ProtectionDisjointYEN(NodeIndex 
orNode, NodeIndex deNode, RouteIndex routeIndex) {
    unsigned int numNodes = topology->GetNumNodes();
    unsigned int nodePairIndex = orNode * numNodes + deNode;
  // Kd = parameters->GetNumberProtectionRoutes();
    std::shared_ptr<Route> orRoute = resources->allRoutes.at(nodePairIndex).at(routeIndex);
    std::shared_ptr<Route> auxRoute;
    std::vector<std::shared_ptr<Route> > routes(0);
    resourceAlloc->DisableRouteLinks(orRoute.get());    
    auxRoute = this->Dijkstra(orNode, deNode);
    if(auxRoute != nullptr)
        routes.push_back(auxRoute);
        
    while (auxRoute != nullptr) {
        resourceAlloc->DisableRouteLinks(auxRoute.get());
        auxRoute = this->Dijkstra(orNode, deNode);
        if(auxRoute != nullptr)
            routes.push_back(auxRoute);
    }
/*for (unsigned int a = 0; a < Kd; a++){
        auxRoute = this->Dijkstra(orNode, deNode);
        routes.push_back(auxRoute);
        
        if(auxRoute != nullptr){   
            resourceAlloc->DisableRouteLinks(auxRoute.get());
        }             
    }*/
    topology->SetAllLinksWorking();
    
    return routes;
}

void Routing::ProtectionYEN() {
    std::vector<std::shared_ptr<Route>> routes;
    unsigned int numNodes = this->topology->GetNumNodes();
    unsigned int numRoutes;
    unsigned int nodePairIndex;
    resources->protectionAllRoutes.resize(resources->allRoutes.size());
    
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            nodePairIndex = orN * numNodes + deN;
            numRoutes = resources->allRoutes.at(nodePairIndex).size();
            resources->protectionAllRoutes.at(nodePairIndex).resize(numRoutes);
            
            for (unsigned int routeIndex = 0;  routeIndex < numRoutes; 
            routeIndex++){
                if(orN != deN){
                    routes = this->ProtectionYEN(orN, deN, routeIndex);                
                }
                else {
                    routes.assign(1, nullptr);
                }
                resources->protectionAllRoutes.at(nodePairIndex).at(routeIndex) = 
                routes;
                routes.clear();                
            }
        }
    }
}

std::vector<std::shared_ptr<Route> > Routing::ProtectionYEN(NodeIndex orNode, 
NodeIndex deNode, RouteIndex routeIndex) {
 unsigned int numNodes = topology->GetNumNodes();
    unsigned int nodePairIndex = orNode * numNodes + deNode;
    std::shared_ptr<Route> orRoute = resources->allRoutes.at(nodePairIndex)
    .at(routeIndex);
    std::shared_ptr<Route> auxRoute;
    std::vector<std::shared_ptr<Route> > routes(0);
    resourceAlloc->DisableRouteLinks(orRoute.get());    
    auxRoute = this->Dijkstra(orNode, deNode);
    routes.push_back(auxRoute);
    std::shared_ptr<Route> lastAuxRoute;
        
    while (auxRoute != nullptr) {
        resourceAlloc->DisableRouteLinks(auxRoute.get());
        auxRoute = this->Dijkstra(orNode, deNode);
        routes.push_back(auxRoute);
        lastAuxRoute = auxRoute;
        
        if (auxRoute == nullptr) {
            resourceAlloc->EnableRouteLinks(lastAuxRoute.get());
            this->K = 1;
            auxRoute = (this->YEN(orNode, deNode)).back();
            routes.push_back(auxRoute);
        }
    }    
    
    topology->SetAllLinksWorking();
    
    return routes;
    
}

void Routing::ProtectionMIR() {
    std::vector<std::shared_ptr<Route>> routesAll;
    unsigned int numNodes = this->topology->GetNumNodes();
    std::vector<std::shared_ptr<Route>> routesPairMIR;

    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            if(orN != deN){
                routesAll = this->ProtectionMIR(orN, deN);
            }
            else{
                routesAll.resize(this->GetK(), nullptr);
            }
            this->CreateMinInterfRouteGroups(routesAll);

            resources->SetRoutes(orN, deN, routesAll);
            routesAll.clear();
        }
    }
}

std::vector<std::shared_ptr<Route>> Routing::ProtectionMIR(NodeIndex orNode,
NodeIndex deNode) {
    assert(orNode != deNode);
    std::vector<std::shared_ptr<Route>> routes;
    std::priority_queue<std::shared_ptr<Route>,
            std::vector<std::shared_ptr<Route>>,
            RouteCompare> candidateRoutes;
    Node* spurNode;
    std::shared_ptr<Route> spurPath;
    std::shared_ptr<Route> rootPath;
    std::shared_ptr<Route> totalPath;

    // Determine the shortest path from the source to the destination.
    std::shared_ptr<Route> newRoute = this->Dijkstra(orNode, deNode);
    routes.push_back(newRoute);

    //declaring auxiliar variables to check duplicated/ Null values in routes container
    unsigned int counter = 2;
    auto i1 = std::adjacent_find(routes.begin(), routes.end());
    bool route;

    for(unsigned int k = 1; k < counter; k++){
        unsigned int auxSize = routes.at(k - 1)->GetNumNodes() - 2;

        //The spurNode ranges from the first node to the next to last node
        //in the previous k-shortest path.
        for(unsigned int i = 0; i <= auxSize; i++){
            //spurNode is retrieved from the previous k-shortest path, k − 1.
            spurNode = routes.at(k - 1)->GetNode(i);
            // The sequence of nodes from the source to the spurNode of the
            //previous k-shortest path.
            rootPath = routes.at(k - 1)->CreatePartialRoute(0, i);

            for(auto it: routes){
                //Remove the links that are part of the previous shortest
                //paths which share the same rootPath.
                if(i < it->GetNumNodes()){
                    newRoute = it->CreatePartialRoute(0, i);
                    if(rootPath->GetPath() == newRoute->GetPath()){
                        it->GetLink(i)->SetLinkState(false);
                    }
                }
            }

            for(unsigned int  a = 0; a < rootPath->GetNumNodes(); a++){
                if(rootPath->GetNode(a)->GetNodeId() == spurNode->GetNodeId())
                    continue;
                rootPath->GetNode(a)->SetNodeState(false);
            }

            // Calculate the spurPath from the spurNode to the destination.
            spurPath = this->Dijkstra(spurNode->GetNodeId(), deNode);

            if(spurPath != nullptr){
                // Entire path is made up of the rootPath and spurPath.
                totalPath = rootPath->AddRoute(spurPath);
                // Add the potential k-shortest path to the queue.
                candidateRoutes.push(totalPath);
            }

            // Add back the edges and nodes that were removed from the graph.
            this->topology->SetAllLinksWorking();
            rootPath->SetAllNodesWorking();
        }
        if(candidateRoutes.empty())
            break;

        //Get the first route and store it in vector candidateRoutes
        routes.push_back(candidateRoutes.top());
        candidateRoutes.pop();

        //checking for duplicated values in container routes
        i1 = std::adjacent_find(routes.begin(), routes.end());
        for(auto it : routes){
            if(it == nullptr)
                route = false;
        }
        if (i1 == routes.end() && route == true)  //routes have no duplicated/Null values
            counter++;
    }

    while(routes.size() < counter){
        routes.push_back(nullptr);
    }

    return routes;
}

std::vector<std::shared_ptr<Route>> Routing::CreateMinInterfRouteGroups(
std::vector<std::shared_ptr<Route>> routes){
    std::vector<std::shared_ptr<Route>> routesAll = routes;
    std::vector<std::shared_ptr<Route>> routesMIR;
    std::vector<Link*> linksR1;
    std::vector<Link*> linksR2;
    std::vector<Link*> linksR3;
    unsigned int countLinkInter = 0;
    unsigned int countRouteGroup = 0;
    unsigned int minNumLinkInter;
    unsigned int maxNumLinkInter;
    std::vector<unsigned int> numLinkInter;
    std::vector<unsigned int> valuesLinkInter;
    std::vector<std::vector<std::shared_ptr<Route>>> routesGroupInter;

    if(parameters->GetNumberPDPPprotectionRoutes() == 3) {
        for (auto r1 : routesAll) {  //begin loop of routes from actual s-d pair
            for (auto r2 : routesAll) {
                for (auto r3 : routesAll) {
                    if (r1 != r2 && r1 != r3 &&
                        r2 != r3) { //find a group of routes to check
                        countRouteGroup++;
                        linksR1 = resourceAlloc->route->GetLinks(r1);
                        linksR2 = resourceAlloc->route->GetLinks(r2);
                        linksR3 = resourceAlloc->route->GetLinks(r3);
                        for (auto l1 : routesAll) { //begin loop of links from actual group of routes
                            for (auto l2 : routesAll) {
                                for (auto l3 :routesAll) {
                                    if (l1 == l2 && l1 == r3 && l2 == l3) {
                                        countLinkInter++;
                                    }
                                }
                            }
                        }
                        numLinkInter.push_back(countLinkInter);
                        routesGroupInter.at(countRouteGroup).push_back(r1);
                        routesGroupInter.at(countRouteGroup).push_back(r2);
                        routesGroupInter.at(countRouteGroup).push_back(r3);
                        countLinkInter = 0;
                    }
                }
            }
        }
        //removing duplicate values from vector valuesLinkInter
        for (unsigned int it = 0; it < numLinkInter.size(); it++) {
            if (numLinkInter.at(it) != valuesLinkInter.at(it))
                valuesLinkInter.push_back(numLinkInter.at(it));
        }
        //sorting vector valuesLinkInter in a crescent order
        sort(valuesLinkInter.begin(), valuesLinkInter.end());
        //for (const auto &i: valuesLinkInter) {
        //    sort(valuesLinkInter.begin(), valuesLinkInter.end());
        //}
        //inserting groups of minimal interference routes in vector routesMIR
        for (auto v : valuesLinkInter) {
            for (auto n : numLinkInter) {
                if (n == v) {
                    for (auto it : routesGroupInter.at(n)) {
                        routesMIR.push_back(it);
                    }
                }
            }
        }
    }

    if(parameters->GetNumberPDPPprotectionRoutes() == 2) {
        for (auto r1 : routesAll) {
            for (auto r2 : routesAll) {
                if(r1 != r2){
                    countRouteGroup++;
                    linksR1 = resourceAlloc->route->GetLinks(r1);
                    linksR2 = resourceAlloc->route->GetLinks(r2);
                    for (auto l1 : routesAll) { //begin loop of links from actual group of routes
                        for (auto l2 : routesAll) {
                            if (l1 == l2 ) {
                                countLinkInter++;
                            }
                        }
                    }
                    numLinkInter.push_back(countLinkInter);
                    routesGroupInter.at(countRouteGroup).push_back(r1);
                    routesGroupInter.at(countRouteGroup).push_back(r2);
                    countLinkInter = 0;
                }
            }
        }
        //removing duplicate values from vector valuesLinkInter
        for(unsigned int it = 0; it < numLinkInter.size(); it++){
            if(numLinkInter.at(it) != valuesLinkInter.at(it))
                valuesLinkInter.push_back(numLinkInter.at(it));
        }
        //sorting vector valuesLinkInter in a crescent order
        for (const auto &i: valuesLinkInter){
            sort(valuesLinkInter.begin(), valuesLinkInter.end());
        }
        //inserting groups of minimal interference routes in vector routesMIR
        for(auto v : valuesLinkInter) {
            for (auto n : numLinkInter) {
                if (n == v) {
                    for (auto it : routesGroupInter.at(n)) {
                        routesMIR.push_back(it);
                    }
                }
            }
        }
    }

    return routesMIR;
}

void Routing::MultiPathRouting() {
    this->AllRoutes();
    std::vector<Link*> linksRi;
    std::vector<Link*> linksRj;
    bool interference = false;
    std::vector<std::vector<std::shared_ptr<Route>>> auxDisjointRoutes;
    auxDisjointRoutes.resize(resources->allRoutes.size());

    for(int pairIndex = 0; pairIndex < resources->allRoutes.size(); pairIndex++) {
        for(auto ri : resources->allRoutes.at(pairIndex)) {
            linksRi = ri->GetLinks(ri);
            for(auto rj : resources->allRoutes.at(pairIndex)) {
                linksRj = rj->GetLinks(rj);
                for(auto li : linksRi){
                    for(auto lj : linksRj){
                        if(li == lj){
                            linksRi.clear();
                            linksRj.clear();
                            interference = true;
                            break;
                        }
                    auxDisjointRoutes.at(pairIndex).push_back(ri);
                    auxDisjointRoutes.at(pairIndex).push_back(rj);
                    }
                }if(interference)
                    break;
            }
        }
    }


}

void Routing::AllRoutes() {
    std::shared_ptr<Route> route;
    std::vector<std::shared_ptr<Route>> vRoutes;
    unsigned int numNodes = this->topology->GetNumNodes();

    for(int orN = 0; orN < numNodes; orN++){
        for(int deN = 0; deN < numNodes; deN++){
            if(orN != deN) {   // Check if orN and deN are different
                std::vector<int> empty;
                //route = std::make_shared<Route>(empty);
                vRoutes.clear();
                AllRoutes(orN, deN, route, vRoutes);
                resources->SetRoutes(orN, deN, vRoutes);
            }
        }
    }
}

void Routing::AllRoutes(NodeIndex curNode, NodeIndex deNode, std::shared_ptr<Route> route,
                        std::vector<std::shared_ptr<Route>> vRoutes) {
    Link* link;
    std::shared_ptr<Route> newRoute;

    if(!route->IsNode(curNode)){   //curNode does not cause loop
        route->AddNodeAtEnd(curNode);
        if(deNode == route->GetDeNodeId()){   //check if destine was reached;
            vRoutes.push_back(route);
        }
        else{
            for(int nextNode = 0; nextNode < this->topology->GetNumNodes(); nextNode++){
                link = this->topology->GetLink(curNode, nextNode);
                if(link != NULL && link->IsLinkWorking()){ //There is a connection between orN and nextNode
                    newRoute = route;
                    AllRoutes(nextNode, deNode, newRoute, vRoutes);
                }
            }
            route.reset();
        }
    }
    else //curNode would cause loop
        route.reset();
}


ResourceAlloc* Routing::GetResourceAlloc() const {
    return resourceAlloc;
}

void Routing::SetResourceAlloc(ResourceAlloc* rsaAlgorithm) {
    this->resourceAlloc = rsaAlgorithm;
}

RoutingOption Routing::GetRoutingOption() const {
    return routingOption;
}

void Routing::SetRoutingOption(RoutingOption routingOption) {
    this->routingOption = routingOption;
}

Topology* Routing::GetTopology() const {
    return topology;
}

void Routing::SetTopology(Topology* topology) {
    this->topology = topology;
}

unsigned int Routing::GetK() const {
    return K;
}

void Routing::SetK(unsigned int K) {
    this->K = K;
}








