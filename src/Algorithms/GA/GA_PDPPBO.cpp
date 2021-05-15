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
#include "../../../include/Data/Data.h"

GA_PDPPBO::GA_PDPPBO(SimulationType* simul) : GA_MO(simul), pdppbo(nullptr), numNodes(0), numTraffic(0),
PDPPBitRateAllDistOption(0), indexDistOption(0) {
    ResourceDeviceAlloc* resource_alloc;
    resource_alloc = dynamic_cast<ResourceDeviceAlloc*>(simul->GetResourceAlloc());
    pdppbo = dynamic_cast<PartitioningDedicatedPathProtection*>(resource_alloc->GetProtectionScheme());
    // Include asserts (Simulation is protection is active, if the protection is the right one, etc.)
   // assert(this->GetSimul()->GetOptions()->GetProtectionOption() == ProtectionPDPPBO_GA);
}

GA_PDPPBO::~GA_PDPPBO() {

}

void GA_PDPPBO::Initialize() {
    GA_MO::Initialize();
    this->SetNumNodes(this->GetSimul()->GetTopology()->GetNumNodes());
    //Mudar pra função interna
    numTraffic = this->GetSimul()->GetTraffic()->GetVecTraffic().size();
    this->LoadPDPPBitRateAllDistOption();
    this->CreateIndexDistributions();
}

void GA_PDPPBO::InitializePopulation() {
    assert(this->actualParetoFronts.empty() && this->totalPopulation.empty());
    std::vector<std::shared_ptr<Individual>> auxVecInd(0);

    for(unsigned int a = 0; a < this->GetNumberIndividuals(); a++)
        auxVecInd.push_back(std::make_shared<IndividualPDPPBO>(this));

    this->actualParetoFronts.push_back(auxVecInd);
}

void GA_PDPPBO::CreateNewPopulation() {
    this->totalPopulation.clear();
    this->Crossover();
    this->Mutation();
    //this->UpdatePDPPBO();
}

void GA_PDPPBO::Crossover() {
    assert(this->totalPopulation.empty());
    assert(this->GetNumIndParetoFronts() == this->GetNumberIndividuals());
    std::shared_ptr<IndividualPDPPBO> auxInd1, auxInd2;
    std::vector<std::shared_ptr<IndividualPDPPBO>> auxVecTotalPop;
    
    //Put all individual in the Pareto fronts to an auxiliary vector.
    for(auto it1: this->actualParetoFronts){
        for(auto it2: it1){
            auxVecTotalPop.push_back(
            std::dynamic_pointer_cast<IndividualPDPPBO>(it2));
        }
    }
    
    //Shuffle the auxiliary vector.
    std::shuffle(auxVecTotalPop.begin(), auxVecTotalPop.end(), 
                 this->random_generator);
    
    while(!auxVecTotalPop.empty()){
        auxInd1 = auxVecTotalPop.back();
        auxVecTotalPop.pop_back();
        auxInd2 = auxVecTotalPop.back();
        auxVecTotalPop.pop_back();
        
        this->GenerateNewIndividuals(auxInd1.get(), auxInd2.get());
    }
}

void GA_PDPPBO::GenerateNewIndividuals(const IndividualPDPPBO * const ind1, 
const IndividualPDPPBO * const ind2) {
    
    this->UniformCrossover(ind1, ind2);
}

void GA_PDPPBO::UniformCrossover(const IndividualPDPPBO * const ind1, 
const IndividualPDPPBO * const ind2) {
    std::shared_ptr<IndividualPDPPBO> newInd1 = std::make_shared<IndividualPDPPBO>(this);
    std::shared_ptr<IndividualPDPPBO> newInd2 = std::make_shared<IndividualPDPPBO>(this);
    double auxProb;
    unsigned int numTraffic = this->GetNumTraffic();
    
    for(unsigned int orN = 0; orN < this->numNodes; orN++){
        for(unsigned int deN = 0; deN < this->numNodes; deN++){                      
            for(unsigned int trIndex = 0; trIndex < numTraffic; trIndex++){
                auxProb = this->GetProbDistribution();
                
                if(auxProb < this->GetProbCrossover()){
                    newInd1->SetGene(orN, deN, trIndex, ind1->GetGene(orN, deN, 
                                                                  trIndex));
                    newInd2->SetGene(orN, deN, trIndex, ind2->GetGene(orN, deN, 
                                                                  trIndex));
                }
                else{
                    newInd1->SetGene(orN, deN, trIndex, ind2->GetGene(orN, deN, 
                                                                  trIndex));
                    newInd2->SetGene(orN, deN, trIndex, ind1->GetGene(orN, deN, 
                                                                  trIndex));
                }
            }
        }
    }

    this->totalPopulation.push_back(newInd1);
    this->totalPopulation.push_back(newInd2);
}

void GA_PDPPBO::Mutation() {
    assert(this->totalPopulation.size() == this->GetNumberIndividuals());
    assert(this->GetNumIndParetoFronts() == this->GetNumberIndividuals());
    unsigned int popSize = this->GetNumberIndividuals();
    
    //Apply the mutation only in the copies created.
    for(unsigned int a = 0; a < popSize; a++){
        this->MutateIndividual(dynamic_cast<IndividualPDPPBO*>
                              (this->totalPopulation.at(a).get()));
    }
    
    for(auto it: this->actualParetoFronts){
        this->totalPopulation.insert(this->totalPopulation.end(),
             it.begin(), it.end());
    }
    
    this->actualParetoFronts.clear();
}

