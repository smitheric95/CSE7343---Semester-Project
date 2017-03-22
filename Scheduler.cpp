//
//  Scheduler.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 3/16/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "Scheduler.hpp"

Scheduler::Scheduler(CustomQueue* queue, Mode m) : queue(queue), m(m), processVector(nullptr) {
    updateProcessVector();
}

Scheduler::~Scheduler() {
    if (this->processVector != nullptr)
        delete this->processVector;
}

//
void Scheduler::updateProcessVector() {
    // delete old values
    if (this->processVector != nullptr)
        delete this->processVector;
    
    this->processVector = new std::vector<ProcessControlBlock*>;
    
    // update values to match queue
    ProcessControlBlock* cur = this->queue->peek();
    while (cur != nullptr) {
        std::cout << cur->getPID() << std::endl;
        this->processVector->push_back(cur);
        cur = cur->getNext();
    }
}

void Scheduler::changeMode(Mode m) {
    this->m = m;
}

Mode Scheduler::getMode() {
    return this->m;
}

