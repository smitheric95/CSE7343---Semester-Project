//
//  Scheduler.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 3/16/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "Scheduler.hpp"

Scheduler::Scheduler(Mode m) : m(m), queue(nullptr) {
}

void Scheduler::changeMode(Mode m) {
    this->m = m;
}

Mode Scheduler::getMode() {
    return this->m;
}

// schedule a queue

// compare two PCBs
int Scheduler::compare() {
    //if mode ==
    return 0;
}

void Scheduler::printSchedule() {
    
}
