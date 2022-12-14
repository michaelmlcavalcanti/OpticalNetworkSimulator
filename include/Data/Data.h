/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Data.h
 * Author: bruno
 *
 * Created on August 8, 2018, 6:25 PM
 */

#ifndef DATA_H
#define DATA_H

class SimulationType;
class Call;
class GA_SO;
class GA_MO;

#include <vector>
#include <ostream>
#include <unordered_set>
#include <map>

class Topology;

#include "../Calls/EventGenerator.h"

/**
 * @brief The Data class is responsible to storage the simulation data.
 */
class Data {
    
    friend std::ostream& operator<<(std::ostream& ostream, 
    const Data* data);
public:
    /**
     * @brief Standard constructor for a Data object.
     * @param simulType SimulationType object that owns 
     * this data.
     */
    Data(SimulationType* simulType);
    /**
     * @brief Copy constructor for a Data object.
     * @param orig original Data object.
     */
    Data(const Data& orig);
    /**
     * @brief Virtual destructor of a Data object.
     */
    virtual ~Data();
    
    /**
     * @brief Initialize the data. The vectors are resized based in the 
     * number of load points. And sets all values to 0.
     */
    void Initialize();
    /**
     * @brief Function to initialize the data containers for a specified number
     * of points.
     * @param numPos Number of points.
     */
    void Initialize(unsigned int numPos);
    /**
     * @brief Update the data based on the call.
     * If call is Accepted, increment the number of accepted requests.
     * If call is Blocked, increment the number of blocked requests.
     * @param call Call to analyze.
     */
    void StorageCall(Call* call);
    /**
     * @brief Saves the data values in the Log.txt file.
     */
    void SaveLog();
    /**
     * @brief Function to save the simulation log for a set of devices.
     * @param vecParam Container of devices.
     */
    void SaveLog(std::vector<unsigned> vecParam);
    /**
     * @brief Saves the data values in the PBvLoad.txt file.
     */
    void SaveBP();
    /**
     * @brief Function to save the specified parameters with the main result(BP) 
     * in and output file.
     * @param vecParam Container of parameters.
     */
    void SaveBP(std::vector<unsigned> vecParam);
    /**
     * @brief Saves the data values in the NetUtiliz.txt file.
     */
    void SaveNetUtiliz();
    /**
     * @brief Saves the data values in the NetFrag.txt file.
     */
    void SaveNetFrag();
    /**
     * @brief Saves the data values in the AccumNetFrag.txt file.
     */
    void SaveAccumNetFrag();
    /**
     * @brief Saves the data values in the FragTraffic.txt file.
     */
    void SaveFragTraffic();
    /**
     * @brief Saves the data values in the LinksUse.txt file.
     */
    void SaveLinksUse();
    /**
     * @brief Saves the data values in the SlotsRelativeUse.txt file.
     */
    void SaveSlotsRelativeUse();
    /**
     * @brief Saves the data values in the NetProtRate.txt file.
     */
    void SaveNetProtRate();
    /**
     * @brief Saves the data values in the NetNonProtRate.txt file.
     */
    void SaveNetNonProtRate();
    /**
     * @brief Saves the data values in the NetBetaAverage.txt file.
     */
    void SaveNetBetaAverage();
    /**
    * @brief Saves the data values in the NetAlphaAverage.txt file.
    */
    void SaveNetAlphaAverage();
    /**
    * @brief Saves the data values in the numHopsRoutes.txt file.
    */
    void SaveNumHopsRoutes();
    /**
     * @brief Saves the genetic algorithms files. Log, initial population, 
     * best individuals, worst individuals and best individual of the last
     * generation.
     */
    void SaveGaFiles();
    /**
     * @brief Saves the Particle Swarm Optimization files.
     */
    void SaveFilesPSO();    
    /**
     * @brief Set the number of request of the actual load point.
     * @param numReq Number of requests.
     */
    void SetNumberReq(double numReq);
    /**
     * @brief Gets the number of requests for the actual load point.
     * @return Number of requests.
     */
    double GetNumberReq() const;
    /**
     * @brief Function that return the number of blocked call requests for the
     * actual point.
     * @return Number of blocked requests.
     */
    double GetNumberBlocReq() const;
    /**
     * @brief Function to get the number of accepted call requests for the 
     * actual point.
     * @return Number of accepted call requests.
     */
    double GetNumberAccReq() const;
    /**
     * @brief Function to get the network call request blocking probability for 
     * the actual point.
     * @return Call request blocking probability.
     */
    double GetReqBP() const;
    /**
     * @brief Function to get the total bandwidth used by the call requests in
     * the actual simulation.
     * @return Total bandwidth.
     */
    double GetNumberSlotsReq() const;
    /**
     * @brief Function to get the accepted call requests bandwidth for the 
     * actual simulation.
     * @return Accepted bandwidth.
     */
    double GetNumberBlocSlots() const;
    /**
     * @brief Function to get the blocked call requests bandwidth for the 
     * actual simulation.
     * @return Blocked bandwidth.
     */
    double GetNumberAccSlots() const;
    /**
     * @brief Function to get the network bandwidth blocking probability for
     * the actual simulation.
     * @return Bandwidth blocking probability.
     */
    double GetSlotsBP() const;
    /**
     * @brief Function to get the mean of hops per route for the actual 
     * simulation.
     * @return Mean of hops per route.
     */
    double GetNumHopsPerRoute() const;
    /**
     * @brief Function to get the average number fo hops of all accepted call
     * requests.
     * @return Average number of hops.
     */
    double GetAverageNumHops() const;
    /**
     * @brief Function to get the network occupancy for the actual simulation.
     * @return Network occupancy.
     */
    double GetNetOccupancy() const;
    /**
     * @brief Function to get the average network utilization. This calculation
     * is done dividing the accepted call requests utilization by the network
     * total utilization. The later is calculated multiplying the total number
     * of link, the number of slots per link and the total simulation time.
     * @return Average network utilization.
     */
    double GetAverageNetUtilization() const;
    /**
     * @brief Function to update the network fragmentation ratio, calculating
     * the average value between the old and the new ratio value.
     * @param ratio New network fragmentation ratio value.
     */
    void UpdateFragmentationRatio(double ratio);
    /**
     * @brief Function to get the network fragmentation ratio.
     * @return Network fragmentation ratio.
     */
    double GetNetworkFragmentationRatio() const;
    /**
     * @brief Function to get the simulation time for the actual simulation.
     * @return Simulation time.
     */
    TIME GetSimulTime() const;
    /**
     * @brief Function to set the simulation time for the actual simulation.
     * @param simulTime Simulation time.
     */
    void SetSimulTime(const TIME simulTime);
    /**
     * @brief Function to get the computer simulation time for the actual 
     * simulation.
     * @return Computer simulation time.
     */
    TIME GetRealSimulTime() const;
    /**
     * @brief Function to set the computer simulation time for the actual 
     * simulation.
     * @param simulTime Computer simulation time.
     */
    void SetRealSimulTime(const TIME simulTime);
    /**
     * @brief Set the links use number for the actual load point.
     * @param topology pointer.
     */    
    void SetLinksUse(Topology* topology);
    /**
     * @brief Set the slots relative use number of request of the actual load point.
     * @param call pointer.
     */
    void SetSlotsRelativeUse(Call* call);
    /**
     * @brief Set the network fragmentation ratio on dependence of a accumulated band
     * for the actual load point.
     * @param band current accumulated band .
     */ 
    void SetBandFrag(const double band, const double netFrag);
    /**
     * @brief Function to get the index of the actual simulation point.
     * @return Actual simulation point index.
     */
    unsigned int GetActualIndex() const;
    /**
     * @brief Function to get the index of the actual simulation point.
     * @param actualIndex Actual simulation point index.
     */
    void SetActualIndex(unsigned int actualIndex);
    /**
     * @brief Function to get the network protection calls rate for 
     * the actual point.
     * @return Call request blocking probability.
     */
    double GetProtRate() const;
     /**
     * @brief Function to get the network non protection calls rate for 
     * the actual point.
     * @return Call request blocking probability.
     */
    double GetNonProtRate() const;
    /**
     * @brief Function to get the total number of protection calls for 
     * the actual point.
     * @return Call request blocking probability.
     */
    double GetProtectedCalls() const;
    /**
     * @brief Function to set/increment the total number of protection calls for
     * the actual point.
     */
    void SetProtectedCalls();
        /**
     * @brief Function to get the total number of non protection calls for 
     * the actual point.
     * @return Call request blocking probability.
     */
    double GetNonProtectedCalls() const;
    /**
     * @brief Function to set/increment the total number of non protection calls for
     * the actual point.
     */
    void SetNonProtectedCalls();
    /**
     * @brief Function to get the sum of all calls beta average for actual load point.
     */
    double GetSumCallsBetaAverage() const;
    /**
     * @brief Function to set/update the sum of all calls beta average on actual load point.
     */
    void SetSumCallsBetaAverage(double callBetaAverage);
    /**
    * @brief Function to get the sum of all calls alpha on actual load point.
    */
    double GetSumCallsAlpha() const;
    /**
    * @brief Function to set/update the sum of all calls alpha on actual load point.
    */
    void SetSumCallsAlpha(double callAlpha);
    /**
     * @brief Function to get the network beta average for the actual point.
     * @return network beta average.
     */
    double GetNetBetaAverage() const;
    /**
    * @brief Function to get the network alpha average for the actual point.
    * @return network alpha average.
    */
    double GetNetAlphaAverage() const;

private:
    /**
     * @brief Saves the data in PBvLoad.txt.
     * @param ostream Stream that contain the PBvLoad file.
     */
    void SaveCallReqBP(std::ostream& ostream);
    /**
     * @brief Function to save the bandwidth blocking probability into a output
     * stream.
     * @param ostream Output stream.
     */
    void SaveBandwidthBP(std::ostream& ostream);    
    /**
     * @brief Function to save the Average Network Utilization into a output
     * stream.
     * @param ostream Output stream.
     */
    void SaveNetUtiliz(std::ostream& ostream);
    /**
     * @brief Function to save the Network Fragmentation into a output stream.
     * @param ostream Output stream.
     */
    void SaveNetFrag(std::ostream& ostream);
    /**
     * @brief Function to save the accumulated Network Fragmentation into a output stream.
     * @param ostream Output stream.
     */
    void SaveAccumNetFrag(std::ostream& ostream);
    /**
     * @brief Function to save the Network Fragmentatio on depending of band into a 
     * output stream.
     * @param ostream Output stream.
     */
    void SaveBandFrag(std::ostream& ostream);
    /**
     * @brief Function to save the Network Links Usage into a output stream.
     * @param ostream Output stream.
     */
    void SaveLinksUse(std::ostream& ostream);
    /**
     * @brief Function to save the Network Slot relative Use into a output stream.
     * @param ostream Output stream.
     */
    void SaveSlotsRelativeUse(std::ostream& ostream);
    /**
     * @brief Function to save the Network Protection Rate into a output stream.
     * @param ostream Output stream.
     */
    void SaveNetProtRate(std::ostream& ostream);
    /**
     * @brief Function to save the Network Non Protection Rate into a output stream.
     * @param ostream Output stream.
     */
    void SaveNetNonProtRate(std::ostream& ostream);
    /**
     * @brief Function to save the Network Beta Average into a output stream.
     * @param ostream Output stream.
     */
    void SaveNetBetaAverage(std::ostream& ostream);
    /**
    * @brief Function to save the Network Alpha Average into a output stream.
    * @param ostream Output stream.
    */
    void SaveNetAlphaAverage(std::ostream& ostream);
    /**
    * @brief Function to save number of Hops per Routes into a output stream.
    * @param ostream Output stream.
    */
    void SaveNumHopsRoutes(std::ostream& ostream);

