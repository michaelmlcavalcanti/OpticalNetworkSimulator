/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Options.cpp
 * Author: bruno
 * 
 * Created on August 3, 2018, 1:18 AM
 */

#include "../../include/Data/Options.h"

#include "../../include/SimulationType/SimulationType.h"
#include "../../include/Data/InputOutput.h"
#include "../../include/Data/Parameters.h"

const boost::unordered_map<TopologyOption, std::string> 
Options::mapTopologyOptions = boost::assign::map_list_of
    (TopologyInvalid, "Invalid")
    (TopologyNSFNet, "NSFNet")
    (TopologyRing, "Ring")
    (TopologyToroidal, "Toroidal")
    (TopologyGermany,  "Germany")
    (TopologyEON_RT, "EON_RT")
    (TopologyItaly, "Italy")
    (Topogy4_Nodes, "4_Nodes")
    (TopologyCoreNet, "CoreNet")
    (TopologySmallNet, "SmallNet")
    (TopologyWaxman, "Waxman")
    (TopologyUS_Backbone, "US_Backbone")
    (TopologyEuropean, "European");

const boost::unordered_map<RoutingOption, std::string>
Options::mapRoutingOptions = boost::assign::map_list_of
    (RoutingInvalid, "Invalid")
    (RoutingDJK, "Dijkstra")
    (RoutingYEN, "YEN")
    (RoutingBSR, "BSR")
    (RoutingBSR_YEN, "BSR_YEN")
     (RoutingMP, "MPR");

const boost::unordered_map<SpectrumAllocationOption, std::string>
Options::mapSpecAlgOptions = boost::assign::map_list_of
    (SpecAllInvalid, "Invalid")
    (SpecAllRandom, "Random")
    (SpecAllFF, "First Fit")
    (SpecAllMSCL, "MSCL");

const boost::unordered_map<LinkCostType, std::string>
Options::mapLinkCostType = boost::assign::map_list_of
    (Invalid, "Invalid cost type")
    (LinkCostHop, "Hops")
    (LinkCostLength, "Length")
    (LinkCostNormLength, "Normalized length");

const boost::unordered_map<TrafficOption, std::string>
Options::mapTrafficOptions = boost::assign::map_list_of
    (TrafficInvalid, "Invalid")
    (Traffic_100_200_400, "100-200-400")
    (Traffic_50_120_300, "50-120-300")
    (Traffic_10_40_100_200_400, "10-40-100-200-400")
    (Traffic_40_100_200, "40-100-200")
    (Traffic_100_200, "100-200")
    (Traffic_200, "200")
    (Traffic_100_200_300_400_500, "100-200-300-400-500");

const boost::unordered_map<ResourceAllocOption, std::string>
Options::mapResourAllocOption = boost::assign::map_list_of
    (ResourAllocInvalid, "Invalid")
    (ResourAllocRSA, "RSA")
    (ResourAllocRMSA, "RMSA");

const boost::unordered_map<PhysicalLayerOption, std::string>
Options::mapPhyLayerOption = boost::assign::map_list_of
    (PhyLayerDisabled, "Disabled")
    (PhyLayerEnabled,  "Enabled");

const boost::unordered_map<NetworkOption, std::string>
Options::mapNetworkOption = boost::assign::map_list_of
    (NetworkInvalid, "Invalid")
    (NetworkWDM, "WDM")
    (NetworkEON, "EON");

const boost::unordered_map<RsaOrder,  std::string>
Options::mapOrderRSA = boost::assign::map_list_of
    (OrderRoutingSa, "Routing-SA")
    (OrderSaRouting, "SA-Routing")
    (MixedOrderGA, "Mixed orderGA")
    (MixedOrderHE, "Mixed orderHE")
    (HeuristicsOrder, "Heuristics Ring Order");

const boost::unordered_map<GAOption, std::string>
Options::mapGaOption = boost::assign::map_list_of
    (GaOptionDisabled, "GA Disabled")
    (GaRsaOrder, "GA RSA Order")
    (GaCoreOrder, "GA Core Order")
    (GaNumRoutesCheckMSCL, "GA Number of Interfering Routes Check")
    (GaPDPPBO, "GA PDPP Bit Rate Optimization");

