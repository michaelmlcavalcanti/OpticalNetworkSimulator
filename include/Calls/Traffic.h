/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Traffic.h
 * Author: bruno
 *
 * Created on August 14, 2018, 1:15 AM
 */

#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <vector>

class SimulationType;

/**
 * @brief Class Traffic represents the traffic of a simulation.
 */
class Traffic {
    friend std::ostream& operator<<(std::ostream& ostream, 
    const Traffic* traffic);
    
public:
    /**
     * @brief Standard constructor for a Traffic object.
     * @param simulType SimulationType object that owns 
     * this Traffic.
     */
    Traffic(SimulationType* simulType);
    /**
     * @brief Copy constructor for a Traffic object.
     * @param orig orig original Traffic object.
     */
    Traffic(const Traffic& orig);
    /**
     * @brief Virtual destructor of a Traffic object.
     */
    virtual ~Traffic();
    
    /**
     * @brief  Loads the traffic from a .txt file.
     */
    void LoadFile();
    
    /**
     * Returns the vector with all traffic values.
     * @return traffic values.
     */
    std::vector<double> GetVecTraffic() const;
    /**
     * @brief Get a specified traffic.
     * @param index index of traffic vector.
     * @return Value of chosen traffic.
     */
    double GetTraffic(unsigned int index) const;
    /**
     * @brief Set the vector with all traffic values.
     * @param vecTraffic all traffic values.
     */
    void SetVecTraffic(std::vector<double> vecTraffic);

    unsigned int GetTrafficIndex(double bitRate) const;
private:
    /**
     * @brief A pointer to the simulation this object belong.
     */
    SimulationType* simulType;
    /**
     * @brief Vector that storage all traffic values.
     */
    std::vector<double> vecTraffic;
};

#endif /* TRAFFIC_H */

