//
//  ProcessControlBlock.hpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 2/15/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#ifndef ProcessControlBlock_hpp
#define ProcessControlBlock_hpp

#include <stdio.h>
#include <iostream> 
#include <vector>

class ProcessControlBlock{
private:
    int PID;
    int arrivalTime;
    int burstTime;
    int priority;
    
    ProcessControlBlock* next;
    ProcessControlBlock* prev;

public:
    ProcessControlBlock();
    ProcessControlBlock(std::vector<int> values);
    
    int getPriority();
    void setPriority(int priority);
    int getPID();
    void setPID(int PID);
    int getArrivalTime();
    void setArrivalTime(int arrivalTime);
    int getBurstTime();
    void setBurstTime(int burstTime);
    
    ProcessControlBlock* getNext();
    void setNext(ProcessControlBlock* next);
    ProcessControlBlock* getPrev();
    void setPrev(ProcessControlBlock* prev);
    void print();
};

#endif /* ProcessControlBlock_hpp */