const boost::unordered_map<DevicesOption, std::string>
Options::mapDevicesOption = boost::assign::map_list_of
    (DevicesDisabled, "Disabled")
    (DevicesEnabled, "Enabled");

const boost::unordered_map<TransponderOption, std::string>
Options::mapTransponderOption = boost::assign::map_list_of
    (TransponderDisabled, "Disabled")
    (TransponderEnabled, "Enabled");

const boost::unordered_map<RegenerationOption, std::string>
Options::mapRegenerationOption = boost::assign::map_list_of
    (RegenerationDisabled, "Disabled")
    (RegenerationVirtualized, "Virtualized")
    (RegenerationBackToBack, "Back-to-back");

const boost::unordered_map<RegPlacementOption, std::string>
Options::mapRegPlacOption = boost::assign::map_list_of
    (RegPlacInvalid, "Invalid")
    (RegPlacUniform, "Uniform");

const boost::unordered_map<RegAssignmentOption, std::string>
Options::mapRegAssOption = boost::assign::map_list_of
    (RegAssInvalid, "Invalid")
    (RegAssMinReg, "Minimum regeneration")
    (RegAssMinSlotsMinReg, "Minimum slots (Minimum regeneration)")
    (RegAssMinSlotsMaxReg, "Minimum slots (Maximum regeneration)")
    (RegAssMaxReg, "Maximum regeneration")
    (RegAssFLR, "First Longest Reach (FLR)")
    (RegAssFLRonline, "First Longest Reach Online (FLR)")
    (RegAssFNS, "First Narrowest Spectrum (FNS)")
    (RegAssDRE2BR, "Dynamic Routing in EONs with Back-to-Back Regeneration "
    "(DRE2BR)")
    (RegAssSCRA1, "SCRA: -0.05 + nl/NL + r/R")
    (RegAssSCRA2, "SCRA:  0.00 + nl/NL + r/R")
    (RegAssSCRA3, "SCRA: -1.00 + e*(nl/NL)")
    (RegAssSCRA4, "SCRA:  0.00 + nl/NL - e*(r/R)")
    (RegAssSCRA5, "SCRA:  0.00 + nl/NL + e*(r/R)")
    (RegAssSCRA_PSO, "SCRA PSO")
    (RegAssSCRA_Mod, "SCRA Modified: -0.05 + nl/NL + r/R + f/F")
    (RegAssSCRA_ModPSO, "SCRA Modified PSO");

const boost::unordered_map<StopCriteria, std::string>
Options::mapStopCriteria = boost::assign::map_list_of
    (NumCallRequestsMaximum, "Number of call requests")
    (NumCallRequestsBlocked, "Number of blocked call requests");

const boost::unordered_map<RandomGenerationOption, std::string>
Options::mapRandomGeneration = boost::assign::map_list_of
    (GenerationSame, "Same pseudo-random generation")
    (GenerationPseudoRandom, "Different pseudo-random generation")
    (GenerationRandom, "Random generation");

const boost::unordered_map<ProtectionOption, std::string>
Options::mapProtectionOption = boost::assign::map_list_of
    (ProtectionDisable, "No Protection")
    (ProtectionDPP, "Dedicated Path Protection")
    (ProtectionPDPP, "Partitioning Dedicated Path Protection")
    (ProtectionPDPPBO_GA, "Partitioning Dedicated Path Protection Bit Rate Optimization(GA)")
    (ProtectionPDPPSO_GA, "Partitioning Dedicated Path Protection Squeezing Optimizagion(GA)")
    (ProtectionNPP, "New Path Protection");

const boost::unordered_map<FragMeasureOption, std::string>
Options::mapFragMeasureOption = boost::assign::map_list_of
    (FragMetricDisabled, "Disabled")
    (FragMetricFR, "Fragmentation Ratio (FR)")
    (FragMetricEF, "External Fragmentation (EF)")
    (FragMetricABP, "Access Blocking Probability (ABP)");

