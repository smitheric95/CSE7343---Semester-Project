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
#include "Scheduler.hpp"
#include <fstream>
#include <iostream>
#include <locale>    
#include <sstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>


class Controller {
private:
    CustomQueue* readyQueue; // null if no processes
    CustomQueue* waitingQueue;
    CustomQueue* selectedQueue; // points to waiting or ready
    
    Mode schedulingMode;
/*
   Key -> Value
   PID -> (0 for waiting)
          (1 for ready)
*/  std::unordered_map<int,int> processTable;
    
    std::ifstream file;
    int roundRobinQuantum;
    
public:
    Controller();
    ~Controller();
    void addQueues();
    void displayMenu(std::string menu);
    void init();
    void parseFile(std::string file);
    void editProcessTable(int PID, int value);
    int processStatus(int PID);
    bool lineIsValid(const std::string &line);
    bool addProcess(std::string line, std::string file=std::string(), int lineCount=0, std::string position="default");
    int getRoundRobinQuantum();
    void setRoundRobinQuantum(int q);
    std::string getSchedulingMode(Mode m);
    void editQueue(int queueSelection);
    
};

#endif /* Controller_hpp */
