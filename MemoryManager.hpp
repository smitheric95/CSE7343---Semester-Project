//
//  MemoryManager.hpp
//  CSE7343 - Semester Project
//
//  Created by Smith, Eric on 4/12/17.
//  Copyright © 2017 Smith, Eric. All rights reserved.
//

#ifndef MemoryManager_hpp
#define MemoryManager_hpp

#include <stdio.h>
#include "ProcessManager.hpp"

#endif /* MemoryManager_hpp */

class MemoryManager : public ProcessManager {
private:
    int numBlocks;
    int totalMemorySize;
    
    // memory blocks
    std::vector<
        std::pair<
            int, // availble memory left
            std::vector<ProcessControlBlock*> // processes in the blocks
        >
    > memory;
    
public:
    MemoryManager(CustomQueue* queue, int totalMemorySize, std::vector<int> memorySizes);
    void calculateMemoryUsage(Mode mode, std::string title);
};