std::ostream& operator<<(std::ostream& ostream,
const Options* options) {
    ostream << "OPTIONS" << std::endl;
    ostream << "Topology: " << options->GetTopologyName()
            << std::endl;
    ostream << "Routing algorithm: " << options->GetRoutingName()
            << std::endl;
    ostream << "Spectral allocation algorithm: " << 
            options->GetSpecAllName() << std::endl;
    ostream << "Links cost type: " << options->GetLinkCostTypeName()
            << std::endl;
    ostream << "Traffic requests(Gbps): " << options->GetTrafficName()
            << std::endl;
    ostream << "Resource Allocation: " << options->GetResourAllocName()
            << std::endl;
    ostream << "Physical Layer: " << options->GetPhyLayerName()
            << std::endl;
    ostream << "Network Type: " << options->GetNetworkOptionName()
            << std::endl;
    ostream << "RSA Order: " << options->GetOrderRsaName()
            << std::endl;
    ostream << "GA: " << options->GetGaOptionName()
            << std::endl;
    ostream << "Devices: " << options->GetDevicesOptionName()
            << std::endl;
    ostream << "Transponders: " << options->GetTransponderOptionName()
            << std::endl;
    ostream << "Regeneration: " << options->GetRegenerationOptionName()
            << std::endl;
    
    if(options->GetRegenerationOption() == RegenerationVirtualized){
        ostream << "Regeneration Placement: " << options->GetRegPlacOptionName()
                << std::endl;
        ostream << "Regeneration Assignment: " << options->GetRegAssOptionName()
                << std::endl;
    }
    ostream << "Stop criteria: " << options->GetStopCriteriaName()
            << std::endl;
    ostream << "Random generation: " << options->GetGenerationOptionName()
            << std::endl;
    ostream << "Protection: " << options->GetProtectionOptionName()
            << std::endl;
    ostream << "Fragmentation Option: " << options->GetFragMeasureOptionName()
            << std::endl;
    return ostream;
}

Options::Options(SimulationType* simulType)
:simulType(simulType), topologyOption(TopologyInvalid),
routingOption(RoutingInvalid), specAllOption(SpecAllInvalid),
linkCostType(Invalid), trafficOption(TrafficInvalid), 
resourAllocOption(ResourAllocInvalid), phyLayerOption(PhyLayerDisabled),
networkOption(NetworkInvalid), orderRSA(OrderRoutingSa),
GaOption(GaOptionDisabled), devicesOption(DevicesDisabled),
transponderOption(TransponderDisabled), regenerationOption(RegenerationDisabled), 
regPlacOption(RegPlacInvalid), regAssOption(RegAssInvalid), 
stopCriteria(NumCallRequestsMaximum), generationOption(GenerationSame),
protectionOption(ProtectionDisable), fragMeasureOpion(FragMetricDisabled) {
    
}

Options::~Options() {

}

