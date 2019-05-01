/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CallDevices.cpp
 * Author: BrunoVinicius
 * 
 * Created on April 30, 2019, 2:52 PM
 */

#include "../../include/Calls/CallDevices.h"

CallDevices::CallDevices(Node* orNode, Node* deNode, double bitRate, 
TIME deacTime):Call(orNode, deNode, bitRate, deacTime), transpSegments(0),
regenerators(0), totalNumSlots(0) {

}

CallDevices::~CallDevices() {

}
