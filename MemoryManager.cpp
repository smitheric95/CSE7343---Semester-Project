//
//  MemoryManager.cpp
//  CSE7343 - Semester Project
//
//  Created by Smith, Eric on 4/12/17.
//  Copyright © 2017 Smith, Eric. All rights reserved.
//

#include "MemoryManager.hpp"

using namespace std;

// inherit from the ProcessManager
MemoryManager::MemoryManager(CustomQueue* queue, vector<int> memorySizes) : ProcessManager(queue) {
    // update the process vector to have the correct order
    this->updateProcessVector();
    
    // add each memory block of size s
    for (auto s : memorySizes)
        this->addMemoryBlock(s);
}

void MemoryManager::addMemoryBlock(int blockSize) {
    memory.push_back(
        pair<int, vector<ProcessControlBlock*> >(blockSize, vector<ProcessControlBlock*>()));
}
