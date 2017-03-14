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
    CustomQueue* readyQueue; // null if no processes
    CustomQueue* waitingQueue;
    //std::unordered_map<std::string,double> processTable;
    
    std::ifstream file;
    
public:
    Controller();
    ~Controller();
    bool inputFileParsed;
    void addQueues();
    void displayMainMenu(bool shortHand=false);
    void handleUserInput();
    void displayErrorMessage();
    bool parseFile(std::string file);
};

#endif /* Controller_hpp */
