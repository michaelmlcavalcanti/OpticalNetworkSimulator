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

#include "../Calls/CallGenerator.h"

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
    void SaveMultiloadLog();
    /**
     * @brief Saves the data values in the PBvLoad.txt file.
     */
    void SavePBvLoad();
    /**
     * @brief Function to save the specified parameters with the main result(BP) 
     * in and output file.
     * @param vecParam Container of parameters.
     */
    void SaveMainResults(std::vector<unsigned> vecParam);
    /**
     * @brief Saves the genetic algorithms files. Log, initial population, 
     * best individuals, worst individuals and best individual of the last
     * generation.
     */
    void SaveGaFiles();
    
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
    double GetPbReq() const;
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
    double GetPbSlots() const;
    /**
     * @brief Function to get the mean of hops per route for the actual 
     * simulation.
     * @return Mean of hops per route.
     */
    double GetNumHopsPerRoute() const;
    /**
     * @brief Function to get the network occupancy for the actual simulation.
     * @return Network occupancy.
     */
    double GetNetOccupancy() const;
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
     * @brief Function to get the index of the actual simulation point.
     * @return Actual simulation point index.
     */
    unsigned int GetActualIndex() const;
    /**
     * @brief Function to get the index of the actual simulation point.
     * @param actualIndex Actual simulation point index.
     */
    void SetActualIndex(unsigned int actualIndex);
private:
    /**
     * @brief Saves the data in PBvLoad.txt.
     * @param ostream Stream that contain the PBvLoad file.
     */
    void SavePBvLoad(std::ostream& ostream);
    /**
     * @brief Function to save specified paramters with the main result(BP) to 
     * a file.
     * @param ostream Output file.
     * @param vec Container of parameters.
     */
    void SaveMainResults(std::ostream& ostream, std::vector<unsigned> vec);
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

