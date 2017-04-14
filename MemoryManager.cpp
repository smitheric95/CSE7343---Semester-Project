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

void MemoryManager::firstFit() {
    // update the process vector to have the correct order
    this->updateProcessVector();

    int n = (int)this->processVector->size();
    int time = 0, totalBurstTime = 0, executedProcesses = 0;
    vector<int> burstTimes, arrivalTimes;

    // sort process vector first by arrival time, then priority
    sortProcessVector(FCFS);

    // initialize burst times and arrival times
    for (auto p : *(this->processVector)) {
        int curBurst = p->getBurstTime();
        burstTimes.push_back(curBurst);
        totalBurstTime += curBurst;

        arrivalTimes.push_back(p->getArrivalTime());

        // ensure that all processes have enough memory
        int i = 0;
        while (i < memory.size())
            if (get<0>(memory[i]) >= p->getMemorySpace())
                break;

        // the process can never be executed
        if (i == memory.size()) {
            cout << "Not enough memory to execute the process P" << p->getPID() << endl;
            n--;
        }
    }

    while (executedProcesses <= n) {
        // update each block of memory
        for (auto m : memory) {
            // go through each process in the current block
            for (int i = 0; i < get<1>(m).size(); i++) {
                // current process
                ProcessControlBlock* p = get<1>(m)[i];

                // if the process has finished
                if (p->getBurstTimeRemaining() == 0) {
                    // update memory block's available space
                    m.first += p->getMemorySpace();

                    // remove the process from the memory block
                    get<1>(m).erase(get<1>(m).begin() + i);

                    executedProcesses++;
                    i--;
                }
                // decrement the remaining burst time of the process
                else {
                    p->setBurstTimeRemaining(p->getBurstTimeRemaining() - 1);
                }
            }

        }  // end for

        // go through the remaining processes to be "executed"
        for (int i = 0; i < arrivalTimes.size(); i++) {
            // if the process has arrived
            if (arrivalTimes[i] <= time) {
                ProcessControlBlock* curProcess = this->processVector->at(i);

                // find a place in memory to fit the process
                int j = 0;
                while (j < memory.size()) {
                    auto m = memory[j];
                    // if the process fits, add it to the memory block
                    if (get<0>(m) >= curProcess->getMemorySpace()) {
                        cout << "Adding process P" << curProcess->getPID() << " to memory block "
                             << j << endl;
                        get<1>(m).push_back(curProcess);
                        break;
                    }
                    j++;
                }  // end while

                // a spot in memory was not found: fragmentation!
                if (j == memory.size())
                    cout << "Fragmentation. Not enough room to add P: " << curProcess->getPID()
                         << endl;
            }  // end for
        }
        // increment time
        time++;
    }  // end while
}
