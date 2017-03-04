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
    
public:
    CustomQueue();
    CustomQueue(ProcessControlBlock* head);
    
    ProcessControlBlock* pop();
    void remove(int PID = -1);
    
    void push(ProcessControlBlock* cur);
    ProcessControlBlock* peek();
    bool isEmpty();
    void print();
};
#endif /* CustomQueue_hpp */
