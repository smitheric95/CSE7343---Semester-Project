//
//  main.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 2/14/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include <iostream>
#include "ProcessControlBlock.hpp"
#include "CustomQueue.hpp"


int main(int argc, const char * argv[]) {
    
    ProcessControlBlock* x = new ProcessControlBlock(1, 123);
    ProcessControlBlock* y = new ProcessControlBlock(2, 456);
    ProcessControlBlock* z = new ProcessControlBlock(3, 789);
    
    CustomQueue processes(x);
    processes.push(y);
    processes.push(z);
    processes.remove(789);
    processes.print();
    
    delete x;
    delete y;
    
    return 0;
}
