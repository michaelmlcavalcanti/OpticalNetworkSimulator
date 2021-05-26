/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Data.cpp
 * Author: bruno
 * 
 * Created on August 8, 2018, 6:25 PM
 */

#include "../../include/Data/Data.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Data/Parameters.h"
#include "../../include/Data/Options.h"
#include "../../include/SimulationType/SimulationType.h"
#include "../../include/SimulationType/SimulationGA.h"
#include "../../include/SimulationType/SimulationPSO.h"
#include "../../include/Calls/Call.h"
#include "../../include/ResourceAllocation/Route.h"
#include "../../include/Algorithms/Algorithms.h"
#include "../../include/Structure/Topology.h"
#include "../../include/Algorithms/GA/IndividualPDPPBO.h"

std::ostream& operator<<(std::ostream& ostream, 
const Data* data) {
    ostream << "Simulation time:" << data->GetSimulTime() 
            << "  Real simul time: " << data->GetRealSimulTime()
            << "  Number of requests:" << data->GetNumberReq() << std::endl;
    
    TypeSimulation type = data->simulType->GetTypeSimulation();
    switch(type){
        case MultiLoadSimulationType:
            ostream << "Load point:" << data->simulType->GetParameters()->
            GetLoadPoint(data->GetActualIndex());
            break;
        case IncNumDevType:
            ostream << "Load point:" << data->simulType->GetParameters()->
            GetMaxLoadPoint();
            break;
        default:
            ostream <<  "Invalid Type of simulation" << std::endl;
            std::abort();
    }
    
    ostream << "  ReqBP:" << data->GetReqBP() 
            << "  SlotsBP:" << data->GetSlotsBP() 
            << "  HopsMed:" << data->GetAverageNumHops() << std::endl;
    ostream << "NetOcc:" << data->GetNetOccupancy() 
            << "  NetUti:" << data->GetAverageNetUtilization() 
            << " NetFrag:" << data->GetNetworkFragmentationRatio() << std::endl;
    ostream << "ProtRate:" << data->GetProtRate()
            << "  BetaAverg:" << data->GetNetBetaAverage() << std::endl;
    
    return ostream;
}

Data::Data(SimulationType* simulType) 
: simulType(simulType), numberReq(0), numberBlocReq(0), numberAccReq(0),
  numberSlotsReq(0), numberBlocSlots(0), numberAccSlots(0), numberAccSlotsInt(0),
  numHopsPerRoute(0), netOccupancy(0), accReqUtilization(0),
  netFragmentationRatio(0), accumNetFragmentationRatio(0) , fragPerTraffic(0),
  linksUse(0), slotsRelativeUse(0), simulTime(0), realSimulTime(0),
  actualIndex(0), protectedCalls(0), nonProtectedCalls(0), sumCallsBetaAverage(0){
    
}

Data::~Data() {
    
}

void Data::Initialize() {
    int size = simulType->GetParameters()->GetNumberLoadPoints();
    int numberSlots = simulType->GetParameters()->GetNumberSlots();
    
    numberReq.assign(size, 0.0);
    numberBlocReq.assign(size, 0.0);
    numberAccReq.assign(size, 0.0);
    numberSlotsReq.assign(size, 0.0);
    numberBlocSlots.assign(size, 0.0);
    numberAccSlots.assign(size, 0.0);
    numberAccSlotsInt.assign(size, 0.0);
    numHopsPerRoute.assign(size, 0.0);
    netOccupancy.assign(size, 0.0);
    accReqUtilization.assign(size, 0.0);
    netFragmentationRatio.assign(size, 0.0);
    accumNetFragmentationRatio.resize(size);
    fragPerTraffic.resize(size);
    linksUse.resize(size);
    simulTime.assign(size, 0.0);
    realSimulTime.assign(size, 0.0);
    slotsRelativeUse.resize(size);
    for(unsigned int a = 0; a < slotsRelativeUse.size(); a++){
        std::vector<double> vec(numberSlots, 0);
        slotsRelativeUse.at(a) = vec;
    }
    protectedCalls.resize(size);
    nonProtectedCalls.resize(size);
    sumCallsBetaAverage.resize(size);
}

