/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Options.h
 * Author: bruno
 *
 * Created on August 3, 2018, 1:18 AM
 */

#ifndef OPTIONS_H
#define OPTIONS_H

class SimulationType;

#include <fstream>
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>
#include <iostream>
#include <assert.h>

/**
 *@brief Numerate the topology options
 */
enum TopologyOption {
    TopologyInvalid,
    TopologyNSFNet,
    TopologyRing,
    TopologyToroidal,
    TopologyGermany,
    TopologyItaly,
    FirstTopology = TopologyNSFNet,
    LastTopology = TopologyItaly
};

/**
 * @brief Numerate the options for routing algorithms
 */
enum RoutingOption {
     RoutingInvalid,
     RoutingDJK,
     RoutingYEN,
     RoutingBSR,
     FirstRoutingOption = RoutingDJK,
     LastRoutingOption = RoutingBSR
};

/**
 * @brief Numerate the options for spectrum allocation algorithms
 */
enum SpectrumAllocationOption {
     SpecAllInvalid,
     SpecAllRandom,
     SpecAllFF,
     SpecAllMSCL,
     FirstSpecAllOption = SpecAllRandom,
     LastSpecAllOption = SpecAllMSCL
};

/**
 * @brief Numerate the options for links cost
 */
enum LinkCostType {
    Invalid,
    MinHops,
    MinLength,
    MinLengthNormalized,
    FirstLinkCostType = MinHops,
    LastLinkCostType = MinLengthNormalized
};

/**
 * @brief Numerate the options for traffic bit rate
 */
enum TrafficOption {
    TrafficInvalid,
    Traficc_100_200_400,
    Traficc_50_120_300,
    Traffic_10_40_100_200_400,
    FirstTrafficOption = Traficc_100_200_400,
    LastTrafficOption = Traffic_10_40_100_200_400
};

/**
 * @brief Enumerate the resource allocation option. 
 * The options are RSA or RMSA.
 */
enum ResourceAllocOption {
    ResourAllocInvalid,
    ResourAllocRSA,
    ResourAllocRMSA,
    FirstResourAllocOption = ResourAllocRSA,
    LastResourAllocOption = ResourAllocRMSA
};

/**
 * @brief Option that enable/disable the network physical layer option.
 */
enum PhysicalLayerOption {
    PhyLayerDisabled,
    PhyLayerEnabled
};

/**
 * @brief Numerate the network options, WDM or EON.
 */
enum NetworkOption {
    NetworkInvalid,
    NetworkWDM,
    NetworkEON,
    FirstNetworkOption = NetworkWDM,
    LastNetworkOption = NetworkEON
};

/**
 * @brief Numerate the resource allocation choice order, R-SA, SA-R or GA found 
 * order.
 */
enum RsaOrder {
    OrderRoutingSa,
    OrderSaRouting,
    MixedOrder,
    FirstOrderRSA = OrderRoutingSa,
    LastOrderRSA = MixedOrder
};

/**
 * @brief Numerate the Genetic Algorithm options.
 */
enum GAOption{
    GAOptionDisabled,
    GaRsaOrder,
    GaCoreOrder,
    GaNumRoutesCheckMSCL,
    FirstGaOption = GaRsaOrder,
    LastGaOption = GaNumRoutesCheckMSCL
};


enum TransponderOption {
    TransOptionDisabled,
    TransOptionEnabled
};

/**
 * @brief The Options class is responsible for storage the simulation
 * options.
 */
class Options {
    
    friend std::ostream& operator<<(std::ostream& ostream,
    const Options* options);
    
public:
    /**
     * @brief Standard constructor for a Options object.
     * @param simulType SimulationType object that owns 
     * this Options.
     */
    Options(SimulationType* simulType);
    /**
     * @brief Copy constructor for a Options object.
     * @param orig original Options object.
     */
    Options(const Options& orig);
    /**
     * @brief Virtual destructor of a Options object.
     */
    virtual ~Options();
    
    /**
     * @brief Load the options from terminal inputs
     */
    void Load();
    /**
     * @brief Load the options from a .txt file.
     */
    void LoadFile();
    /**
     * @brief Saves the options selected in a .txt file.
     */
    void Save();
    
