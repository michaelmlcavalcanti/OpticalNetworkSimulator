/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SA.h
 * Author: BrunoVinicius
 *
 * Created on November 22, 2018, 11:36 PM
 */

#ifndef SA_H
#define SA_H

#include <random>
#include <cstdlib>

class ResourceAlloc;
class Topology;
class Call;
class CallDevices;

#include "../Data/Options.h"

/**
 * @brief Class responsible for spectral allocation of the call requests.
 */
class SA {
public:
    /**
     * @brief Standard constructor for a SA object.
     * @param rsa RourceAlloc that own this object.
     * @param option Spectral allocation option.
     * @param topology Topology used in this spectral allocation.
     */
    SA(ResourceAlloc* rsa, SpectrumAllocationOption option, Topology* topology);
    /**
     * @brief Virtual destructor of a SA object.
     */
    virtual ~SA();
    
    /**
     * @brief Function that choose the spectral allocation based on the option 
     * for a specified call request.
     * @param call Call request.
     */
    void SpecAllocation(Call* call);
    /**
     * @brief Function to apply the spectrum allocation for a call request with
     * devices.
     * @param call Call request to allocate.
     */
    void SpecAllocation(CallDevices* call);
    /**
     * @brief Function that gets the possible first slots for spectral 
     * allocation based on the last possible first slot.
     * @param lastSlot Last possible first slot.
     * @return Container of possible first slots.
     */
    std::vector<unsigned int> SpecAllocation();
    
    /**
     * @brief Function to apply random spectral allocation to a call request.
     * This SA randomly select between the available slots.
     * @param call Call request.
     */
    void Random(Call* call);
    /**
     * @brief Function to apply random spectral allocation to a call request.
     * This SA select the first available slots.
     * @param call Call request.
     */
    virtual void FirstFit(Call* call);
    
    void MostUsed(Call* call);
    
    void LeastUsed(Call* call);
    
    void MaxSum(Call* call);
    
    void RCL(Call* call);
    /**
     * @brief Function to apply the MSCL spectral allocation to a call request.
     * This SA select the option with minimum loss of capacity, considering the
     * interfering routes of the call route.
     * @param call Call request.
     */
    virtual void MSCL(Call* call);
    
    /**
     * @brief Gets the topology in which this SA is done.
     * @return Topology pointer.
     */
    Topology* GetTopology();
    /**
     * @brief Gets the ResosurceAlloc object that owns this SA.
     * @return ResourceAlloc pointer.
     */
    ResourceAlloc* GetResourceAlloc();
    
    int CalcNumFormAloc(int L, bool* Disp, int tam);
private:
    /**
     * @brief Gets the vector with all possible first slots for spectrum
     * allocation, with random ordination.
     * @param lastSlot Last first slot possible for allocation.
     * @return Vector with possible first slots.
     */
    std::vector<unsigned int> RandomSlots();
    /**
     * @brief Gets the vector with all possible first slots for spectrum
     * allocation, with first fit ordination.
     * @param lastSlot Last first slot possible for allocation.
     * @return Vector with possible first slots.
     */
    std::vector<unsigned int> FirstFitSlots();
    /**
     * @brief Function that find all possible slots for SA, based on FF order.
     * @param call Call request.
     * @return Vector with all possible slots for allocation.
     */
    std::vector<unsigned int> FirstFitSlots(Call* call);
    /**
     * @brief Function to calculate the number of forms of a call request number
     * of slots in a specified availability vector.
     * @param reqSize Call request number of slots.
     * @param dispVec Availability vector.
     * @return Number of forms.
     */
    unsigned int CalcNumFormAloc(unsigned int reqSize, 
                                 std::vector<bool>& dispVec);
    /**
     * @brief Function to get the blocks of free slots equal or larger then the 
     * specified call request number of slots.
     * @param reqSize Call request number of slots.
     * @param dispVec Availability vector.
     * @return Container with the size of free slots blocks.
     */
    std::vector<unsigned int> GetBlocksFreeSlots(unsigned int reqSize, 
                                                 std::vector<bool>& dispVec);
private:
    /**
     * @brief RourceAlloc that own this object.
     */
    ResourceAlloc* resourceAlloc;
    /**
     * @brief Spectral allocation option.
     */
    SpectrumAllocationOption specAllOption;
    /**
     * @brief Topology used in this SA
     */
    Topology* topology;
};

#endif /* SA_H */