void Data::Initialize(unsigned int numPos) {
    
    numberReq.assign(numPos, 0.0);
    numberBlocReq.assign(numPos, 0.0);
    numberAccReq.assign(numPos, 0.0);
    numberSlotsReq.assign(numPos, 0.0);
    numberBlocSlots.assign(numPos, 0.0);
    numberAccSlots.assign(numPos, 0.0);
    numHopsPerRoute.assign(numPos, 0.0);
    netOccupancy.assign(numPos, 0.0);
    accReqUtilization.assign(numPos, 0.0);
    netFragmentationRatio.assign(numPos, 0.0);
    accumNetFragmentationRatio.resize(numPos);
    fragPerTraffic.resize(numPos);
    linksUse.resize(numPos);
    simulTime.assign(numPos, 0.0);
    realSimulTime.assign(numPos, 0.0);
    slotsRelativeUse.resize(numPos);
    protectedCalls.resize(numPos);
    nonProtectedCalls.resize(numPos);
    sumCallsBetaAverage.resize(numPos);
}

void Data::StorageCall(Call* call) {
    double bitRate = call->GetBitRate();
    unsigned int numSlot = call->GetNumberSlots();
    
    switch(call->GetStatus()){
        case Accepted:
            numberAccReq.at(actualIndex)++;
            numberAccSlots.at(actualIndex) += bitRate;
            numberAccSlotsInt.at(actualIndex) += numSlot;
            numHopsPerRoute.at(actualIndex) += (double) 
            call->GetRoute()->GetNumHops();
            netOccupancy.at(actualIndex) += 
            (double) call->GetTotalNumSlots();
            accReqUtilization.at(actualIndex) += 
            ((double) call->GetTotalNumSlots()) * call->GetDeactivationTime();
            this->SetSlotsRelativeUse(call);
            break;
        case Blocked:
            numberBlocReq.at(actualIndex)++;
            numberBlocSlots.at(actualIndex) += bitRate;
            break;
        case NotEvaluated:
            std::cerr << "Not evaluated call" <<  std::endl;
            std::abort();
    }
    numberSlotsReq.at(actualIndex) += bitRate;
}

void Data::SaveLog() {
    std::ofstream &logOfstream = this->simulType->GetInputOutput()
                                     ->GetLogFile();
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    logOfstream << "DATA" << std::endl;
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        logOfstream << this << std::endl;
    }
}

void Data::SaveLog(std::vector<unsigned> vecParam) {
    std::ofstream &logOfstream = this->simulType->GetInputOutput()
                                     ->GetLogFile();
    unsigned int numPoints = vecParam.size();
    
    logOfstream << "DATA" << std::endl;
    
    for(unsigned int a = 0; a < numPoints; a++){
        this->SetActualIndex(a);
        logOfstream << "Number of devices: " << vecParam.at(a) << std::endl;
        logOfstream << this << std::endl;
    }
}

void Data::SaveBP() {
    std::ofstream &callReqBP = this->simulType->GetInputOutput()
                                        ->GetReqBpFile();
    std::ofstream &bandwidthBP = simulType->GetInputOutput()
                                          ->GetBandBpFile();
    
    this->SaveCallReqBP(callReqBP);
    this->SaveBandwidthBP(bandwidthBP);
}

void Data::SaveNetUtiliz() {
    std::ofstream &netUtiliz = this->simulType->GetInputOutput()
                                     ->GetNetUtilizFile();
    
    this->SaveNetUtiliz(netUtiliz);
}

void Data::SaveNetFrag() {
    std::ofstream &netFrag = this->simulType->GetInputOutput()
                                     ->GetNetFragFile();
    
    this->SaveNetFrag(netFrag);
}

