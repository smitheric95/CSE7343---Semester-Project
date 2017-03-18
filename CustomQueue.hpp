//
//  CustomQueue.hpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 2/18/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#ifndef CustomQueue_hpp
#define CustomQueue_hpp

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "ProcessControlBlock.hpp"

// scheduling algorithm
enum Mode { SJF, FCFS, Priority, RoundRobin };

class CustomQueue {
   private:
    ProcessControlBlock* head;
    ProcessControlBlock* tail;
    std::string name;
    
    ProcessControlBlock* sortedMerge(ProcessControlBlock*, ProcessControlBlock* b, Mode m);
    void frontBackSplit(ProcessControlBlock* source,
                        ProcessControlBlock** frontRef,
                        ProcessControlBlock** backRef);
    void reestablishTail();
    
   public:
    CustomQueue(std::string name);
    CustomQueue(std::string name, ProcessControlBlock* head);
    ~CustomQueue();

    ProcessControlBlock* remove(int PID = -1);
    void add(ProcessControlBlock* cur);
    ProcessControlBlock* peek();

    std::string getName();
    ProcessControlBlock* getHead();

    bool isEmpty();
    void print();

    int comparePCBs(ProcessControlBlock* LHS, ProcessControlBlock* RHS, Mode m = SJF);
    void sort(ProcessControlBlock** headRef, Mode m);
};
#endif /* CustomQueue_hpp */