void Options::Load() {
    int auxInt;
    
    std::cout << "Topology options" << std::endl;
    for(TopologyOption a = FirstTopology; a <= LastTopology; 
    a = TopologyOption(a+1)){
        std::cout << a << "-" << this->mapTopologyOptions.at(a) << std::endl;
    }
    std::cout << "Insert topology: ";
    std::cin >> auxInt;
    this->SetTopologyOption((TopologyOption) auxInt);
    
    std::cout << "Routing algorithm options" << std::endl;
    for(RoutingOption a = FirstRoutingOption; a <= LastRoutingOption; 
    a = RoutingOption(a+1)){
        std::cout << a << "-" << this->mapRoutingOptions.at(a) << std::endl;
    }
    std::cout << "Insert the routing algorithm: ";
    std::cin >> auxInt;
    this->SetRoutingOption((RoutingOption) auxInt);
    
    std::cout << "Spectral allocation options" << std::endl;
    for(SpectrumAllocationOption a = FirstSpecAllOption; 
    a <= LastSpecAllOption; a = SpectrumAllocationOption(a+1)){
        std::cout << a << "-" << this->mapSpecAlgOptions.at(a) << std::endl;
    }
    std::cout << "Insert the spectral allocation: ";
    std::cin >> auxInt;
    this->SetSpecAllOption((SpectrumAllocationOption) auxInt);
    
    std::cout << "Links cost type options" << std::endl;
    for(LinkCostType a = FirstLinkCostType; a <= LastLinkCostType; 
    a = LinkCostType(a+1)){
        std::cout << a << "-" << this->mapLinkCostType.at(a) << std::endl;
    }
    std::cout << "Insert the link cost type: ";
    std::cin >> auxInt;
    this->SetLinkCostType((LinkCostType) auxInt);
    
    std::cout << "Traffic options" << std::endl;
    for(TrafficOption a = FirstTrafficOption; a <= LastTrafficOption; 
    a = TrafficOption(a+1)){
        std::cout << a << "-" << this->mapTrafficOptions.at(a) << std::endl;
    }
    std::cout << "Insert the traffic option: ";
    std::cin >> auxInt;
    this->SetTrafficOption((TrafficOption) auxInt);
    
    std::cout << "Resource allocation options" << std::endl;
    for(ResourceAllocOption a = FirstResourAllocOption; 
    a <= LastResourAllocOption; a = ResourceAllocOption(a+1)){
        std::cout << a << "-" << this->mapResourAllocOption.at(a) << std::endl;
    }
    std::cout << "Insert the resource allocation option: ";
    std::cin >> auxInt;
    this->SetResourAllocOption((ResourceAllocOption) auxInt);
    
    std::cout << "Physical layer options" << std::endl;
    for(PhysicalLayerOption a = PhyLayerDisabled; a <= PhyLayerEnabled;
    a = PhysicalLayerOption(a+1)){
        std::cout << a << "-" << this->mapPhyLayerOption.at(a) << std::endl;
    }
    std::cout << "Insert the physical layer option: ";
    std::cin >> auxInt;
    this->SetPhyLayerOption((PhysicalLayerOption) auxInt);
    
    std::cout << "Network options" << std::endl;
    for(NetworkOption a = FirstNetworkOption; a <= LastNetworkOption;
    a = NetworkOption(a+1)){
        std::cout << a << "-" << this->mapNetworkOption.at(a) << std::endl;
    }
    std::cout << "Insert the network option: ";
    std::cin >> auxInt;
    this->SetNetworkOption((NetworkOption) auxInt);
    
    std::cout << "RSA Order" << std::endl;
    for(RsaOrder a = FirstOrderRSA; a <= LastOrderRSA;
    a = RsaOrder(a+1)){
        std::cout << a << "-" << this->mapOrderRSA.at(a) << std::endl;
    }
    std::cout << "Insert the RSA order: ";
    std::cin >> auxInt;
    this->SetOrderRSA((RsaOrder) auxInt);
    
    std::cout << "GA Option" << std::endl;
    for(GAOption a = FirstGaOption; a <= LastGaOption; a = GAOption(a+1)){
        std::cout << a << "-" << this->mapGaOption.at(a) << std::endl;
    }
    std::cout << "Insert the GA Option: ";
    std::cin >> auxInt;
    this->SetGaOption((GAOption) auxInt);
    
    std::cout << "Devices Option" << std::endl;
    for(DevicesOption a = DevicesDisabled; a <= DevicesEnabled;
    a = DevicesOption(a+1)){
        std::cout << a << "-" << this->mapDevicesOption.at(a)
                  << std::endl;
    }
    std::cout << "Insert the Devices option: ";
    std::cin >> auxInt;
    this->SetDevicesOption((DevicesOption) auxInt);
    
    std::cout << "Transponder Option" << std::endl;
    for(TransponderOption a = TransponderDisabled; a <= TransponderEnabled;
    a = TransponderOption(a+1)){
        std::cout << a << "-" << mapTransponderOption.at(a)
                  << std::endl;
    }
    std::cout << "Insert the Transponder option: ";
    std::cin >> auxInt;
    this->SetTransponderOption((TransponderOption) auxInt);
    
    std::cout << "Regeneration Option" << std::endl;
    for(RegenerationOption a = RegenerationDisabled; a <= RegenerationBackToBack;
    a = RegenerationOption(a+1)){
        std::cout << a << "-" << this->mapRegenerationOption.at(a) << std::endl;
    }
    std::cout << "Insert the Regeneration option: ";
    std::cin >> auxInt;
    this->SetRegenerationOption((RegenerationOption) auxInt);
    
    std::cout << "Regeneration Placement Option" << std::endl;
    for(RegPlacementOption a = FirstRegPlac; a <= LastRegPlac;
    a = RegPlacementOption(a+1)){
        std::cout << a << "-" << this->mapRegPlacOption.at(a) << std::endl;
    }
    std::cout << "Insert the Regeneration Placement option: ";
    std::cin >> auxInt;
    this->SetRegPlacOption((RegPlacementOption) auxInt);
    
    std::cout << "Regeneration Assignment Option" << std::endl;
    for(RegAssignmentOption a = FirstRegAss; a <= LastRegAss;
    a = RegAssignmentOption(a+1)){
        std::cout << a << "-" << this->mapRegAssOption.at(a) << std::endl;
    }
    std::cout << "Insert the Regeneration Assignment option: ";
    std::cin >> auxInt;
    this->SetRegAssOption((RegAssignmentOption) auxInt);
    
    std::cout << "Stop Criteria Options" << std::endl;
    for(StopCriteria a = FirstStopCriteria; a <= LastStopCriteria;
    a = StopCriteria(a+1)){
        std::cout << a << "-" << this->mapStopCriteria.at(a) << std::endl;
    }
    std::cout << "Insert the Stop Criteria option: ";
    std::cin >> auxInt;
    this->SetStopCriteria((StopCriteria) auxInt);
    
    std::cout << "Random Generation Options" << std::endl;
    for(RandomGenerationOption a = FirstGeneration; a <= LastGeneration;
    a = RandomGenerationOption(a+1)){
        std::cout << a << "-" << this->mapRandomGeneration.at(a) << std::endl;
    }
    std::cout << "Insert the Random Generation option: ";
    std::cin >> auxInt;
    this->SetGenerationOption((RandomGenerationOption) auxInt);
    
    std::cout << std::endl;
}

