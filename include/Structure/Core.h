/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Core.h
 * Author: paulofilho
 *
 * Created on 4 de fevereiro de 2019, 15:17
 */

#ifndef CORE_H
#define CORE_H

#include <vector>
#include <cstddef>
#include <iostream>
#include <cassert>

#include "Link.h"

/**
 * @brief Core Class represents a core inside a Fiber. 
 */
class Core {
    
    friend std::ostream& operator<<(std::ostream& ostream,
    Core* core);
public:
    /**
     * @brief Standard constructor for a Core object.
     * @param cId integer that represents the index of 
     * the core.
     * @param nSlots represents the total numbr of slots
     * in a core.
     */
    Core(CoreIndex cId, unsigned int nSlots);
    
    Core(Link* link, CoreIndex coreId, unsigned int numSlots);
    /**
     * @brief Default destructor of Core object.
     */
    virtual ~Core();
    
    /**
     * @brief Function to initialize the core, setting all slots as free.
     */
    void Initialize();
    /**
     * @brief Get Id of the core.
     * @return Id of the core.
     */
    CoreIndex GetCoreId();
    /**
     * @brief Check if a specified slot is occupied in this core.
     * @param sPosition Represents the position of the slot in the core.
     * @return True if the slot is occupied.
     */
    bool IsSlotOccupied(SlotIndex sPosition);
    /**
     * @brief Check if a specified slot is free in this core.
     * @param sPosition Represents the position of the slot in the core.
     * @return True if the slot is free.
     */
    bool IsSlotFree(SlotIndex sPosition);
    /**
     * @brief Occupy the slot position.
     * @param Represents the position of the slot
     * in the core. 
     */
    void OccupySlot(SlotIndex sPosition);
     /**
     * @brief Release the slot position.
     * @param Represents the position of the slot
     * in the core. 
     */
    void ReleaseSlot(SlotIndex sPosition);
    
    bool IsCoreWorking();
    
    void SetCoreState(bool state);
    
    unsigned int GetNumSlots();
private:
    
    Link* link;
    /**
     * @brief Index of the core.
     */
    CoreIndex coreId;
    /**
     * @brief Vector of slots of this core.
     */
    std::vector<SlotState> slotsStatus;
    
    bool coreWorking;
};

#endif /* CORE_H */

