/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Regenerator.h
 * Author: BrunoVinicius
 *
 * Created on April 28, 2019, 4:51 PM
 */

#ifndef REGENERATOR_H
#define REGENERATOR_H

#include "Device.h"
#include "../../Calls/EventGenerator.h"

/**
 * @brief Class that represents a virtualized electronic regenerator.
 */
class Regenerator : public Device {
public:
    /**
     * @brief Standard constructor of a virtualized regenerator object.
     * @param topology Physical topology to which this regenerator belongs.
     * @param node Node to which this regenerator belongs.
     */
    Regenerator(Topology* topology, NodeDevices* node);
    /**
     * @brief Standard constructor of a virtualized regenerator object.
     */
    virtual ~Regenerator();
    
    /**
     * @brief Initialize all parameters, active flag, actual activation time and
     * total activation time, in this regenerator.
     */
    void Initialize() override;

    /**
     * @brief Function to set this regeneration on.
     */
    void SetRegeneratorOn();
    /**
     * @brief Function to set this regeneration off.
     */
    void SetRegeneratorOff();
    /**
     * @brief Function to check if the regenerator is occupied.
     * @return 
     */
    bool IsActive() const;
    
    /**
     * @brief Function to get the traffic bit rate supported by this 
     * regenerator.
     * @return Traffic bit rate.
     */
    static const double GetTrafficSupported();
private:
    /**
     * @brief Regenerator state indicator.
     */
    bool isActive;
    /**
     * @brief Actual activation time.
     */
    TIME onTime;
    /**
     * @brief Total activation time.
     */
    TIME totalOnTime;

    /**
     * @brief Maximum traffic bit rate supported by this type of regenerator.
     */
    static const double trafficSupported;
};

#endif /* REGENERATOR_H */

