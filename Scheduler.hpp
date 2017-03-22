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

class Scheduler {
private:
    Mode m;
    CustomQueue* queue;
    std::vector<ProcessControlBlock*> * processVector;
    
public:
    Scheduler(CustomQueue* queue, Mode m=SJF);
    ~Scheduler();
    void updateProcessVector();
    
    void changeMode(Mode m);
    Mode getMode();
};

#endif /* Scheduler_hpp */
