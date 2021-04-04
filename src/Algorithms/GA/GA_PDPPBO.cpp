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
#include "../../../include/Calls/Traffic.h"
#include "../../../include/ResourceAllocation/ResourceDeviceAlloc.h"
#include "../../../include/ResourceAllocation/ProtectionSchemes/PartitioningDedicatedPathProtection.h"

GA_PDPPBO::GA_PDPPBO(SimulationType* simul) : GA_MO(simul), numNodes(0), numTraffic(0) {
    ResourceDeviceAlloc* resource_alloc;
    resource_alloc = dynamic_cast<ResourceDeviceAlloc*>(simul->GetResourceAlloc());
    pdppbo = dynamic_cast<PartitioningDedicatedPathProtection*>(resource_alloc->GetProtectionScheme());
    // Include asserts (Simulation is protection is active, if the protection is the right one, etc.)
}

GA_PDPPBO::~GA_PDPPBO() {

}

void GA_PDPPBO::Initialize() {
    GA_MO::Initialize();
    this->SetNumNodes(this->GetSimul()->GetTopology()->GetNumNodes());
    //Mudar pra função interna
    numTraffic = simul->GetTraffic()->GetVecTraffic().size();
    this->LoadPDPPBitRateAllDistOption();
    // Chamar função que inicializa as indexDistOption
}

void GA_PDPPBO::InitializePopulation() {
    assert(this->actualParetoFronts.empty() && this->totalPopulation.empty());
    std::vector<std::shared_ptr<Individual>> auxVecInd(0);

    for(unsigned int a = 0; a < this->GetNumberIndividuals(); a++)
        auxVecInd.push_back(std::make_shared<IndividualPDPPBO>(this));

    this->actualParetoFronts.push_back(auxVecInd);
}

void GA_PDPPBO::CreateNewPopulation() {

}

void GA_PDPPBO::ApplyIndividual(Individual* ind) {
    IndividualPDPPBO* auxInd = dynamic_cast<IndividualPDPPBO*>(ind);

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

unsigned int GA_PDPPBO::GetNumTraffic() const {
    return numTraffic;
}

std::vector<double> GA_PDPPBO::CreateGene(unsigned int trIndex) {
    unsigned int select_index;
    select_index = indexDistOption.at(trIndex)(this->random_generator);
    return PDPPBitRateAllDistOption.at(trIndex).at(select_index);
}


