//
//  Scheduler.hpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 3/16/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#ifndef Scheduler_hpp
#define Scheduler_hpp

#include <stdio.h>
#include <algorithm>
#include "CustomQueue.hpp"

class Scheduler {
private:
    CustomQueue* queue;
    std::vector<ProcessControlBlock*> * processVector;
    
public:
    Scheduler(CustomQueue* queue);
    ~Scheduler();
    void updateProcessVector();
    void sortVector(Mode m);
    
    // scheduling algorithms
    void shortestJobFirst();
    void firstComeFirstServe();
    void priority();
    void roundRobin(int quantum);
};

#endif /* Scheduler_hpp */
