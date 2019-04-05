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
#include "../../include/Calls/Call.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/Structure/Topology.h"
#include "../../include/GeneralClasses/Def.h"
#include "../../include/ResourceAllocation/ResourceAlloc.h"

SA::SA(ResourceAlloc* rsa, SpectrumAllocationOption option, Topology* topology) 
:resourceAlloc(rsa), specAllOption(option), topology(topology) {
    
}

SA::~SA() {
    
}

void SA::SpecAllocation(Call* call) {
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
    }
}

std::vector<unsigned int> SA::SpecAllocation(unsigned int lastSlot) {
    std::vector<unsigned int> vec(0);
    
    switch(this->specAllOption){
        case SpecAllRandom:
            return this->RandomSlots(lastSlot);
        case SpecAllFF:
        case SpecAllMSCL:
            return this->FirstFitSlots(lastSlot);
        default:
            std::cerr << "Invalid spectrum allocation option" << std::endl;
            return vec;
    }
}

void SA::Random(Call* call) {
    std::vector<unsigned int> vecSlots(0);
    
    vecSlots = this->FirstFitSlots(call);
    std::shuffle(vecSlots.begin(), vecSlots.end(), Def::pseudoRandomDevice);
    
    call->SetFirstSlot(vecSlots.back());
    call->SetLastSlot(vecSlots.back() + call->GetNumberSlots() - 1);
}

void SA::FirstFit(Call* call) {
    Route* route = call->GetRoute();
    unsigned int numSlotsReq = call->GetNumberSlots();
    unsigned int maxSlotIndex = this->topology->GetNumSlots() - 
                                 numSlotsReq;
    
    for(unsigned int slot = 0; slot <= maxSlotIndex; slot++){
        if(this->topology->CheckSlotsDisp(route, slot, slot + numSlotsReq - 1)){
            call->SetFirstSlot(slot);
            call->SetLastSlot(slot + numSlotsReq - 1);
        }
    }

}

void SA::MSCL(Call* call) {
    std::cout << "Function not implemented" << std::endl;
}

Topology* SA::GetTopology() {
    return topology;
}

int SA::CalcNumFormAloc(int L, bool* Disp,int tam) { 
//L indica a largura da requisicao e Disp o vetor de disponibilidade
/*
	int sum = 0, si, se; //si eh o slot inicial da alocacao, que vai de 0 ate SE-L
	for(si = 0; si <= tam-L; si++){
		for(se = si; se < si+L; se++) //se checa se todos os slots de si ate si+L-1 estao disponiveis
			if(Disp[se] == false)
				break;
		if(se == si+L) // Os slots si,si+1,...,si+Lf-1 estao disponiveis
			sum++;
	}
	return sum;
}
*/
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

ResourceAlloc* SA::GetResourceAlloc(){
    return this->resourceAlloc;
}

std::vector<unsigned int> SA::RandomSlots(Call* call) {
    std::vector<unsigned int> vecSlots(0);
    vecSlots = this->FirstFitSlots(call);
    
    std::shuffle(vecSlots.begin(), vecSlots.end(), Def::pseudoRandomDevice);
    
    return vecSlots;
}

std::vector<unsigned int> SA::RandomSlots(unsigned int lastSlot) {
    std::vector<unsigned int> vecSlots = this->FirstFitSlots(lastSlot);
    
    std::shuffle(vecSlots.begin(), vecSlots.end(), Def::pseudoRandomDevice);
    
    return vecSlots;
}

std::vector<unsigned int> SA::FirstFitSlots(Call* call) {
    Route* route = call->GetRoute();
    unsigned int numSlotsReq = call->GetNumberSlots();
    unsigned int maxSlotIndex = this->topology->GetNumSlots() - numSlotsReq;
    std::vector<unsigned int> slots(0);
    
    for(unsigned int a = 0; a <= maxSlotIndex; a++){
        if(this->topology->CheckSlotsDisp(route, a, a + numSlotsReq - 1)){
            slots.push_back(a);
        }
    }
    
    return slots;
}

std::vector<unsigned int> SA::FirstFitSlots(unsigned int lastSlot) {
    //unsigned int numSlots = this->resourceAlloc->GetTopology()->GetNumSlots();
    std::vector<unsigned int> vecSlots(0);
    
    for(unsigned int a = 0; a <= lastSlot; a++)
        vecSlots.push_back(a);
    
    return vecSlots;
}
