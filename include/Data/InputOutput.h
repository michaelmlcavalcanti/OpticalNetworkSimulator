/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InputOutput.h
 * Author: bruno
 *
 * Created on August 8, 2018, 8:14 PM
 */

#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include <fstream>

class SimulationType;

/**
 * @brief The InputOutput class is responsible for ready/write
 * in simulation files.
 */
class InputOutput {
public:
    /**
     * @brief Standard constructor for a InputOutput object.
     * @param simulType SimulationType object that owns 
     * this InputOutput.
     */
    InputOutput(SimulationType* simulType);
    /**
     * @brief Virtual destructor of a InputOutput object.
     */
    virtual ~InputOutput();
    
    /**
     * @brief Function responsible for access the Parameters file.
     * @param parameters stream that operates in Parameters file.
     */
    void LoadParameters(std::ifstream& parameters);
    /**
     * @brief Function responsible for access the Options file.
     * @param options stream that operates in Options file.
     */
    void LoadOptions(std::ifstream& options);
    /**
     * @brief Function responsible for access the Topology file.
     * @param topology stream that operates in Topology file.
     */
    void LoadTopology(std::ifstream& topology);
    /**
     * @brief Function responsible for access the Traffic file.
     * @param traffic stream that operates in Traffic file.
     */
    void LoadTraffic(std::ifstream& traffic);
    /**
     * @brief Function to load the basic GA parameters file.
     * @param gaParam Input file.
     */
    void LoadGA(std::ifstream& gaParam);
    /**
     * @brief Function to load the GA single objective parameters file.
     * @param gaSoParam Input file.
     */
    void LoadGA_SO(std::ifstream& gaSoParam);
    /**
     * @brief Function to load the GA multi-objective parameters file.
     * @param gaMoParam Input file.
     */
    void LoadGA_MO(std::ifstream& gaMoParam);
    /**
     * @brief Loads the RSA order input file from the first simulation made.
     * @param orderRsa Ifstream with the input file.
     */
    void LoadRsaOrderFirstSimul(std::ifstream& orderRsa);
    /**
    * @brief Loads the PDPP bit rate distribution for all pair S-D input file from
    * the first simulation made.
    * @param bitRateDist Ifstream with the input file.
    */
    void LoadPDPPBitRateNodePairsDistFirstSimul(std::ifstream& bitRateDist);
    /**
     * @brief Function to load the devices file, with fixed number of each one.
     * @param devicesFile Input devices file.
     */
    void LoadDevicesFile(std::ifstream& devicesFile);
    
    void LoadPSO(std::ifstream& psoFile);
    
    void LoadCoefficientsSCRA(std::ifstream& coeSCRA);
    
    /**
     * @brief Get the Log.txt ofstream.
     * @return ofstream containing the log file.
     */
    std::ofstream& GetLogFile();
    /**
     * @brief Get the PBvLoad.txt ofstream.
     * @return ofstream containing the PBvLoad file.
     */
    std::ofstream& GetReqBpFile();
    /**
     * @brief Function to get the file that storage the bandwidth blocking
     * probabilities.
     * @return Bandwidth blocking probability file.
     */
    std::ofstream& GetBandBpFile();
    /**
     * @brief Function to get the file that storage the Network Utilization 
     * File.
     * @return Network Utilization file.
     */
    std::ofstream& GetNetUtilizFile();
    /**
     * @brief Function to get the file that storage the Network Fragmentation.
     * @return Network Fragmentation file.
     */
    std::ofstream& GetNetFragFile();
      /**
     * @brief Function to get the file that storage the Accumulated Network 
     * Fragmentation.
     * @return Accumulated Network Fragmentation file.
     */
    std::ofstream& GetAccumNetFragFile();
    
    std::ofstream& GetFragBandFile();
    
    std::ofstream& GetLinksUse();
    /**
     * @brief Get the output file that will contain the Slots Relative Usage 
     * of the simulation.
     * @return Slots Relative Slots output file.
     */
    std::ofstream& GetSlotsRelativeUse();
    /**
     * @brief Function to get the file that storage the call Protection Rate.
     * @return Network call protection Rate file.
     */
    std::ofstream& GetNetProtRate();
    /**
     * @brief Function to get the file that storage the call non Protection Rate.
     * @return Network call non protection Rate file.
     */
    std::ofstream& GetNetNonProtRate();
    /**
     * @brief Function to get the file that storage the network beta average.
     * @return Network call beta average file.
     */
    std::ofstream& GetNetBetaAverage();
    /**
    * @brief Function to get the file that storage the network alpha average.
    * @return Network call alpha average file.
    */
    std::ofstream& GetNetAlphaAverage();
    /**
    * @brief Function to get the file that storage the number of Hops per Route list.
    * @return number of Hops per Route list file.
    */
    std::ofstream& GetNumHopsRoutes();
    /**
     * @brief Get the output file that will contain the best individuals, with
     * their correspondent generation and blocking probability.
     * @return Best individuals output file.
     */
    std::ofstream& GetBestIndividualsFile();
    /**
     * @brief Get the output file that will contain the best individual of the 
     * last generation (Genes of this individual).
     * @return Best individual output file.
     */
    std::ofstream& GetBestIndividualFile();
    /**
     * @brief Get the output file that will contain the worst individuals, with
     * their correspondent generation and blocking probability.
     * @return Worst individuals output file.
     */
    std::ofstream& GetWorstIndividualsFile();
    /**
     * @brief Get the output file that will contain the initial population, 
     * before the first generation, with their blocking probability.
     * @return Initial population output file.
     */
    std::ofstream& GetIniPopulationFile();
    
