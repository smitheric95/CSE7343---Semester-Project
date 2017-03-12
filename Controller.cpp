//
//  Controller.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 3/12/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "Controller.hpp"

Controller::Controller() {
    //create all necessary queues
    addQueues();
    
    
    // controller must keep track of pcb ids that have been added (table)
    // prevent from adding same pcb
}

Controller::~Controller(){
    delete readyQueue;
    delete waitingQueue;
}

void Controller::addQueues() {
    readyQueue = new CustomQueue("Ready");
    waitingQueue = new CustomQueue("Waiting");
}