    /**
     * @brief Returns the topology choice.
     * @return Topology option.
     */
    TopologyOption GetTopologyOption() const;
    /**
     * @brief Gets the name of topology based in option selected.
     * @return The name of the topology.
     */
    std::string GetTopologyName() const;
    /**
     * @brief Sets the topology choice.
     * @param topologyOption topology option.
     */
    void SetTopologyOption(TopologyOption topologyOption);
    /**
     * @brief Returns the routing option.
     * @return Routing option.
     */
    RoutingOption GetRoutingOption() const;
    /**
     * @brief Gets the name of routing algorithm based in 
     * option selected.
     * @return The name of the routing algorithm.
     */
    std::string GetRoutingName() const;
    /**
     * @brief Sets the routing option.
     * @param routingOption routing option.
     */
    void SetRoutingOption(RoutingOption routingOption);
    /**
     * @brief Returns the spectral allocation option.
     * @return Spectral allocation option.
     */
    SpectrumAllocationOption GetSpecAllOption() const;
    /**
     * @brief Gets the name of spectral allocation algorithm 
     * based in option selected.
     * @return The name of the spectral allocation algorithm.
     */
    std::string GetSpecAllName() const;
    /**
     * @brief Sets the spectral allocation option
     * @param specAllOption spectral allocation option
     */
    void SetSpecAllOption(SpectrumAllocationOption specAllOption);
    /**
     * @brief Returns the type of links cost option.
     * @return Link cost option.
     */
    LinkCostType GetLinkCostType() const;
    /**
     * @brief Gets the name of links cost based in 
     * option selected.
     * @return The name of the links cost.
     */
    std::string GetLinkCostTypeName() const;
    /**
     * @brief Sets the links cost option.
     * @param linkCostType links cost option.
     */
    void SetLinkCostType(LinkCostType linkCostType);
    /**
     * @brief Get the selected traffic option.
     * @return Traffic option.
     */
    TrafficOption GetTrafficOption() const;
    /**
     * @brief Get the traffic option name.
     * @return String with selected traffic option name.
     */
    std::string GetTrafficName() const;
    /**
     * @brief Set the traffic option.
     * @param trafficOption traffic option.
     */
    void SetTrafficOption(TrafficOption trafficOption);
    /**
     * @brief Gets the resource allocation option selected.
     * @return Resource allocation option.
     */
    ResourceAllocOption GetResourAllocOption() const;
    /**
     * @brief Gets the name of the resource allocation option selected.
     * @return String with the name of the resource allocation option.
     */
    std::string GetResourAllocName() const;
    /**
     * @brief Set the resource allocation option.
     * @param resourAllocOption Resource allocation option.
     */
    void SetResourAllocOption(ResourceAllocOption resourAllocOption);
    /**
     * @brief Gets the physical layer option selected.
     * @return Physical layer option.
     */
    PhysicalLayerOption GetPhyLayerOption() const;
    /**
     * @brief Gets the name of the physical layer option selected.
     * @return String with the name of the physical layer option.
     */
    std::string GetPhyLayerName() const; 
    /**
     * @brief Set the physical layer option.
     * @param phyLayerOption Physical layer option.
     */
    void SetPhyLayerOption(PhysicalLayerOption phyLayerOption);
    
    NetworkOption GetNetworkOption() const;
    
    std::string GetNetworkOptionName() const;

    void SetNetworkOption(NetworkOption networkOption);
    
    RsaOrder GetOrderRSA() const;
    
    std::string GetOrderRsaName() const;

    void SetOrderRSA(RsaOrder orderRSA);
    
    void SetTransponderOption(TransponderOption transponderOption);
    
    GAOption GetGAOption() const;
    
    std::string GetGAOptionName() const;

    void SetGAOption(GAOption coreOrder);
private:
    /**
     * @brief A pointer to the simulation this object belong.
     */
    SimulationType* simulType;
    /**
     * @brief Topology option.
     */
    TopologyOption topologyOption;
    /**
     * @brief Routing option.
     */
    RoutingOption routingOption;
    /**
     * @brief Spectral allocation option.
     */
    SpectrumAllocationOption specAllOption;
    /**
     * @brief Link cost option selected.
     */
    LinkCostType linkCostType;
    /**
     * @brief Traffic option selected.
     */
    TrafficOption trafficOption;
    /**
     * @brief Resource allocation options selected.
     */
    ResourceAllocOption resourAllocOption;
    /**
     * @brief Physical layer option selected.
     */
    PhysicalLayerOption phyLayerOption;
    /**
     * @brief Network option selected. WDM or EON.
     */
    NetworkOption networkOption;
    /**
     * @brief Resource allocation order option.
     */
    RsaOrder orderRSA;
    /**
     * @brief Genetic algorithm option.
     */
    GAOption GaOption;
    
    TransponderOption transponderOption;
    
    /**
     * @brief Map that keeps the topology option 
     * and the name of the selected topology.
     */
    static const boost::unordered_map<TopologyOption, 
    std::string> mapTopologyOptions;
    /**
     * @brief Map that keeps the routing algorithm option 
     * and the name of the selected routing algorithm.
     */
    static const boost::unordered_map<RoutingOption,
    std::string> mapRoutingOptions;
    /**
     * @brief Map that keeps the spectral allocation algorithm 
     * option and the name of the selected spectral 
     * allocation algorithm.
     */
    static const boost::unordered_map<SpectrumAllocationOption,
    std::string> mapSpecAlgOptions;
    /**
     * @brief Map that keeps the links cost option 
     * and the name of the selected links cost.
     */
    static const boost::unordered_map<LinkCostType, 
    std::string> mapLinkCostType;
    /**
     * @brief Map that keeps the traffic options 
     * and the name of each traffic.
     */
    static const boost::unordered_map<TrafficOption,
    std::string> mapTrafficOptions;
    /**
     * @brief Map that keeps the resource allocation options 
     * and the name of each one.
     */
    static const boost::unordered_map<ResourceAllocOption,
    std::string> mapResourAllocOption;
    /**
     * @brief Map that keeps the physical layer options 
     * and the name of each one (Enabled and Disabled).
     */
    static const boost::unordered_map<PhysicalLayerOption,
    std::string> mapPhyLayerOption;
    /**
     * @brief Map that keeps the network options and the
     * name of each one.
     */
    static const boost::unordered_map<NetworkOption,
    std::string> mapNetworkOption;
    /**
     * @brief Map that keeps the resource allocation order
     * and the name of each one.
     */
    static const boost::unordered_map<RsaOrder,
    std::string> mapOrderRSA;
    /**
     * @brief Map that keeps the GA options 
     * and the name of each one (Enabled and Disabled).
     */
    static const boost::unordered_map<GAOption,
    std::string> mapGaOption;
    
    static const boost::unordered_map<TransponderOption,
    std::string> mapTransponderOption;
};

#endif /* OPTIONS_H */

