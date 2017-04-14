//
//  ProcessManager.cpp
//  CSE7343 - Semester Project
//
//  Created by Smith, Eric on 4/12/17.
//  Copyright © 2017 Smith, Eric. All rights reserved.
//

#include "ProcessManager.hpp"
using namespace std;

ProcessManager::ProcessManager(CustomQueue* queue) : queue(queue), processVector(nullptr) {
    updateProcessVector();
}

ProcessManager::~ProcessManager() {
    if (this->processVector != nullptr)
        delete this->processVector;
}

// sorts vector based off mode
void ProcessManager::sortProcessVector(Mode m) {
    if (m == SJF)
        // sort process vector by burst time
        sort(this->processVector->begin(), this->processVector->end(),
             [](ProcessControlBlock* a, ProcessControlBlock* b) -> bool {
                 return a->getBurstTime() < b->getBurstTime();
             });
    else if (m == FCFS)
        // sort process vector by arrival, then priority
        sort(this->processVector->begin(), this->processVector->end(),
             [](ProcessControlBlock* a, ProcessControlBlock* b) -> bool {
                 // if same arrival, sort by priority
                 if (a->getArrivalTime() == b->getArrivalTime())
                     return a->getPriority() < b->getPriority();
                 return a->getArrivalTime() < b->getArrivalTime();
             });
    else if (m == RoundRobin)
        // sort process vector by PID
        sort(this->processVector->begin(), this->processVector->end(),
             [](ProcessControlBlock* a, ProcessControlBlock* b) -> bool {
                 return a->getPID() < b->getPID();
             });
    else  // sort process vector by PID
        sort(this->processVector->begin(), this->processVector->end(),
             [](ProcessControlBlock* a, ProcessControlBlock* b) -> bool {
                 return a->getPriority() < b->getPriority();
             });
}

// ensure processVector has the correct values
void ProcessManager::updateProcessVector() {
    // delete old values
    if (this->processVector != nullptr)
        delete this->processVector;
    
    this->processVector = new vector<ProcessControlBlock*>;
    
    // update values to match queue
    ProcessControlBlock* cur = this->queue->peek();
    while (cur != nullptr) {
        this->processVector->push_back(cur);
        cur = cur->getNext();
    }
}

// given an index,
// return a process control block from the process vector
ProcessControlBlock* ProcessManager::getPCB(int index) {
    return this->processVector->at(index);
}
