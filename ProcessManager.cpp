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