void Options::LoadFile() {
    std::ifstream auxIfstream;
    int auxInt;
    
    this->simulType->GetInputOutput()->LoadOptions(auxIfstream);
    auxIfstream >> auxInt;
    this->SetTopologyOption((TopologyOption) auxInt);
    auxIfstream >> auxInt;
    this->SetRoutingOption((RoutingOption) auxInt);
    auxIfstream >> auxInt;
    this->SetSpecAllOption((SpectrumAllocationOption) auxInt);
    auxIfstream >> auxInt;
    this->SetLinkCostType((LinkCostType) auxInt);
    auxIfstream >> auxInt;
    this->SetTrafficOption((TrafficOption) auxInt);
    auxIfstream >> auxInt;
    this->SetResourAllocOption((ResourceAllocOption) auxInt);
    auxIfstream >> auxInt;
    this->SetPhyLayerOption((PhysicalLayerOption) auxInt);
    auxIfstream >> auxInt;
    this->SetNetworkOption((NetworkOption) auxInt);
    auxIfstream >> auxInt;
    this->SetOrderRSA((RsaOrder) auxInt);
    auxIfstream >> auxInt;
    this->SetGaOption((GAOption) auxInt);
    auxIfstream >> auxInt;
    this->SetDevicesOption((DevicesOption) auxInt);
    auxIfstream >> auxInt;
    this->SetTransponderOption((TransponderOption) auxInt);
    auxIfstream >> auxInt;
    this->SetRegenerationOption((RegenerationOption) auxInt);
    auxIfstream >> auxInt;
    this->SetRegPlacOption((RegPlacementOption) auxInt);
    auxIfstream >> auxInt;
    this->SetRegAssOption((RegAssignmentOption) auxInt);
    auxIfstream >> auxInt;
    this->SetStopCriteria((StopCriteria) auxInt);
    auxIfstream >> auxInt;
    this->SetGenerationOption((RandomGenerationOption) auxInt);
    auxIfstream >> auxInt;
    this->SetProtectionOption((ProtectionOption) auxInt);
    auxIfstream >> auxInt;
    this->SetFragMeasureOption((FragMeasureOption) auxInt);
}

