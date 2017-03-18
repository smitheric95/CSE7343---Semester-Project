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

class CustomQueue{
private:
    ProcessControlBlock* head;
    ProcessControlBlock* tail;
    std::string name;
    
public:
    CustomQueue(std::string name);
    CustomQueue(std::string name, ProcessControlBlock* head);
    ~CustomQueue();
    
    ProcessControlBlock* remove(int PID = -1);
    void add(ProcessControlBlock* cur);
    ProcessControlBlock* peek();
    
    std::string getName();
    
    int comparePCBs(ProcessControlBlock* LHS, ProcessControlBlock* RHS, int comparison=0);
    void sort();
    
    bool isEmpty();
    void print();
};
#endif /* CustomQueue_hpp */