void Data::SaveAccumNetFrag() {
   std::ofstream &accumNetFrag = this->simulType->GetInputOutput()
                                     ->GetAccumNetFragFile();
    
    this->SaveAccumNetFrag(accumNetFrag);
}

void Data::SaveFragTraffic() {
    std::ofstream &netFrag = this->simulType->GetInputOutput()
                                 ->GetFragBandFile();
    this->SaveBandFrag(netFrag);
}

void Data::SaveLinksUse() {
    std::ofstream &linksUse = this->simulType->GetInputOutput()->GetLinksUse();
    
    this->SaveLinksUse(linksUse);
}

void Data::SaveSlotsRelativeUse() {
    std::ofstream &slotsRelativeUse = this->simulType->GetInputOutput()->
            GetSlotsRelativeUse();
    
    this->SaveSlotsRelativeUse(slotsRelativeUse);
}

void Data::SaveNetProtRate() {
    std::ofstream &netProtRate = this->simulType->GetInputOutput()->GetNetProtRate();
    
    this->SaveNetProtRate(netProtRate);
}

void Data::SaveNetNonProtRate() {
    std::ofstream &netNonProtRate = this->simulType->GetInputOutput()->GetNetNonProtRate();
    
    this->SaveNetNonProtRate(netNonProtRate);
}

void Data::SaveNetBetaAverage() {
    std::ofstream &netBetaAverage = this->simulType->GetInputOutput()->GetNetBetaAverage();
    
    this->SaveNetBetaAverage(netBetaAverage);
}

void Data::SaveNumHopsRoutes() {
    std::ofstream &numHopsRoutes = this->simulType->GetInputOutput()->GetNumHopsRoutes();

    this->SaveNumHopsRoutes(numHopsRoutes);
}


void Data::SaveBP(std::vector<unsigned> vecParam) {
    std::ofstream &callReqBP = this->simulType->GetInputOutput()
                                        ->GetReqBpFile();
    std::ofstream &bandwidthBP = simulType->GetInputOutput()
                                          ->GetBandBpFile();
    
    this->SaveCallReqBP(callReqBP, vecParam);
    this->SaveBandwidthBP(bandwidthBP, vecParam);
}

void Data::SaveGaFiles() {
    std::ofstream& logOfstream = this->simulType->GetInputOutput()
                                     ->GetLogFile();
    std::ofstream& initPop = this->simulType->GetInputOutput()
                                        ->GetIniPopulationFile();
    std::ofstream& bestInds = this->simulType->GetInputOutput()
                                 ->GetBestIndividualsFile();
    std::ofstream& bestInd = this->simulType->GetInputOutput()
                                ->GetBestIndividualFile();
    std::ofstream& worstInds = this->simulType->GetInputOutput()
                                 ->GetWorstIndividualsFile();
    
    if(this->simulType->GetOptions()->IsGA_SO())
        this->SaveGaSoFiles(logOfstream, initPop, bestInds, worstInds, bestInd);
    else if(this->simulType->GetOptions()->IsGA_MO())
        this->SaveGaMoFiles(logOfstream, bestInds, bestInd);
}

void Data::SaveFilesPSO() {
    std::ofstream& logOfstream = simulType->GetInputOutput()->GetLogFile();
    std::ofstream& bestParticle = simulType->GetInputOutput()
                                           ->GetBestParticle();
    std::ofstream& bestParticles = simulType->GetInputOutput()
                                            ->GetBestParticles();
    PSO* pso = dynamic_cast<SimulationPSO*>(simulType)->GetPSO();
    unsigned int numIterations = pso->GetNumberIterations();
    
    pso->PrintParameters(logOfstream);
    
    //Save the Log.txt and the best particle of each iteration.
    for(unsigned int a = 1; a <= numIterations; a++){
        pso->SetActualIteration(a);
        logOfstream << pso << std::endl;
        bestParticles << a << "\t" << pso->GetBestParticle()->GetMainParameter()
                      << std::endl;
    }
    
    //Save the best particle coefficients.
    std::vector<double> coefficients = pso->GetBestParticle()
                                          ->GetBestPosition();
    
    for(unsigned int a = 0; a < coefficients.size(); a++)
        bestParticle << coefficients.at(a) << std::endl;
}

