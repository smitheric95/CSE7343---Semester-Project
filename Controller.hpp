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
#include <iostream>
#include <sstream>
#include <regex>
#include <string>
#include <unordered_map>

class Controller {
private:
    CustomQueue* readyQueue;
    CustomQueue* waitingQueue;
    //std::unordered_map<std::string,double> processTable;
    
    std::ifstream file;
    
    void addQueues();
    
public:
    Controller(std::string file);
    ~Controller();
    bool parseFile(std::string file);
    void setFile(std::string file);
};

#endif /* Controller_hpp */
