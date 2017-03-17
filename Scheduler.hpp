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
#include "CustomQueue.hpp"

// scheduling algorithm
enum Mode { SJF, FCFS, Priority, RoundRobin };

class Scheduler {
private:
    Mode m;
    CustomQueue* queue;
public:
    Scheduler(Mode m=SJF);
    void changeMode(Mode m);
    Mode getMode();
    int compare();
    
/*
 - keep track of waiting/ready queues
 - SJF, FCFS, non-pre priority are all the same with diff things
    to compare while sorting
    - requires own comparison algorithm
 - round-robin will require clarification
    - must take Q as an input
 - print average waiting time
 */
};

#endif /* Scheduler_hpp */