void Data::SetNumberReq(double numReq) {
    assert(numReq >= 0);
    this->numberReq.at(this->actualIndex) = numReq;
}

double Data::GetNumberReq() const {
    return this->numberReq.at(this->actualIndex);
}

double Data::GetNumberBlocReq() const {
    return this->numberBlocReq.at(this->actualIndex);
}

double Data::GetNumberAccReq() const {
    return this->numberAccReq.at(this->actualIndex);
}

double Data::GetReqBP() const {
    return this->GetNumberBlocReq()/this->GetNumberReq();
}

double Data::GetNumberSlotsReq() const {
    return this->numberSlotsReq.at(this->actualIndex);
}

double Data::GetNumberBlocSlots() const {
    return this->numberBlocSlots.at(this->actualIndex);
}

double Data::GetNumberAccSlots() const {
    return this->numberAccSlots.at(this->actualIndex);
}

double Data::GetSlotsBP() const {
    return this->GetNumberBlocSlots()/this->GetNumberSlotsReq();
}

double Data::GetNumHopsPerRoute() const {
    return this->numHopsPerRoute.at(this->actualIndex);
}

double Data::GetAverageNumHops() const {
    return (this->GetNumHopsPerRoute() / this->GetNumberAccReq());
}

double Data::GetNetOccupancy() const {
    return this->netOccupancy.at(this->actualIndex);
}

double Data::GetAverageNetUtilization() const {
    double total = (double) simulType->GetTopology()->GetNumLinks() * 
    (double) simulType->GetTopology()->GetNumSlots() * simulTime.at(actualIndex);
    
    return accReqUtilization.at(actualIndex) / total;
}

void Data::UpdateFragmentationRatio(double ratio) {
    assert(ratio >= 0.0 && ratio <= 1.0);
    
    accumNetFragmentationRatio.at(actualIndex).push_back(ratio);
    netFragmentationRatio.at(actualIndex) += ratio;
    netFragmentationRatio.at(actualIndex) /= 2;
}

double Data::GetNetworkFragmentationRatio() const {
    return netFragmentationRatio.at(actualIndex);
}

TIME Data::GetSimulTime() const {
    return this->simulTime.at(this->actualIndex);
}

void Data::SetSimulTime(const TIME simulTime) {
    assert(simulTime > 0);
    this->simulTime.at(this->actualIndex) = simulTime;
}

TIME Data::GetRealSimulTime() const {
    return this->realSimulTime.at(this->actualIndex);
}

void Data::SetRealSimulTime(const TIME simulTime) {
    this->realSimulTime.at(this->actualIndex) = simulTime;
}

void Data::SetLinksUse(Topology* topology) {
    unsigned int numNodes = topology->GetNumNodes();
    Link* auxLink;
    std::pair<unsigned, unsigned> nodePair;
    std::pair<std::pair<unsigned, unsigned>, unsigned> use;
    
    for(unsigned int orN = 0; orN < numNodes; orN++){
        for(unsigned int deN = 0; deN < numNodes; deN++){
            auxLink = topology->GetLink(orN, deN);
            
            if(auxLink){
                nodePair = std::make_pair(orN, deN);
                use = std::make_pair(nodePair, auxLink->GetUse());
                linksUse.at(actualIndex).insert(use);
            }
        }
    }
}

void Data::SetSlotsRelativeUse(Call* call) {
    unsigned int callFirstSlot = call->GetFirstSlot();
    unsigned int callLastSlot = call->GetLastSlot();
    
    for (unsigned a = callFirstSlot; a <= callLastSlot; a++){
        slotsRelativeUse.at(actualIndex).at(a)++;
    }
}

