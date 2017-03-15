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
#include <utility>


class Controller {
private:
    CustomQueue* readyQueue; // null if no processes
    CustomQueue* waitingQueue;
    
/*
   Key -> Value
   PID -> (0 for waiting)
          (1 for ready)
          (2 for completed)
*/  std::unordered_map<int,int> processTable;
    
    std::ifstream file;
    bool inputFileParsed;
    
public:
    Controller();
    ~Controller();
    void addQueues();
    void displayMainMenu(bool shortHand=false);
    void handleUserInput();
    void displayErrorMessage();
    bool parseFile(std::string file);
    void editProcessTable(int PID, int value);
    int processStatus(int PID);
    bool lineIsValid(std::string line);
};

#endif /* Controller_hpp */
