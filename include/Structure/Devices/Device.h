/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Device.h
 * Author: BrunoVinicius
 *
 * Created on April 28, 2019, 3:19 PM
 */

#ifndef DEVICE_H
#define DEVICE_H

class Topology;
class NodeDevices;

enum DeviceType {
    DeviceUndefined,
    DeviceRegenerator,
    DeviceAmplifier,
    DeviceTransponder
};

/**
 * @brief Basic device class used in the physical topology.
 */
class Device {
public:
    /**
     * @brief Standard constructor of a basic device object.
     * @param topology Physical topology to which this device belongs.
     * @param type Type that characterizes the device.
     */
    Device(Topology* topology, NodeDevices* node, 
    DeviceType type = DeviceUndefined);
    /**
     * @brief Standard destructor of a basic device object.
     */
    virtual ~Device();
    
    /**
     * @brief Initialize the main parameters of a device.
     */
    virtual void Initialize() = 0;
protected:
    /**
     * @brief Physical topology to which this device belongs.
     */
    Topology* topology;
    /**
     * @brief Node to which this SBVT belongs.
     */
    NodeDevices* node;
private:
    /**
     * @brief Type that characterizes the device.
     */
    DeviceType type;
};

#endif /* DEVICE_H */