void Options::Save() {
    std::ofstream& auxOfstream = this->simulType->GetInputOutput()
                                     ->GetLogFile();
    auxOfstream << this << std::endl;
}

bool Options::IsGA_SO() const {
    
    switch(this->GetGaOption()){
        case GaRsaOrder:
        case GaCoreOrder:
            return true;
        default:
            return false;
    }
}

bool Options::IsGA_MO() const {
    
    switch(this->GetGaOption()){
        case GaPDPPBO:
        case GaNumRoutesCheckMSCL:
            return true;
        default:
            return false;
    }
}

TopologyOption Options::GetTopologyOption() const {
    return topologyOption;
}

std::string Options::GetTopologyName() const {
    return mapTopologyOptions.at(this->topologyOption);
}

void Options::SetTopologyOption(TopologyOption topologyOption) {
    assert(topologyOption >= FirstTopology && 
           topologyOption <= LastTopology);
    this->topologyOption = topologyOption;
}

RoutingOption Options::GetRoutingOption() const {
    return routingOption;
}

std::string Options::GetRoutingName() const {
    return mapRoutingOptions.at(this->routingOption);
}

void Options::SetRoutingOption(RoutingOption routingOption) {
    assert(routingOption >= FirstRoutingOption && 
           routingOption <= LastRoutingOption);
    this->routingOption = routingOption;
}

SpectrumAllocationOption Options::GetSpecAllOption() const {
    return specAllOption;
}

std::string Options::GetSpecAllName() const {
    return mapSpecAlgOptions.at(this->specAllOption);
}

void Options::SetSpecAllOption(SpectrumAllocationOption specAllOption) {
    assert(specAllOption >= FirstSpecAllOption &&
           specAllOption <= LastSpecAllOption);
    this->specAllOption = specAllOption;
}

LinkCostType Options::GetLinkCostType() const {
    return linkCostType;
}

std::string Options::GetLinkCostTypeName() const {
    return mapLinkCostType.at(this->linkCostType);
}

void Options::SetLinkCostType(LinkCostType linkCostType) {
    assert(linkCostType >= FirstLinkCostType &&
           linkCostType <= LastLinkCostType);
    this->linkCostType = linkCostType;
}

TrafficOption Options::GetTrafficOption() const {
    return trafficOption;
}

std::string Options::GetTrafficName() const {
    return mapTrafficOptions.at(this->trafficOption);
}

void Options::SetTrafficOption(TrafficOption trafficOption) {
    assert(trafficOption >= FirstTrafficOption &&
           trafficOption <= LastTrafficOption);
    this->trafficOption = trafficOption;
}

ResourceAllocOption Options::GetResourAllocOption() const {
    return resourAllocOption;
}

std::string Options::GetResourAllocName() const {
    return this->mapResourAllocOption.at(this->resourAllocOption);
}

void Options::SetResourAllocOption(ResourceAllocOption resourAllocOption) {
    assert(resourAllocOption >= FirstResourAllocOption &&
           resourAllocOption <= LastResourAllocOption);
    this->resourAllocOption = resourAllocOption;
}

PhysicalLayerOption Options::GetPhyLayerOption() const {
    return phyLayerOption;
}

std::string Options::GetPhyLayerName() const {
    return this->mapPhyLayerOption.at(this->phyLayerOption);
}

void Options::SetPhyLayerOption(PhysicalLayerOption phyLayerOption) {
    this->phyLayerOption = phyLayerOption;
}

NetworkOption Options::GetNetworkOption() const {
    return networkOption;
}

std::string Options::GetNetworkOptionName() const {
    return this->mapNetworkOption.at(this->networkOption);
}

void Options::SetNetworkOption(NetworkOption networkOption) {
    assert(networkOption >= FirstNetworkOption &&
           networkOption <= LastNetworkOption);
    this->networkOption = networkOption;
    
    switch(this->networkOption){
        case NetworkWDM:
            this->simulType->GetParameters()->SetSlotBandwidth(50.0);
            break;
        case NetworkEON:
            this->simulType->GetParameters()->SetSlotBandwidth(12.5);
            break;
        default:
            std::cout << "Invalid type of network" << std::endl;
    }
}