void Data::SetBandFrag(const double band, const double netFrag) {
    std::pair<double, double> frag;
    frag = std::make_pair(band, netFrag);
    fragPerTraffic.at(actualIndex).push_back(frag);
}

unsigned int Data::GetActualIndex() const {
    return actualIndex;
}

void Data::SetActualIndex(unsigned int actualIndex) {
    this->actualIndex = actualIndex;
}

void Data::SetProtectedCalls() {
    this->protectedCalls.at(this->actualIndex)++;
}

double Data::GetProtectedCalls() const {
    return this->protectedCalls.at(this->actualIndex);
}

void Data::SetNonProtectedCalls() {
    this->nonProtectedCalls.at(this->actualIndex)++;
}

double Data::GetNonProtectedCalls() const {
    return this->nonProtectedCalls.at(this->actualIndex);
}

double Data::GetProtRate() const {
    return this->GetProtectedCalls()/((this->GetProtectedCalls())+
    (this->GetNonProtectedCalls()));
}

double Data::GetNonProtRate() const {
    return this->GetNonProtectedCalls()/((this->GetProtectedCalls())+
    (this->GetNonProtectedCalls()));
}

double Data::GetSumCallsBetaAverage() const {
    return this->sumCallsBetaAverage.at(actualIndex);
}

void Data::SetSumCallsBetaAverage(double callBetaAverage) {
    this->sumCallsBetaAverage.at(actualIndex) += callBetaAverage;
}

double Data::GetNetBetaAverage() const {
    return this->GetSumCallsBetaAverage() / this->GetProtectedCalls();
}

void Data::SaveCallReqBP(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        ostream << this->simulType->GetParameters()->GetLoadPoint(
                   this->GetActualIndex()) << "\t" << this->GetReqBP() 
                << std::endl;
    }
}

void Data::SaveBandwidthBP(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        ostream << this->simulType->GetParameters()->GetLoadPoint(
                   this->GetActualIndex()) << "\t" << this->GetSlotsBP() 
                << std::endl;
    }
}

void Data::SaveCallReqBP(std::ostream& ostream, std::vector<unsigned> vec) {
    assert(vec.size() == this->numberReq.size());
    
    for(unsigned int a = 0; a < vec.size(); a++){
        this->SetActualIndex(a);
        ostream << vec.at(a) << "\t" << this->GetReqBP() << std::endl;
    }
}

void Data::SaveBandwidthBP(std::ostream& ostream, std::vector<unsigned> vec) {
    assert(vec.size() == this->numberReq.size());
    
    for(unsigned int a = 0; a < vec.size(); a++){
        this->SetActualIndex(a);
        ostream << vec.at(a) << "\t" << this->GetSlotsBP() << std::endl;
    }
}

void Data::SaveNetUtiliz(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        ostream << this->simulType->GetParameters()->GetLoadPoint(
                   this->GetActualIndex()) << "\t" 
                << this->GetAverageNetUtilization() << std::endl;
    }
}

void Data::SaveNetFrag(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        ostream << this->simulType->GetParameters()->GetLoadPoint(
                   this->GetActualIndex()) << "\t" 
                << this->GetNetworkFragmentationRatio() << std::endl;
    }
}

void Data::SaveAccumNetFrag(std::ostream& ostream) {
unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
        
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        
        for(unsigned int b = 0; b < accumNetFragmentationRatio.at(actualIndex).
        size(); b++){
            ostream << b << "\t" << accumNetFragmentationRatio.at(actualIndex).at(b) 
            << std::endl;
        }  
        ostream << std::endl;    
    }        
}

void Data::SaveBandFrag(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    std::vector<std::pair<double, double>> bandFrag;
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        bandFrag = fragPerTraffic.at(actualIndex);
        
        for(auto const& pair: bandFrag){
            ostream << pair.first << "\t" << pair.second << std::endl;
        }
        ostream << std::endl;
    }
}