    std::ofstream& GetBestParticle();

    std::ofstream& GetBestParticles();

    
    std::ofstream& LoadTable();
    
    /**
     * @brief Function to print the progress bar, based in 
     * the inputs proportion.
     * @param actual Progress actual value.
     * @param max Progress total value.
     */
    void PrintProgressBar(unsigned int actual, unsigned int max);
    /**
     * @brief Function to print the progress bar, based in the inputs 
     * proportion.
     * @param actual Progress actual value.
     * @param min Progress minimum value.
     * @param max Progress maximum value.
     */
    void PrintProgressBar(unsigned actual, unsigned min, unsigned max);
private:
    /**
     * @brief Function to load the .txt file to output the blocking probability
     * as function of the network load.
     * @param results ofstream to the PBvLoad.txt.
     */
    void LoadReqBP(std::ofstream& pBvLoad);
    /**
     * @brief Function to load the bandwidth blocking probability file.
     * @param bandBP Bandwidth blocking probability file.
     */
    void LoadBandBP(std::ofstream& bandBP);
    /**
     * @brief Function to load the Network Utilization file.
     * @param NetUtiliz Network Utilization
     */
    void LoadNetUtiliz(std::ofstream& NetUtiliz);
    /**
    * @brief Function to load the Network Fragmentation file.
    * @param NetFrag
    */
    void LoadNetFrag(std::ofstream& NetFrag);
    
     /**
    * @brief Function to load the Accumulated Network Fragmentation file.
    * @param AccumNetFrag
    */
    void LoadAccumNetFrag(std::ofstream& AccumNetFrag);
    
    /**
     * @brief Function to load the .txt file to output the simulation log.
     * @param results ofstream to the Log.txt.
     */
    void LoadLog(std::ofstream& log);
    
    void LoadLinksUse(std::ofstream& linksUse);
    /**
    * @brief Function to load the Slots Relative Usage file.
    * @param slotsRelativeUse 
    */
    void LoadSlotsRelativeUse(std::ofstream& slotsRelativeUse);
    
    void LoadFragBand(std::ofstream& fragBand);
    /**
    * @brief Function to load the call protection Rate file.
    * @param netProtRate 
    */
    void LoadNetProtRate(std::ofstream& netProtRate);
    /**
    * @brief Function to load the call non protection Rate file.
    * @param netNonProtRate 
    */
    void LoadNetNonProtRate(std::ofstream& netNonProtRate);
    /**
    * @brief Function to load the network beta average file.
    * @param netBetaAverage
    */
    void LoadNetBetaAverage(std::ofstream& netBetaAverage);
    /**
    * @brief Function to load the network alpha average file.
    * @param netAlphaAverage
    */
    void LoadNetAlphaAverage(std::ofstream& netAlphaAverage);
    /**
    * @brief Function to load the number of Hops per Route list.
    * @param numHopsRoute
    */
    void LoadNumHopsRoutes(std::ofstream& numHopsRoutes);

    /**
     * @brief Function to load the GA algorithm files.
     * @param bests Best individuals file.
     * @param best Best individual file.
     * @param worst Worst individuals file.
     * @param iniPop Initial population file.
     */
    void LoadGaFiles(std::ofstream& bests, std::ofstream& best, 
                     std::ofstream& worst, std::ofstream& iniPop);
    
    void LoadPsoFiles(std::ofstream& best, std::ofstream& bests);
private:
    /**
     * @brief Pointer to a SimulationType object that owns this object
     */
    SimulationType* simulType;
    /**
     * @brief Ofstream with the Log.txt file.
     */
    std::ofstream logFile;
    /**
     * @brief Request blocking probability output file.
     */
    std::ofstream reqBpFile;
    /**
     * @brief Bandwidth blocking probability output file.
     */
    std::ofstream bandBpFile;
    /**
     * @brief Network Utilization output file.
     */
    std::ofstream netUtilizFile;
    /**
     * @brief Network Fragmentation output file.
     */
    std::ofstream netFragFile;
    
    /**
     * @brief Accumulated Network Fragmentation output file.
     */
    std::ofstream accumNetFragFile;
    
    std::ofstream fragBand;
    
    std::ofstream linksUse;
    /**
     * @brief Slots Relative Usage output file.
     */
    std::ofstream slotsRelativeUse;
    /**
     * @brief call protection rate output file.
     */
    std::ofstream netProtRate;
    /**
     * @brief call non protection rate output file.
     */
    std::ofstream netNonProtRate;
    /**
     * @brief network beta average output file.
     */
    std::ofstream netBetaAverage;
    /**
    * @brief network alpha average output file.
    */
    std::ofstream netAlphaAverage;
    /**
    * @brief number of hops per route list output file.
    */
    std::ofstream numHopsRoutes;
    /**
     * @brief Output file to save the best individuals for a GA single 
     * objective.
     */
    std::ofstream bestIndividuals;
    /**
     * @brief Output file to save the best individual for a GA single 
     * objective.
     */
    std::ofstream bestIndividual;
    /**
     * @brief Output file to save the worst individuals for a GA single 
     * objective.
     */
    std::ofstream worstIndividuals;
    /**
     * @brief Output file to save the initial population of a GA.
     */
    std::ofstream initialPopulation;
    
    std::ofstream bestParticle;
    
    std::ofstream bestParticles;
    
    std::ofstream table;
    /**
     * @brief Size of the progress bar.
     */
    static const int barWidth = 30;
};

#endif /* INPUTOUTPUT_H */