void GA_PDPPBO::MutateIndividual(IndividualPDPPBO * const ind) {
    double auxProb;
    unsigned int numTraffic = this->GetNumTraffic();
    
    for(unsigned int orN = 0; orN < this->numNodes; orN++){
        for(unsigned int deN = 0; deN < this->numNodes; deN++){                      
            for(unsigned int trIndex = 0; trIndex < numTraffic; trIndex++){
                auxProb = this->GetProbDistribution();
                
                if(auxProb < this->GetProbMutation()){
                    ind->SetGene(orN, deN, trIndex, 
                                 this->CreateGene(trIndex));
                }
            }
        }
    }
}

void GA_PDPPBO::UpdatePDPPBO() {
    std::shared_ptr<IndividualPDPPBO> auxInd;
    
    for(auto it: totalPopulation){
        auxInd = std::dynamic_pointer_cast<IndividualPDPPBO>(it);
       //auxInd->SetTotalNumInterRoutes();
    }
}

void GA_PDPPBO::CreateIndexDistributions(void) {
    indexDistOption.resize(PDPPBitRateAllDistOption.size());
    for(unsigned int a = 0; a < PDPPBitRateAllDistOption.size(); a++) {
        indexDistOption.at(a) = std::uniform_int_distribution<unsigned int>
                (0,PDPPBitRateAllDistOption.at(a).size()-1);
    }
}

void GA_PDPPBO::ApplyIndividual(Individual* ind) {
    IndividualPDPPBO* auxInd = dynamic_cast<IndividualPDPPBO*>(ind);
    this->pdppbo->SetPDPPBitRateNodePairsDist(auxInd->GetGenes());

}

void GA_PDPPBO::SetIndParameters(Individual* ind) {
    double blockProb = this->GetSimul()->GetData()->GetReqBP();
    double betaAverage = this->GetSimul()->GetData()->GetNetBetaAverage();
    
    IndividualPDPPBO* auxInd = dynamic_cast<IndividualPDPPBO*>(ind);
    auxInd->SetBlockProb(blockProb);
    auxInd->SetBetaAverage(betaAverage);
    auxInd->SetCount(auxInd->GetCount() + 1);
}

unsigned int GA_PDPPBO::GetNumNodes() const {
    return numNodes;
}

void GA_PDPPBO::SetNumNodes(unsigned int numNodes) {
    assert(numNodes > 0);
    
    this->numNodes = numNodes;
}

void GA_PDPPBO::LoadPDPPBitRateAllDistOption() {
    std::vector<double> VecTraffic;
    VecTraffic = this->GetSimul()->GetTraffic()->GetVecTraffic();
    numTraffic = this->GetSimul()->GetTraffic()->GetVecTraffic().size();
    PDPPBitRateAllDistOption.resize(numTraffic);
    std::vector<double> auxBitRateOption;
    double beta = this->GetSimul()->GetParameters()->GetBeta();
    unsigned int trIndex = 0;
  
    if(beta != 0){     
        for(auto it : VecTraffic){
            double BR = it;
            double BRmin = ((it*(1 - beta)));
            double BRdown = ((it/2) - (0.2*it));
            double BRup = ((it/2) + (0.2*it));
            
            if(BR == 100000000000){
                for(double a = BRdown; a <= BRup; a = a+5e9){
                    for(double b = BRdown; b <= BRup; b = b+5e9){                    
                        for(double c = BRdown; c <= BRup; c = c+5e9){
                            if (a + b >= BRmin && b + c >= BRmin && a + c >= BRmin 
                                && a + b + c <= 1.3*BR){                            
                                auxBitRateOption.push_back(a);
                                auxBitRateOption.push_back(b);
                                auxBitRateOption.push_back(c);
                                PDPPBitRateAllDistOption.at(trIndex).push_back(auxBitRateOption);                       
                                auxBitRateOption.clear();
                            }
                        }             
                    }
                }
            }
            else if(BR == 200000000000){
                for(double a = BRdown; a <= BRup; a = a+10e9){
                    for(double b = BRdown; b <= BRup; b = b+10e9){                    
                        for(double c = BRdown; c <= BRup; c = c+10e9){
                            if (a + b >= BRmin && b + c >= BRmin && a + c >= BRmin 
                                && a + b + c <= 1.3*BR){                            
                                auxBitRateOption.push_back(a);
                                auxBitRateOption.push_back(b);
                                auxBitRateOption.push_back(c);
                                PDPPBitRateAllDistOption.at(trIndex).push_back(auxBitRateOption);                       
                                auxBitRateOption.clear();
                            }
                        }             
                    }
                }
            }
            else if(BR == 400000000000){
                for(double a = BRdown; a <= BRup; a = a+20e9){
                    for(double b = BRdown; b <= BRup; b = b+20e9){                    
                        for(double c = BRdown; c <= BRup; c = c+20e9){
                            if (a + b >= BRmin && b + c >= BRmin && a + c >= BRmin 
                                && a + b + c <= 1.3*BR){                            
                                auxBitRateOption.push_back(a);
                                auxBitRateOption.push_back(b);
                                auxBitRateOption.push_back(c);
                                PDPPBitRateAllDistOption.at(trIndex).push_back(auxBitRateOption);                       
                                auxBitRateOption.clear();
                            }
                        }             
                    }
                }
            }
            else {
                std::cout << "Invalid traffic option. Traffic option should be 100/200/400 GB";
            }
            trIndex++;
        }
    }
    
    else if(beta == 0){
        std::cout << "Invalid beta Squeezing. Beta should be different of zero";
    }
}

unsigned int GA_PDPPBO::GetNumTraffic() const {
    return numTraffic;
}

std::vector<double> GA_PDPPBO::CreateGene(unsigned int trIndex) {
    unsigned int select_index;
    select_index = indexDistOption.at(trIndex)(this->random_generator);
    return PDPPBitRateAllDistOption.at(trIndex).at(select_index);
}




