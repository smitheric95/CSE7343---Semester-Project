//
//  main.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 2/14/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include <iostream>
#include "ProcessControlBlock.hpp"
#include "Controller.hpp"

int main(int argc, const char * argv[]) {
    if (argv[1] == nullptr)
        std::cout << "No text file specified\nUsage: ./CSE7343\\ -\\ Semester\\ Project input.txt" << std::endl;
    else{
        Controller main(argv[1]);
        // std::cout << "Unable to open file: " << argv[1] << std::endl;
    }
    return 0;
}
