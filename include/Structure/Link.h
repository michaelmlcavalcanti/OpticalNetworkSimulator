/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Link.h
 * Author: bruno
 *
 * Created on August 4, 2018, 1:54 AM
 */

#ifndef LINK_H
#define LINK_H

#include <iostream>

/**
 * @brief Numerate the possible slot status.
 */
enum SlotStatus{
    SlotFree,
    SlotUsed
};

#include <vector>
#include <limits>
#include <iostream>

class Topology;
class Signal;

/**
 * @brief Class Link represents a link inside a topology. 
 */
class Link {
    
    friend std::ostream& operator<<(std::ostream& ostream,
    Link* link);
    
public:
    /**
     * @brief Standard constructor for a Link object.
     * @param topPointer pointer to a Topology object that
     * owns this link.
     * @param origimNode origin node of the link.
     * @param destinationNode destination node of the link.
     * @param length length of the link (meters).
     * @param numberSections total number of sections 
     * of the link.
     */
    Link(Topology* topPointer, unsigned int origimNode, 
    unsigned int destinationNode, double length, 
    unsigned int numberSections, unsigned int numberSlots);
    /**
     * @brief Virtual destructor of a Link object.
     */
    virtual ~Link();
    
    bool operator==(const Link& right) const;
    /**
     * @brief Initialize the link, setting the start values
     * contained in it to free. 
     */
    virtual void Initialize();
    
    /**
     * @brief Get the origin node of the link.
     * @return Origin node index of this link.
     */
    unsigned int GetOrigimNode() const;
    /**
     * @brief Set the origin node of the link.
     * @param origimNode origin node index of this link.
     */
    void SetOrigimNode(unsigned int origimNode);
    /**
     * @brief Get the destination node of the link.
     * @return Destination node index of this link.
     */
    unsigned int GetDestinationNode() const;
    /**
     * @brief Set the destination node of the link.
     * @param destinationNode Destination node index of this link.
     */
    void SetDestinationNode(unsigned int destinationNode);
    /**
     * @brief Get the length of the link.
     * @return Length of the link.
     */
    double GetLength() const;
    /**
     * @brief Set the length of the link.
     * @param length length of the link.
     */
    void SetLength(double length);
    /**
     * @brief Get the number of sections of the link.
     * @return Number of sections of the link.
     */
    unsigned int GetNumberSections() const;
    /**
     * @brief Set the number of sections of the link.
     * @param numberSections number of sections of the link.
     */
    void SetNumberSections(unsigned int numberSections);
    /**
     * @brief Get the cost of the link.
     * @return Cost of the link.
     */
    double GetCost() const;
    /**
     * @brief Set the cost of the link.
     * @param cost cost of the link.
     */
    void SetCost(double cost);
    /**
     * @brief Checks if the link is functional.
     * @return True if the link is working.
     */
    bool IsLinkWorking() const;
    /**
     * @brief Set the link state (working or not).
     * @param NodeWorking Link state.
     */
    void SetLinkState(bool linkWorking);
        
    /**
     * @brief Update the signal power, ASE power and
     * nonlinear power crossing this link.
     * @param signal Signal pointer that will be updated.
     */
    void CalcSignal(Signal* signal) const;
    /**
     * @brief Occupy an specified slot in this link.
     * @param index Slot index.
     */
    void OccupySlot(const unsigned int index);
    /**
     * @brief Release an specified slot in this link.
     * @param index Slot index.
     */
    void ReleaseSlot(const unsigned int index);
    /**
     * @brief Check if an specified slot is occupied in
     * the link.
     * @param index Slot index.
     * @return True if the slot is occupied.
     */
    bool IsSlotOccupied(unsigned int index) const;
    /**
     * @brief Check if an specified slot is free in
     * the link.
     * @param index Slot index.
     * @return True if the slot is free.
     */
    bool IsSlotFree(unsigned int index) const;
    /**
     * @brief Return the number of free slots in the link.
     * @return Number of free slots.
     */
    unsigned int GetNumberFreeSlots() const;
    /**
     * @brief Return the number of occupied slots in the link.
     * @return Number of occupied slots.
     */
    unsigned int GetNumberOccupiedSlots() const;
    /**
     * @brief Function to return pointer of the topology
     * @return Pointer to topology
     */
    Topology* GetTopology() const;
    
private:
    /**
     * @brief Pointer to a Topology object that
     * this link is contained
     */
    Topology* topPointer;
    /**
     * @brief Origin node of the link
     */
    unsigned int origimNode;
    /**
     * @brief Destination node of the link
     */
    unsigned int destinationNode;
    /**
     * @brief Length of this link (meters)
     */
    double length;
    /**
     * @brief Number of sections of this link
     */
    unsigned int numberSections;
    /**
     * @brief Cost of this link, based in the 
     * selected metric
     */
    double cost;
    /**
     * @brief Vector with slots status, free or occupied,
     * in this link
     */
    std::vector<SlotStatus> slotsStatus;
    /**
     * @brief Boolean variable to indicate the  link state.
     */
    bool linkWorking;
};

#endif /* LINK_H */

