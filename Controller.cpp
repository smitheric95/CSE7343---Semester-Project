//
//  Controller.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 3/12/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "Controller.hpp"

Controller::Controller(std::string file) {
    // open input file
    this->file.open(file, std::ios::in);

    // if the file was found, parse it
    if (this->file.is_open()) {
        // create all necessary queues
        addQueues();

        // go through each line
        std::string line;
        int lineCount = 1;

        while (getline(this->file, line)) {
            // check format of input
            if (std::regex_match(line, std::regex("(0*[0-9]{1,4})+\\,(0*[0-9]{1,4})+\\,+(0*[0-9]{1,"
                                                  "4})+\\,+(0*[0-9]{1,4})"))) {
                std::cout << line << std::endl;
            }
            //format is wrong, stop program
            else {
                std::cout << "ERROR: Incorrect format in " << file << " on line " << lineCount
                          << std::endl;
                std::cout << "Usage is: <0-9999>,<0-9999>,<0-9999>,<0-9999>" << std::endl;
                exit(0);
            }
            lineCount++;
        }
    }
    else {
        std::cout << "Unable to open file: " << file << std::endl;
    }
}

Controller::~Controller() {
    delete readyQueue;
    delete waitingQueue;
    this->file.close();
}

void Controller::addQueues() {
    readyQueue = new CustomQueue("Ready");
    waitingQueue = new CustomQueue("Waiting");
}

// to be deleted
void Controller::setFile(std::string file) {
    // this->file.open(file, std::ios::in);
}
