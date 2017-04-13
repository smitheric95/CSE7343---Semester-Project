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
    for (auto blockSize : memorySizes)
        memory.push_back(
            pair<int, vector<ProcessControlBlock*> >(blockSize, vector<ProcessControlBlock*>()));

//    for (int i = 0; i < this->processVector->size(); i++) {
//        get<1>(this->memory[i]).push_back(this->processVector->at(i));
//    }
//
//    for (auto m : memory) {
//        cout << get<0>(m) << ": ";
//        for (auto i : get<1>(m)) cout << i->getPID() << endl;
//    }
}