    /**
     * @brief Function to save specified paramters with the main result(BP) to 
     * a file.
     * @param ostream Output file.
     * @param vec Container of parameters.
     */
    void SaveCallReqBP(std::ostream& ostream, std::vector<unsigned> vec);
    /**
     * @brief Function to save the bandwidth blocking probability with a set
     * of devices into a output stream.
     * @param ostream Output stream.
     * @param vec Container of devices.
     */
    void SaveBandwidthBP(std::ostream& ostream, std::vector<unsigned> vec);
    /**
     * @brief Function to save all GA single objective data in the output files.
     * @param logOfstream Output log file.
     * @param initPop Output file for the initial population.
     * @param bestInds Output file for the GA best individuals.
     * @param worstInds Output file for the GA worst individuals.
     * @param bestInd Output file for the GA best individual.
     */
    void SaveGaSoFiles(std::ostream& logOfstream, std::ostream& initPop, 
    std::ostream& bestInds, std::ostream& worstInds, std::ostream& bestInd);
    /**
     * @brief Function to save the best and the worst individuals in their
     * respective files and in the log file.
     * @param ga GA single objective algorithm.
     * @param logOfstream Output logo file.
     * @param bestInds Output file for the GA best individuals.
     * @param worstInds Output file for the GA worst individuals.
     */
    void SaveBestWorstIndividuals(GA_SO* ga, std::ostream& logOfstream, 
    std::ostream& bestInds, std::ostream& worstInds);
    /**
     * @brief Function to save the best individual (genes) in the output file.
     * @param ga GA single objective algorithm.
     * @param bestInd Output file for the GA best individual.
     */
    void SaveBestIndividual(GA_SO* ga, std::ostream& bestInd);
    /**
     * @brief Function to save the initial population in the output file.
     * @param ga GA single objective algorithm.
     * @param initPop Output file for the GA best individual.
     */
    void SaveInitPopulation(GA_SO* ga, std::ostream& initPop);
    /**
     * @brief Function to save all GA multi-objective data in the output files.
     * @param logOfstream Output log file.
     * @param bestInds Output file for the GA Pareto fronts.
     * @param bestInd Output file for the GA last Pareto front individuals.
     */
    void SaveGaMoFiles(std::ostream& logOfstream, std::ostream& bestInds, 
    std::ostream& bestInd);
    /**
     * @brief Function to save the GA Pareto fronts in the output files.
     * @param ga GA multi-objective algorithm.
     * @param logOfstream Output log file.
     * @param bestInds Output file for the GA Pareto fronts.
     */
    void SaveParetoFronts(GA_MO* ga, std::ostream& logOfstream, 
    std::ostream& bestInds);
    /**
     * @brief Function to save the last GA Pareto front in the output file.
     * @param ga GA multi-objective algorithm.
     * @param bestInd Output file for the GA last Pareto front individuals.
     */
    void SaveLastIndividuals(GA_MO* ga, std::ostream& bestInd);
    
private:
    /**
     * @brief A pointer to the simulation this object belong
     */
    SimulationType* simulType;
    /**
     * @brief Actual number of requisitions simulated, per load.
     */
    std::vector<double> numberReq;
    /**
     * @brief Actual number of blocked requisitions simulated,
     * per load.
     */
    std::vector<double> numberBlocReq;
    /**
     * @brief Actual number of accepted requisitions simulated,
     * per load.
     */
    std::vector<double> numberAccReq;
    /**
     * @brief Total number of slots of each request, for eaach load point.
     */
    std::vector<double> numberSlotsReq;
    /**
     * @brief Actual number of blocked slots simulated,
     * per load.
     */
    std::vector<double> numberBlocSlots;
    /**
     * @brief Actual number of accepted slots simulated,
     * per load.
     */
    std::vector<double> numberAccSlots;
     /**
     * @brief Actual Integer number of accepted slots simulated,
     * per load.
     */
    std::vector<unsigned int> numberAccSlotsInt;
    /**
     * @brief Mean of hops per route, per load.
     */
    std::vector<double> numHopsPerRoute;
    /**
     * @brief Network occupancy, per load.
     * It is Calculated multiplying the number of slots
     * occupied by the number of route hops.
     */
    std::vector<double> netOccupancy;
    /**
     * @brief Accepted call requests utilization. The utilization is calculated
     * multiplying the number of slots used, the number of route links and the 
     * time the connection remain active.
     */
    std::vector<double> accReqUtilization;
    /**
     * @brief Network fragmentation ratio, the mean of all links fragmentation.
     */
    std::vector<double> netFragmentationRatio;
    /**
     * @brief Vector which accumulates all the Network fragmentation ratio
     * measured between the event interval, the mean of all links fragmentation.
     */
    std::vector<std::vector<double>> accumNetFragmentationRatio;    
    
    std::vector<std::vector<std::pair<double, double>>> fragPerTraffic;
    
    std::vector<std::map<std::pair<unsigned, unsigned>, unsigned>> linksUse;
     /**
     * @brief Vector that contain the slots relative uses for each load point.
     */
    std::vector<std::vector<double>> slotsRelativeUse;  
     /**
     * @brief Vector that contain the total number of protected calls
     * for each load point.
     */
    std::vector<double> protectedCalls;  
     /**
     * @brief Vector that contain the total number of non protected calls
     * for each load point.
     */
    std::vector<double> nonProtectedCalls; 
    /**
     * @brief Vector that contain the summation of calls beta average for each load point.
     */
    std::vector<double> sumCallsBetaAverage;
    /**
    * @brief Vector that contain the summation of calls alpha for each load point.
    */
    std::vector<double> sumCallsAlpha;

    /**
     * @brief Vector that contain the simulation time of each load point.
     */
    std::vector<TIME> simulTime;
    /**
     * @brief Container of computer simulation times.
     */
    std::vector<TIME> realSimulTime;
    /**
     * @brief Index of the actual simulation. This is referred to multiload
     * simulation, to indicate the position of the vector to update the data.
     */
    unsigned int actualIndex;
};

#endif /* DATA_H */