void Data::SaveLinksUse(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    std::map<std::pair<unsigned, unsigned>, unsigned> auxMap;
    std::pair<unsigned, unsigned> nodePair;
    unsigned int use;
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        auxMap = linksUse.at(actualIndex);
        
        for(auto const& pair: auxMap){
            nodePair = pair.first;
            use = pair.second;
            ostream << nodePair.first << "->" << nodePair.second << "\t"
                    << use << std::endl;
        }
        ostream << std::endl;
    }
}

void Data::SaveSlotsRelativeUse(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    double numSlots = simulType->GetParameters()->GetNumberSlots();
        
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
            
        for(unsigned int b = 0; b < numSlots; b++){
            slotsRelativeUse.at(actualIndex).at(b) = 
            slotsRelativeUse.at(actualIndex).at(b) / numberAccSlotsInt.at(actualIndex);
            ostream << b << "\t" << slotsRelativeUse.at(actualIndex).at(b) 
            << std::endl;
        }  
        ostream << std::endl;    
    }        
}

void Data::SaveNetProtRate(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        ostream << this->simulType->GetParameters()->GetLoadPoint(
                   this->GetActualIndex()) << "\t" << this->GetProtRate() 
                << std::endl;
    }
}

void Data::SaveNetNonProtRate(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        ostream << this->simulType->GetParameters()->GetLoadPoint(
                   this->GetActualIndex()) << "\t" << this->GetNonProtRate() 
                << std::endl;
    }
}

void Data::SaveNetBetaAverage(std::ostream& ostream) {
    unsigned int numLoadPoints = this->simulType->GetParameters()
                                     ->GetNumberLoadPoints();
    
    for(unsigned int a = 0; a < numLoadPoints; a++){
        this->SetActualIndex(a);
        ostream << this->simulType->GetParameters()->GetLoadPoint(
                   this->GetActualIndex()) << "\t" << this->GetNetBetaAverage() 
                << std::endl;
    }
}

void Data::SaveNumHopsRoutes(std::ostream &ostream) {
    std::vector<std::vector<std::shared_ptr<Route>>> auxAllRoutes = this->simulType->
            GetResourceAlloc()->resources->allRoutes;
    std::vector<std::vector<std::vector<std::shared_ptr<Route>>>>
            auxProtectionAllRoutes = this->simulType->GetResourceAlloc()->
            resources->protectionAllRoutes;
    unsigned int numNodes = this->simulType->GetTopology()->GetNumNodes();

    for(unsigned int orN = 0; orN < numNodes; orN++) {
        for (unsigned int deN = 0; deN < numNodes; deN++) {
            if (orN != deN && auxAllRoutes.at(orN*numNodes+deN).front() != nullptr) {
                std::vector<int> path = auxAllRoutes.at(orN*numNodes+deN).front().get()->GetPath();
                for(unsigned int a = 0; a < path.size(); a++){
                    ostream << path.at(a) <<"-";
                }
                ostream << "\t" << auxAllRoutes.at(orN*numNodes+deN).front().get()->GetNumHops()
                << std::endl;
                unsigned int numDisjRoutes = auxProtectionAllRoutes.at(orN*numNodes+deN).front().size();
                for(unsigned int kd = 0; kd < numDisjRoutes; kd++) {
                    if(auxProtectionAllRoutes.at(orN * numNodes + deN).front().at(kd) == nullptr)
                        continue;
                    std::vector<int> path = auxProtectionAllRoutes.at(
                            orN * numNodes + deN).front().at(kd).get()->GetPath();
                    for (unsigned int a = 0; a < path.size(); a++) {
                        ostream << path.at(a) <<"-";
                    }
                    ostream << "\t" << auxProtectionAllRoutes.at(orN * numNodes + deN)
                    .front().at(kd).get()->GetNumHops()  << std::endl;
                }
            }
            ostream << std::endl;
        }
    }
}


