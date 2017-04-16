//
//  ProcessManager.hpp
//  CSE7343 - Semester Project
//
//  Created by Smith, Eric on 4/12/17.
//  Copyright © 2017 Smith, Eric. All rights reserved.
//

#ifndef ProcessManager_hpp
#define ProcessManager_hpp
#include "CustomQueue.hpp"
#include <stdio.h>
#include <vector>

enum Mode { SJF, FCFS, Priority, RoundRobin, FirstFit, BestFit, WorstFit };

class ProcessManager {
protected:
    CustomQueue* queue;
    std::vector<ProcessControlBlock*> * processVector;
    
public:
    ProcessManager(CustomQueue* queue);
    ~ProcessManager();
    
    void updateProcessVector();
    void sortProcessVector(Mode m=FCFS);
    ProcessControlBlock* getPCB(int index);
};

#endif /* ProcessManager_hpp */
