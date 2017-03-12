//
//  Controller.hpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 3/12/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#ifndef Controller_hpp
#define Controller_hpp

#include <stdio.h>
#include "ProcessControlBlock.hpp"
#include "CustomQueue.hpp"
#include <fstream>

class Controller {
private:
    CustomQueue* readyQueue;
    CustomQueue* waitingQueue;
    std::ofstream file;
    
    void addQueues();
    
public:
    Controller(std::string file);
    ~Controller();
    
    void setFile(std::string file);
};

#endif /* Controller_hpp */
