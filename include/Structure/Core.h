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
     * @param link Link in which this core is inserted.
     * @param coreId Core index.
     * @param numSlots Number of slots of the core.
     */
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
    /**
     * @brief Function to check if the core is working.
     * @return Core state.
     */
    State IsCoreWorking();
    /**
     * @brief Function to set the core state.
     * @param state Core state.
     */
    void SetCoreState(State state);
    /**
     * @brief Function to get the core number of slots.
     * @return number of slots.
     */
    unsigned int GetNumSlots();
    /**
     * @brief Function to get the container with the core slots state.
     * @return Core slots state.
     */
    std::vector<SlotState> GetSlotsStatus() const;

    bool IsSlotOccupied(SlotIndex sPosition, SlotState type);

private:
    /**
     * @brief Link in which this core is inserted.
     */
    Link* link;
    /**
     * @brief Index of the core.
     */
    CoreIndex coreId;
    /**
     * @brief Vector of slots of this core.
     */
    std::vector<SlotState> slotsStatus;
    /**
     * @brief Core state (working or notWorking).
     */
    State coreState;

};

#endif /* CORE_H */