RsaOrder Options::GetOrderRSA() const {
    return orderRSA;
}

std::string Options::GetOrderRsaName() const {
    return this->mapOrderRSA.at(this->orderRSA);
}

void Options::SetOrderRSA(RsaOrder orderRSA) {
    this->orderRSA = orderRSA;
}

GAOption Options::GetGaOption() const {
    return GaOption;
}

std::string Options::GetGaOptionName() const {
    return this->mapGaOption.at(this->GaOption);
}

void Options::SetGaOption(GAOption GaOption) {
    this->GaOption = GaOption;
}

DevicesOption Options::GetDevicesOption() const {
    return devicesOption;
}

std::string Options::GetDevicesOptionName() const {
    return this->mapDevicesOption.at(this->devicesOption);
}

void Options::SetDevicesOption(DevicesOption devicesOption) {
    this->devicesOption = devicesOption;
}

TransponderOption Options::GetTransponderOption() const {
    return transponderOption;
}

std::string Options::GetTransponderOptionName() const {
    return mapTransponderOption.at(this->transponderOption);
}

void Options::SetTransponderOption(TransponderOption transponderOption) {
    this->transponderOption = transponderOption;
}

RegenerationOption Options::GetRegenerationOption() const {
    return regenerationOption;
}

std::string Options::GetRegenerationOptionName() const {
    return mapRegenerationOption.at(this->regenerationOption);
}

void Options::SetRegenerationOption(RegenerationOption regenerationOption) {
    this->regenerationOption = regenerationOption;
}

RegPlacementOption Options::GetRegPlacOption() const {
    return regPlacOption;
}

std::string Options::GetRegPlacOptionName() const {
    return mapRegPlacOption.at(this->regPlacOption);
}

void Options::SetRegPlacOption(RegPlacementOption regPlacOption) {
    assert(regPlacOption >= FirstRegPlac && regPlacOption <= LastRegPlac);
    this->regPlacOption = regPlacOption;
}

RegAssignmentOption Options::GetRegAssOption() const {
    return regAssOption;
}

std::string Options::GetRegAssOptionName() const {
    return mapRegAssOption.at(this->regAssOption);
}

void Options::SetRegAssOption(RegAssignmentOption regAssOption) {
    assert(regAssOption >= FirstRegAss && regAssOption <= LastRegAss);
    this->regAssOption = regAssOption;
}

StopCriteria Options::GetStopCriteria() const {
    return stopCriteria;
}

std::string Options::GetStopCriteriaName() const {
    return mapStopCriteria.at(this->stopCriteria);
}

void Options::SetStopCriteria(StopCriteria stopCriteria) {
    assert(stopCriteria >= FirstStopCriteria && 
           stopCriteria <= LastStopCriteria);
    this->stopCriteria = stopCriteria;
}

RandomGenerationOption Options::GetGenerationOption() const {
    return generationOption;
}

std::string Options::GetGenerationOptionName() const {
    return mapRandomGeneration.at(generationOption);
}

void Options::SetGenerationOption(RandomGenerationOption generationOption) {
    assert(generationOption >= FirstGeneration && 
    generationOption  <= LastGeneration);
    this->generationOption = generationOption;
}

ProtectionOption Options::GetProtectionOption() const {
    return protectionOption;
}

std::string Options::GetProtectionOptionName() const {
    return mapProtectionOption.at(protectionOption);
}

void Options::SetProtectionOption(ProtectionOption protectionOption) {
    assert(protectionOption >= FirstProtectionOption && 
           protectionOption <= LastProtectionOption);
    
    this->protectionOption = protectionOption;
}

FragMeasureOption Options::GetFragMeasureOption() const {
    return fragMeasureOpion;
}

std::string Options::GetFragMeasureOptionName() const {
    return mapFragMeasureOption.at(fragMeasureOpion);
}

void Options::SetFragMeasureOption(FragMeasureOption fragMeasureOption) {
    fragMeasureOpion = fragMeasureOption;
}
