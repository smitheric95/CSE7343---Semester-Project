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

class ProcessControlBlock{
private:
    int priority;
    int PID;
    
    ProcessControlBlock* next;
    ProcessControlBlock* prev;

public:
    ProcessControlBlock();
    ProcessControlBlock(int priority, int PID);
    
    int getPriority();
    void setPriority(int priority);
    int getPID();
    void setPID(int PID);
    ProcessControlBlock* getNext();
    void setNext(ProcessControlBlock* next);
    ProcessControlBlock* getPrev();
    void setPrev(ProcessControlBlock* prev);
    void print();
};

#endif /* ProcessControlBlock_hpp */
