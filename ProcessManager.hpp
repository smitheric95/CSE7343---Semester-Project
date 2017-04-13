//
//  ProcessManager.hpp
//  CSE7343 - Semester Project
//
//  Created by Smith, Eric on 4/12/17.
//  Copyright © 2017 Smith, Eric. All rights reserved.
//

#ifndef ProcessManager_hpp
#define ProcessManager_hpp
#include "CustomQueue.hpp"
#include <stdio.h>
#include <vector>

class ProcessManager {
protected:
    CustomQueue* queue;
    std::vector<ProcessControlBlock*> * processVector;
    
public:
    ProcessManager(CustomQueue* queue);
    ~ProcessManager();
    
    void updateProcessVector();
};

#endif /* ProcessManager_hpp */
