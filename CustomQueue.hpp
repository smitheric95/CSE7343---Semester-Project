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
#include <limits> 
#include "ProcessControlBlock.hpp"

// scheduling algorithm
enum Mode { SJF, FCFS, Priority, RoundRobin };

class CustomQueue {
   private:
    ProcessControlBlock* head;
    ProcessControlBlock* tail;
    std::string name;
    void reestablishTail();
    int size;
    
   public:
    CustomQueue(std::string name);
    CustomQueue(std::string name, ProcessControlBlock* head);
    ~CustomQueue();

    ProcessControlBlock* remove(int PID = -1);
    bool add(ProcessControlBlock* PCB, int position=0);
    ProcessControlBlock* peek();

    std::string getName();
    ProcessControlBlock* getHead();

    bool isEmpty();
    void print();
    
    // scheduler
    int comparePCBs(ProcessControlBlock* LHS, ProcessControlBlock* RHS, Mode m = SJF);
    void sortVector(Mode m);
    void shortestJobFirst();
    void firstComeFirstServe();
    void priority();
    void roundRobin(int quantum);
};
#endif /* CustomQueue_hpp */
