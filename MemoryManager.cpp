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

    /* FOR TESTING ONLY */
    static const int arr[] = {300, 200, 350, 125};
    // static const int arr[] = {300,600,350,200,125,900,1,400,399,239,3329,2343,234,9999, 1999,
    // 1222, 1111, 9999, 3333,664,82,33};
    vector<int> testingMemorySizes(arr, arr + sizeof(arr) / sizeof(arr[0]));

    // add each memory
    //    for (auto blockSize : memorySizes)
    /*************************/

    for (auto blockSize : testingMemorySizes)
        memory.push_back(
            pair<int, vector<ProcessControlBlock*>>(blockSize, vector<ProcessControlBlock*>()));

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
    cout << "--------------------------------------------------------- First Fit "
            "---------------------------------------------------------"
         << endl;
    // update the process vector to have the correct order
    this->updateProcessVector();

    int n = (int)this->processVector->size();
    int originalN = n;

    int time = 0, totalBurstTime = 0, totalWaitTime = 0, executedProcesses = 0, numFrags = 0,
        totalMemory = 0, maxMemoryUsed = 0, maxMemoryTime = 0;
    
    vector<int> burstTimes, arrivalTimes, inMemory, waitTimes;
    
    // sort process vector first by arrival time, then priority
    sortProcessVector(FCFS);
    
    // determine a total memory
    for (auto m : memory) totalMemory += get<0>(m);
    
    // initialize burst times and arrival times
    for (auto p : *(this->processVector)) {
        int curBurst = p->getBurstTime();
        burstTimes.push_back(curBurst);
        totalBurstTime += curBurst;

        arrivalTimes.push_back(p->getArrivalTime());

        // whether or not the process is in memory OR has completed
        // (0 for false, 1 for true)
        inMemory.push_back(0);

        waitTimes.push_back(0);  // how long a fragmented process has to wait

        // ensure that all processes have enough memory
        int i = 0;
        while (i < memory.size()) {
            if (get<0>(memory[i]) >= p->getMemorySpace())
                break;
            i++;
        }
        // the process can never be executed
        if (i == memory.size()) {
            cout << "Not enough memory to execute the process P" << p->getPID() << endl;
            inMemory.back() = 1;
            n--;
        }
    }
    
    cout << endl;

    // there are still valid processes to be loaded into memory
    while (executedProcesses < n) {
        int j = 0, curMemoryUsed = 0;
        
        // update each block of memory
        while (j < memory.size()) {
            std::pair<int, std::vector<ProcessControlBlock*>>* m = &memory[j];

            // go through each process in the current block
            int processesPerBlock = (int)get<1>(*m).size();
            for (int i = 0; i < processesPerBlock; i++) {
                // current process
                ProcessControlBlock* p = get<1>(*m)[i];

                // if the process has finished
                if (p->getBurstTimeRemaining() == 0) {
                    // update memory block's available space
                    (*m).first += p->getMemorySpace();
                    cout << "t=" << time << ": \t ";
                    cout << "Process P" << p->getPID() << " has completed. \t\t ";
                    cout << " \t (Memory block " << j + 1 << " now has " << (*m).first
                         << " units of available space.)" << endl;

                    // remove the process from the memory block
                    get<1>(*m).erase(get<1>(*m).begin() + i);
                    executedProcesses++;
                    processesPerBlock--;
                }
                // decrement the remaining burst time of the process
                else {
                    p->setBurstTimeRemaining(p->getBurstTimeRemaining() - 1);
                }
            }
            
            j++;
            curMemoryUsed += get<0>(*m);
        }  // end memory for loop
        
        curMemoryUsed = totalMemory - curMemoryUsed;
        
        // go through the remaining processes to be "executed"
        for (int i = 0; i < arrivalTimes.size(); i++) {
            // if the process has arrived and it's not currently in memory
            if (arrivalTimes[i] <= time && inMemory[i] == 0) {
                ProcessControlBlock* curProcess = this->processVector->at(i);

                // find a place in memory to fit the process
                int j = 0;
                while (j < memory.size()) {
                    std::pair<int, std::vector<ProcessControlBlock*>>* m = &memory[j];
                    
                    // if the process fits, add it to the memory block
                    if (get<0>(*m) >= curProcess->getMemorySpace()) {
                        cout << "t=" << time << ": \t ";
                        cout << "Adding process P" << curProcess->getPID() << " to memory block "
                             << j + 1 << ".";
                        get<1>(*m).push_back(curProcess);

                        // assume the process wil be completed
                        inMemory[i] = 1;

                        (*m).first -= curProcess->getMemorySpace();

                        cout << "\t\t (Memory block " << j + 1 << " now has " << (*m).first
                             << " units of available space.)" << endl;
                        break;
                    }
                    j++;
                }  // end while

                // a spot in memory was not found: fragmentation!
                if (j == memory.size()) {
                    cout << "t=" << time << ": \t ";
                    cout << "Fragmentation. Not enough room to add P" << curProcess->getPID() << "."
                         << endl;
                    waitTimes[i] += 1;
                    numFrags++;
                }
            }  // end for
        }

        // check to see if this is the maximum amount of memory used
        if (curMemoryUsed > maxMemoryUsed) {
            maxMemoryUsed = curMemoryUsed;
            maxMemoryTime = time-1;
        }
        
        // increment time
        time++;
    }  // end while

    // calculate total wait time
    for (int w : waitTimes) totalWaitTime += w;
    
    // print stats
    cout << "--------------------------------------------------------------------"
            "---------------------------------------------------------"
         << endl;
    
    cout << n << " processes were loaded into and out of memory with " << numFrags
         << " fragmentations." << endl;
    
    if (originalN != n)
        cout << (originalN - n) << " processes weren't able to be completed." << endl;
    
    cout << "Blocking probability: " << (int)(n*1.0/originalN*100) << "%" << endl;
    cout << "Total wait time of fragmented processes: " << totalWaitTime << endl;
    cout << "Maximum memory utilization: " << (int)(maxMemoryUsed*1.0/totalMemory*100) << "% occurs at t=" << maxMemoryTime << endl;
    
    cout << "--------------------------------------------------------------------"
        "---------------------------------------------------------"
        << endl;
}
