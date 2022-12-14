/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationType.h
 * Author: bruno
 *
 * Created on August 2, 2018, 3:50 PM
 */

#ifndef SIMULATIONTYPE_H
#define SIMULATIONTYPE_H

#include <memory>
#include <iostream>

class Parameters;
class Options;
class Data;
class Topology;
class InputOutput;
class Traffic;
class EventGenerator;
class ResourceAlloc;

#include "../Kernel.h"
#include "../GeneralClasses/Def.h"

/**
 * @brief Base class for Simulations objects.
 */
class SimulationType {
public:
    /**
     * @brief Standard constructor for a SimulationType object.
     * @param simulIndex index of this simulation.
     */
    SimulationType(SimulIndex simulIndex, TypeSimulation typeSimulation);
    /**
     * @brief Virtual destructor of a SimulationType object.
     */
    virtual ~SimulationType();
    
    /**
     * @brief Runs a simulation with specified parameters.
     */
    virtual void Run();
    /**
     * @brief Function used to call the Run function of the base class 
     * SimulationType.
     */
    virtual void RunBase() = 0;
    /**
     * @brief Load parameters to the simulation from the terminal.
     */
    virtual void Load();
    /**
     * @brief Load parameters, options and such,
     * to the simulation from a .txt file.
     */
    virtual void LoadFile();
    /**
     * @brief Sets SimulationType additional settings for this simulation,
     * as maximum length and initial cost for the links in topology.
     */
    virtual void AdditionalSettings();
    /**
     * @brief Print this simulation in terminal.
     */
    virtual void Print();
    /**
     * @brief Save the results in .txt files.
     */
    virtual void Save();
    /**
     * @brief Prints a description of the simulation.
     * @param ostream Output stream.
     */
    virtual std::ostream& Help(std::ostream& ostream) = 0;
    
    /**
     * @brief Function to get the type of simulation.
     * @return 
     */
    TypeSimulation GetTypeSimulation() const;
    /**
     * @brief Returns the simulation index
     * @return Simulation index
     */
    SimulIndex GetSimulationIndex() const;
    /**
     * @brief Returns a pointer to a Parameters object 
     * in this simulation
     * @return pointer to a Parameters object
     */
    Parameters* GetParameters() const;
    /**
     * @brief Returns a pointer to a Options object 
     * in this simulation
     * @return pointer to a Options object
     */
    Options* GetOptions() const;
    /**
     * @brief Returns a pointer to a Data object 
     * in this simulation
     * @return pointer to a Data object
     */
    Data* GetData() const;
    /**
     * @brief Returns a pointer to a Topology object 
     * used in this simulation
     * @return pointer to a Topology object
     */
    Topology* GetTopology() const;
    /**
     * @brief Returns a pointer to a InputOutput object 
     * used in this simulation.
     * @return pointer to a InputOutput object.
     */
    InputOutput* GetInputOutput() const;
    /**
     * @brief Returns a pointer to a Traffic object
     * used in this simulation.
     * @return pointer to a Traffic object.
     */
    Traffic* GetTraffic() const;
    /**
     * @brief Returns a pointer to a CallGenerator object
     * used in this simulation.
     * @return pointer to a CallGenerator object.
     */
    EventGenerator* GetCallGenerator() const;
    /**
     * @brief Returns a pointer to a ResourceAlloc object
     * used in this simulation.
     * @return pointer to a ResourceAlloc object.
     */
    ResourceAlloc* GetResourceAlloc() const;
private:
    /**
     * @brief Initialize all the simulation parameters, such as
     * Topology and CallGenerator. Also set to 0 the actual number 
     * of requests.
     */
    void InitializeAll();
    /**
     * @brief Function to do the actual simulation, generating the
     * new calls/events and implementing the first event in container.
     * This function can be limited by the total number of requests
     * or by the total number of blocked requests.
     */
    void Simulate();
    /**
     * @brief Finalize all the simulation parameters, such as
     * Topology and CallGenerator.
     */
    void FinalizeAll();
    /**
     * @brief Function to create a load the resource allocation object.
     */
    void CreateLoadResourceAlloc();
    
    /**
     * @brief Function to simulate for a specified total number of call 
     * requests.
     */
    void SimulateNumTotalReq();
    /**
     * @brief Function to simulate for a specified number of blocked call
     * requests.
     */
    void SimulateNumBlocReq();
public:
    /**
     * @brief Actual number of request created.
     * This variable is updated when a new call/event is create,
     * in function GenerateCall of CallGenerator class.
     */
    NumRequest numberRequests;
private:
    /**
     * @brief Define the type of simulation based on the option chosen.
     */
    TypeSimulation typeSimulation;
    /**
     * @brief Index of the simulation
     */
    const SimulIndex simulationIndex;
    /**
     * @brief pointer to an Parameters object used in this simulation
     */
    std::shared_ptr<Parameters> parameters;
    /**
     * @brief pointer to an Options object used in this simulation
     */
    std::shared_ptr<Options> options;
    /**
     * @brief pointer to an Data object used in this simulation
     */
    std::unique_ptr<Data> data;
    /**
     * @brief pointer to Topology object used in this simulation
     */
    std::shared_ptr<Topology> topology;
    /**
     * @brief pointer to InputOutput object used in this simulation
     */
    std::unique_ptr<InputOutput> inputOutput;
    /**
     * @brief pointer to Traffic object used in this simulation
     */
    std::shared_ptr<Traffic> traffic;
    /**
     * @brief pointer to a CallGenerator object.
     */
    std::shared_ptr<EventGenerator> callGenerator;
    /**
     * @brief Pointer to the ResourceAlloc object of this simulation.
     */
    std::shared_ptr<ResourceAlloc> resourceAlloc;
};

#endif /* SIMULATIONTYPE_H */

