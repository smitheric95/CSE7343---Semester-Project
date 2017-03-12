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
    
    CustomQueue processes("ready", x);
    processes.add(y);
    processes.add(z);
    //processes.remove()->print();
    
    //processes.push(y);
    processes.print();
    
    // controller must keep track of pcb ids that have been added (table)
    // prevent from adding same pcb
    
    delete z;
    delete x;
    delete y;
    
    return 0;
}