void Data::SaveGaSoFiles(std::ostream& logOfstream, std::ostream& initPop, 
std::ostream& bestInds, std::ostream& worstInds, std::ostream& bestInd) {
    GA_SO* ga = dynamic_cast<GA_SO*>( dynamic_cast<SimulationGA*>
                                      (this->simulType)->GetGA() );
    
    ga->printParameters(logOfstream);
    this->SaveBestWorstIndividuals(ga, logOfstream, bestInds, worstInds);
    this->SaveBestIndividual(ga, bestInd);
    this->SaveInitPopulation(ga, initPop);
}

void Data::SaveBestWorstIndividuals(GA_SO* ga, std::ostream& logOfstream, 
std::ostream& bestInds, std::ostream& worstInds) {
    unsigned int numGen = ga->GetNumberGenerations();
    
    for(unsigned int a = 1; a <= numGen; a++){
        ga->SetActualGeneration(a);
        logOfstream << ga << std::endl;
        bestInds << a << "\t" << ga->GetBestIndividual()->GetMainParameter() 
                << std::endl;
        worstInds << a << "\t" << ga->GetWorstIndividual()->GetMainParameter()
                 << std::endl;
    }
}

void Data::SaveBestIndividual(GA_SO* ga, std::ostream& bestInd) {
    //Make function to check the cast for the best individual
    //and a switch function for casting according to the individual. 
    IndividualBool* ind = dynamic_cast<IndividualBool*>
                          (ga->GetBestIndividual());
    
    std::vector<bool> gene = ind->GetGenes();
    for(unsigned int a = 0; a < gene.size(); a++){
        bestInd << gene.at(a) << std::endl;
    }
}

void Data::SaveInitPopulation(GA_SO* ga, std::ostream& initPop) {
    unsigned int numIniPop = ga->GetNumberIndividuals();
    
    for(unsigned int a = 0; a < numIniPop; a++){
        initPop << 0 << "\t" << ga->GetIniIndividual(a)->GetMainParameter()
                << std::endl;
    }
}

void Data::SaveGaMoFiles(std::ostream& logOfstream, std::ostream& bestInds, 
std::ostream& bestInd) {
    GA_MO* ga = dynamic_cast<GA_MO*>( dynamic_cast<SimulationGA*>
                                      (this->simulType)->GetGA() );
    
    ga->printParameters(logOfstream);
    this->SaveParetoFronts(ga, logOfstream, bestInds);
    this->SaveLastIndividuals(ga, bestInd);
}

void Data::SaveParetoFronts(GA_MO* ga, std::ostream& logOfstream, 
std::ostream& bestInds) {
    std::vector<Individual*> auxVecInd(0);
    unsigned int numGen = ga->GetNumberGenerations();
    unsigned int passo = ga->GetSaveStep();
    
    for(unsigned int a = 0; a <= numGen; a++){  
        ga->SetActualGeneration(a);
        auxVecInd.clear();
        
        if(a == 0)
            auxVecInd = ga->GetIniPopulation();
        else if(a % passo == 0){
            logOfstream << ga << std::endl;
            auxVecInd = ga->GetParetoFront();
        }
        
        for(auto it: auxVecInd){
            bestInds << it->GetMainParameter() << "\t" 
                     << it->GetSecondParameter() << std::endl;
        }
        
        if(!auxVecInd.empty())
            bestInds << std::endl;
    }
}

void Data::SaveLastIndividuals(GA_MO* ga, std::ostream& bestInd) {
    //Make function to check the cast for the best individual
    //and a switch function for casting according to the individual.

    ga->SetActualGeneration(ga->GetNumberGenerations());
    std::vector<Individual*> auxVecInd = ga->GetParetoFront();

    for(auto ind: auxVecInd){
        ind->Save(bestInd);
        bestInd << std::endl;
    }
}




