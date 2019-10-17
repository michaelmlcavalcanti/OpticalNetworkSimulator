/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SA.cpp
 * Author: BrunoVinicius
 * 
 * Created on November 22, 2018, 11:36 PM
 */

#include "../../include/ResourceAllocation/SA.h"
#include "../../include/ResourceAllocation/CSA.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/ResourceAllocation/ResourceAlloc.h"
#include "../../include/Calls/Call.h"
#include "../../include/Calls/CallDevices.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Structure/Node.h"
#include "../../include/GeneralClasses/Def.h"

SA::SA(ResourceAlloc* rsa, SpectrumAllocationOption option, Topology* topology) 
:resourceAlloc(rsa), specAllOption(option), topology(topology) {
    
}

SA::~SA() {
    
}

void SA::SpecAllocation(Call* call) {
    call->SetCore(0);
    
    switch(this->specAllOption){
        case SpecAllRandom:
            this->Random(call);
            break;
        case SpecAllFF:
            this->FirstFit(call);
            break;
        case SpecAllMSCL:
            this->MSCL(call);
            break;
        default:
            std::cerr << "Invalid spectrum allocation option" << std::endl;
            std::abort();
    }
}

void SA::SpecAllocation(CallDevices* call) {
    std::vector<Call*> calls = call->GetTranspSegments();
    
    if(!calls.empty()){
        for(auto it: calls)
            this->SpecAllocation(it);
    }
    else{
        Call* baseCall = dynamic_cast<Call*>(call);
        this->SpecAllocation(baseCall);
    }
}

std::vector<unsigned int> SA::SpecAllocation() {
    
    switch(this->specAllOption){
        case SpecAllRandom:
            return this->RandomSlots();
        case SpecAllFF:
            return this->FirstFitSlots();
        default:
            std::cerr << "Invalid spectrum allocation option" << std::endl;
            std::abort();
    }
}

void SA::Random(Call* call) {
    std::vector<unsigned int> vecSlots(0);
    
    vecSlots = this->FirstFitSlots(call);
    std::shuffle(vecSlots.begin(), vecSlots.end(), Def::pseudoRandomEngine);
    
    call->SetFirstSlot(vecSlots.back());
    call->SetLastSlot(vecSlots.back() + call->GetNumberSlots() - 1);
}

void SA::FirstFit(Call* call) {
    Route* route = call->GetRoute();
    unsigned int numSlotsReq = call->GetNumberSlots();
    unsigned int topNumSlots = this->topology->GetNumSlots();
    unsigned int numContigSlots = 0;
    
    for(unsigned int slot = 0; slot < topNumSlots; slot++){
        
        if(resourceAlloc->CheckSlotDisp(route, slot)){
            numContigSlots++;
            
            if(numContigSlots == numSlotsReq){
                call->SetFirstSlot(slot - numSlotsReq + 1);
                call->SetLastSlot(slot);
                break;
            }
        }
        else
            numContigSlots = 0;
    }
}

