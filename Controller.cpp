//
//  Controller.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 3/12/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "Controller.hpp"

Controller::Controller(std::string file) {
    // open input file
    this->setFile(file);

    // create all necessary queues
    addQueues();

    // controller must keep track of pcb ids that have been added (table)
    // prevent from adding same pcb
}

Controller::~Controller() {
    delete readyQueue;
    delete waitingQueue;
    this->file.close();
}

void Controller::addQueues() {
    readyQueue = new CustomQueue("Ready");
    waitingQueue = new CustomQueue("Waiting");
}

void Controller::setFile(std::string file) {
    this->file.open(file, std::ios::in);
    
    if (!this->file.is_open()) {
        std::cout << "Unable to open file: " << file << std::endl;
        exit(0);
    }
}
