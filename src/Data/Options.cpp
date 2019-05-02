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
    (TopologyItaly, "Italy");

const boost::unordered_map<RoutingOption, std::string>
Options::mapRoutingOptions = boost::assign::map_list_of
    (RoutingInvalid, "Invalid")
    (RoutingDJK, "Dijkstra")
    (RoutingYEN, "YEN")
    (RoutingBSR, "BSR");

const boost::unordered_map<SpectrumAllocationOption, std::string>
Options::mapSpecAlgOptions = boost::assign::map_list_of
    (SpecAllInvalid, "Invalid")
    (SpecAllRandom, "Random")
    (SpecAllFF, "First Fit")
    (SpecAllMSCL, "MSCL");

const boost::unordered_map<LinkCostType, std::string>
Options::mapLinkCostType = boost::assign::map_list_of
    (Invalid, "Invalid cost type")
    (MinHops, "Minimum hops")
    (MinLength, "Minimum length")
    (MinLengthNormalized, "Minimum length normalized");

const boost::unordered_map<TrafficOption, std::string>
Options::mapTrafficOptions = boost::assign::map_list_of
    (TrafficInvalid, "Invalid")
    (Traficc_100_200_400, "100-200-400")
    (Traficc_50_120_300, "50-120-300")
    (Traffic_10_40_100_200_400, "10-40-100-200-400");

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
    (MixedOrder, "Mixed order");

const boost::unordered_map<GAOption, std::string>
Options::mapGaOption = boost::assign::map_list_of
    (GaOptionDisabled, "GA Disabled")
    (GaRsaOrder, "GA RSA Order")
    (GaCoreOrder, "GA Core Order")
    (GaNumRoutesCheckMSCL, "GA Number of Interfering Routes Check");

const boost::unordered_map<DevicesOption, std::string>
Options::mapDevicesOption = boost::assign::map_list_of
    (DevicesDisabled, "Disabled")
    (DevicesEnabled, "Enabled");

const boost::unordered_map<RegenerationOption, std::string>
Options::mapRegenerationOption = boost::assign::map_list_of
    (RegenerationDisabled, "Disabled")
    (RegenerationUniformDist, "Enabled");

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
    ostream << "Regeneration: " << options->GetRegenerationOptionName()
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
regenerationOption(RegenerationDisabled) {
    
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
        std::cout << a << "-" << this->mapDevicesOption.at(this->devicesOption)
                  << std::endl;
    }
    std::cout << "Insert the Devices option: ";
    std::cin >> auxInt;
    this->SetDevicesOption((DevicesOption) auxInt);
    
    std::cout << "Regeneration Option" << std::endl;
    for(RegenerationOption a = RegenerationDisabled; a <= RegenerationUniformDist;
    a = RegenerationOption(a+1)){
        std::cout << a << "-" << this->mapRegenerationOption.at(a) << std::endl;
    }
    std::cout << "Insert the Regeneration option: ";
    std::cin >> auxInt;
    this->SetRegenerationOption((RegenerationOption) auxInt);
    
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
    this->SetRegenerationOption((RegenerationOption) auxInt);
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

RegenerationOption Options::GetRegenerationOption() const {
    return regenerationOption;
}

std::string Options::GetRegenerationOptionName() const {
    return mapRegenerationOption.at(this->regenerationOption);
}

void Options::SetRegenerationOption(RegenerationOption regenerationOption) {
    this->regenerationOption = regenerationOption;
}