void SA::MSCL(Call* call) {
    unsigned int numSlotsReq = call->GetNumberSlots();
    unsigned int numSlotsTop = this->topology->GetNumSlots();
    Route* route = call->GetRoute();
    Route* auxRoute;
    unsigned int orNode = route->GetOrNode()->GetNodeId();
    unsigned int deNode = route->GetDeNode()->GetNodeId();
    
    std::vector<std::shared_ptr<Route>> intRoutes =
    this->resourceAlloc->GetInterRoutes(orNode, deNode, route);
    unsigned int numInterRoutesCheck =
    this->resourceAlloc->GetNumInterRoutesToCheck(orNode, deNode, route);
    std::vector<unsigned> vecTrafficSlots = 
    this->resourceAlloc->GetNumSlotsTraffic();
    
    unsigned int vetCapInic[numInterRoutesCheck+1];
    unsigned int vetCapFin[numInterRoutesCheck+1];
    double perda, perdaMin = Def::Max_Double;
    unsigned int si = Def::Max_UnInt;
    bool DispFitSi = false;    
    
    std::vector<SlotState> auxVecDisp;
    std::vector<std::vector<SlotState>> vecDisp;
    vecDisp.resize(numInterRoutesCheck+1);
    for(unsigned int a = 0; a <= numInterRoutesCheck; a++){
        if(a != 0)
            auxRoute = intRoutes.at(a-1).get();
        else
            auxRoute = route;
        
        vecDisp.at(a) = resourceAlloc->GetDispVector(auxRoute);
    }
    
    for(unsigned int s = 0; s < (numSlotsTop - numSlotsReq + 1); s++){
        DispFitSi = resourceAlloc->CheckSlotsDisp(route, s, 
                                                  s + numSlotsReq - 1);
        
        if(DispFitSi){
            perda = 0.0;
            
            for(unsigned int r = 0; r <= numInterRoutesCheck; r++){
                
                auxVecDisp = vecDisp.at(r);
                
                vetCapInic[r] = 0;
                vetCapFin[r] = 0;
                
                //Calculates the initial capacity based on the number of 
                //allocation forms.
                for(unsigned i = 0; i < vecTrafficSlots.size(); i++){
                    vetCapInic[r] += resourceAlloc->
                    CalcNumFormAloc(vecTrafficSlots.at(i), auxVecDisp);
                }
                //Calculates the requisition allocation impact in the 
                //interfering routes for each set of slots
                for(unsigned int i = s; i < s + numSlotsReq; i++)
                    auxVecDisp.at(i) = occupied;
                
                //Calculates the final capacity based on the number of 
                //allocation forms.
                for(unsigned i = 0; i < vecTrafficSlots.size(); i++){
                    vetCapFin[r] += resourceAlloc->
                    CalcNumFormAloc(vecTrafficSlots.at(i), auxVecDisp);
                }
                
                perda += vetCapInic[r] - vetCapFin[r];
                
                if(perda >= perdaMin)
                    break;
            }
            
            if(perda < perdaMin){
                perdaMin = perda;
                si = s;
            }
        }
    }
    
    if(perdaMin < Def::Max_Double){
        call->SetFirstSlot(si);
        call->SetLastSlot(si + numSlotsReq - 1);
    }
}

Topology* SA::GetTopology() {
    return topology;
}

ResourceAlloc* SA::GetResourceAlloc(){
    return this->resourceAlloc;
}

int SA::CalcNumFormAloc(int L, bool* Disp,int tam) { 
    int sum = 0, si;//si eh o slot inicial da alocacao, que vai de 0 ate SE-L
    int cont = 0;
    for(si = 0; si < tam; si++){
        if(Disp[si] == false){
            if(cont >= L){
                 sum += cont - L + 1;
                 //sum += floor (cont/L);
            }
                cont = 0;
        }
            else{
                cont++;
            }
    }
    if(cont >= L){
       sum += cont - L + 1;
    }
    return sum;
}

std::vector<unsigned int> SA::RandomSlots() {
    std::vector<unsigned int> vecSlots = this->FirstFitSlots();
    
    std::shuffle(vecSlots.begin(), vecSlots.end(), Def::pseudoRandomEngine);
    
    return vecSlots;
}

std::vector<unsigned int> SA::FirstFitSlots() {
    std::vector<unsigned int> vecSlots(0);
    unsigned int numSlots = topology->GetNumSlots();
    
    for(unsigned int a = 0; a < numSlots; a++)
        vecSlots.push_back(a);
    
    return vecSlots;
}

std::vector<unsigned int> SA::FirstFitSlots(Call* call) {
    Route* route = call->GetRoute();
    unsigned int numSlotsReq = call->GetNumberSlots();
    unsigned int maxSlotIndex = this->topology->GetNumSlots() - numSlotsReq;
    std::vector<unsigned int> slots(0);
    
    for(unsigned int a = 0; a <= maxSlotIndex; a++){
        if(resourceAlloc->CheckSlotsDisp(route, a, a + numSlotsReq - 1)){
            slots.push_back(a);
        }
    }
    
    return slots;
}