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
 * @brief The Data class is responsible for storage the simulation
 * data.
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
     * @brief Gets the number of requests for a specified load point.
     * @param index Index of the load point.
     * @return Number of requests.
     */
    double GetNumberReq(unsigned int index) const;
    /**
     * @brief Gets the number of requests for the actual load point.
     * @return Number of requests.
     */
    double GetNumberReq() const;
    /**
     * @brief Gets the number of blocked requests for a specified load point.
     * @param index Index of the load point.
     * @return Number of requests.
     */
    double GetNumberBlocReq(unsigned int index) const;
    /**
     * @brief Function that return the number of blocked call requests.
     * @return Number of blocked requests.
     */
    double GetNumberBlocReq() const;
    
    double GetNumberAccReq(unsigned int index) const;

    double GetNumberAccReq() const;
    
    double GetPbReq() const;
    
    double GetNumberSlotsReq() const;
    
    double GetNumberBlocSlots(unsigned int index) const;
    
    double GetNumberBlocSlots() const;
    
    double GetNumberAccSlots(unsigned int index) const;
    
    double GetNumberAccSlots() const;
    
    double GetPbSlots() const;

    double GetNumHopsPerRoute(unsigned int index) const;
    
    double GetNumHopsPerRoute() const;
    
    double GetNetOccupancy(unsigned int index) const;
    
    double GetNetOccupancy() const;
    
    TIME GetSimulTime() const;
    
    void SetSimulTime(const TIME simulTime);
    
    TIME GetRealSimulTime() const;
    
    void SetRealSimulTime(const TIME simulTime);
    
    unsigned int GetActualIndex() const;

    void SetActualIndex(unsigned int actualIndex);

private:
    /**
     * @brief Saves the data in PBvLoad.txt.
     * @param ostream Stream that contain the PBvLoad file.
     */
    void SavePBvLoad(std::ostream& ostream);
    
    void SaveGaSoFiles(std::ostream& logOfstream, std::ostream& initPop, 
    std::ostream& bestInds, std::ostream& worstInds, std::ostream& bestInd);
    
    void SaveBestWorstIndividuals(GA_SO* ga, std::ostream& logOfstream, 
    std::ostream& bestInds, std::ostream& worstInds);
    
    void SaveBestIndividual(GA_SO* ga, std::ostream& bestInd);
    
    void SaveInitPopulation(GA_SO* ga, std::ostream& initPop);
    
    void SaveGaMoFiles(std::ostream& logOfstream, std::ostream& bestInds);
    
    void SaveParetoFronts(GA_MO* ga, std::ostream& logOfstream, 
    std::ostream& bestInds);
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
    
    std::vector<TIME> realSimulTime;
    /**
     * @brief Index of the actual simulation. This is referred to multiload
     * simulation, to indicate the position of the vector to update the data.
     */
    unsigned int actualIndex;
};

#endif /* DATA_H */

