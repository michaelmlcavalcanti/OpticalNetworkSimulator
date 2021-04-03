/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GA_PDPPBO.cpp
 * Author: Henrique Dinarte
 *
 * Created on April 1, 2021, 2:14 PM
 */

#include <condition_variable>

#include "../../../include/Algorithms/GA/GA_PDPPBO.h"
#include "../../../include/SimulationType/SimulationType.h"
#include "../../../include/Structure/Topology.h"

GA_PDPPBO::GA_PDPPBO(SimulationType* simul) : GA_MO(simul), numNodes(0) {

}

GA_PDPPBO::~GA_PDPPBO() {

}

void GA_PDPPBO::Initialize() {
    GA_MO::Initialize();
    this->SetNumNodes(this->GetSimul()->GetTopology()->GetNumNodes());
    this->LoadPDPPBitRateAllDistOption();
}

void GA_PDPPBO::InitializePopulation() {

}

void GA_PDPPBO::CreateNewPopulation() {

}

void GA_PDPPBO::ApplyIndividual(Individual* ind) {

}

void GA_PDPPBO::SetIndParameters(Individual* ind) {

}

unsigned int GA_PDPPBO::GetNumNodes() const {
    return numNodes;
}

void GA_PDPPBO::SetNumNodes(unsigned int numNodes) {
    assert(numNodes > 0);
    
    this->numNodes = numNodes;
}

void GA_PDPPBO::LoadPDPPBitRateAllDistOption() {
    /*std::vector<double> VecTraffic;
    VecTraffic = resDevAlloc->traffic->GetVecTraffic();
    std::vector<double> auxBitRateOption;
    double partialBitRate;
    double beta = parameters->GetBeta();

    if(PDPPType == 0){
        for(auto it : VecTraffic){
            partialBitRate = ceil (((it)/(numSchProtRoutes -1)) -
            (((beta) * (it)) / (numSchProtRoutes -1)));
            for(unsigned int a = 0; a < numSchProtRoutes;a++){
                auxBitRateOption.push_back(partialBitRate);                        
            }
        PDPPBitRateDistOptions.push_back(auxBitRateOption);
        auxBitRateOption.clear();
        }
    }
    else if(PDPPType == 1){
        if(beta != 0){
            for(auto it : VecTraffic){
                double BRdown = ((it/2) - (beta*it));
                double BRup = ((it/2) + (beta*it));
                double BRmin = ((it*(1 - beta)));
                   
                for(double a = BRdown; a <= BRup; a = a+5e9){
                    for(double b = BRdown; b <= BRup; b = b+5e9){
                        for(double c = BRdown; c <= BRup; c = c+5e9){
                            if(a + b >= BRmin && b + c >= BRmin && a + c >= BRmin){
                                auxBitRateOption.push_back(a);
                                auxBitRateOption.push_back(b);
                                auxBitRateOption.push_back(c);
                            }
                        }
                    }
                }
                PDPPBitRateDistOptions.push_back(auxBitRateOption);
            }
        }
        else{
            for(auto it : VecTraffic){
                partialBitRate = ceil (((it)/(numSchProtRoutes -1)) -
                                       (((beta) * (it)) / (numSchProtRoutes -1)));
                for(unsigned int a = 0; a < numSchProtRoutes;a++){
                    auxBitRateOption.push_back(partialBitRate);
                }
                PDPPBitRateDistOptions.push_back(auxBitRateOption);
                auxBitRateOption.clear();
            }
        }
    }*/
    
}


