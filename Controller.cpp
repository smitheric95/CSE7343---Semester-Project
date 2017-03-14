
//
//  Controller.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 3/12/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "Controller.hpp"

Controller::Controller() : readyQueue(nullptr), waitingQueue(nullptr), inputFileParsed(false) {
    displayMainMenu();
    handleUserInput();
}

Controller::~Controller() {
    if (readyQueue != nullptr)
        delete readyQueue;
    if (waitingQueue != nullptr)
        delete waitingQueue;
    this->file.close();
}

// simply displays the main menu
void Controller::displayMainMenu(bool shortHand) {
    if (shortHand) {
        std::cout << "################ Main Menu ###################" << std::endl;
        std::cout << "# [0] Exit [1] Input file [2] Command line   #" << std::endl;
        std::cout << "# [3] Execute processes [4] Select scheduler #" << std::endl;
        std::cout << "##############################################" << std::endl;
    }
    else {
        std::cout << "##############################################" << std::endl;
        std::cout << "# Welcome to EricOS !!                       #" << std::endl;
        std::cout << "#                                            #" << std::endl;
        std::cout << "# Version 1.0                                #" << std::endl;
        std::cout << "#                                            #" << std::endl;
        std::cout << "# Select process input type:                 #" << std::endl;
        std::cout << "#     [1] File [2] Command line              #" << std::endl;
        std::cout << "#                                            #" << std::endl;
        std::cout << "# [3] Execute processes                      #" << std::endl;
        std::cout << "# [4] Select scheduling algorithm            #" << std::endl;
        std::cout << "#                                            #" << std::endl;
        std::cout << "# [0] Exit                                   #" << std::endl;
        std::cout << "##############################################" << std::endl << std::endl;
    }
    std::cout << "Please select a mode: ";
}

void Controller::handleUserInput() {
    int modeSelection = 0;

    // while the user has not exited
    while (std::cin >> modeSelection) {
    
        // exit
        if (modeSelection == 0) {
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        // select input file to build queues
        else if (modeSelection == 1) {
            std::string filename;
            int promptCount = 0;

            do {
                // user has entered an error
                if (promptCount > 0)
                    std::cout << "Unable to open file: " << filename << std::endl;

                // prompt the user to enter file name
                std::cout << "Please enter a file to parse: ([0] to go back)" << std::endl;
                std::cin >> filename;

                // exit
                if (filename == "0")
                    break;
                // open file
                else
                    this->file.open(filename, std::ios::in);

                promptCount++;

            } while (!this->file.is_open());

            // user has entered correct input
            promptCount = 0;

            // if the file was found, parse it
            // create all necessary queues
            addQueues();

            // try parsing file
            if (parseFile(filename)) {
                std::cout << "Input successfully entered." << std::endl;
                this->inputFileParsed = true;
                displayMainMenu(true);
            }
        }
        else {
            displayErrorMessage();
        }

        /*
         // go back to mode selection == file
         std::cout << "Please select an input type: ([0] to exit)";
         std::cin >> modeSelection;
         */
    }
}

void Controller::displayErrorMessage() {
    std::cout << "Incorrect input. Please try again: " << std::endl;
}

// parse file given by user, build processTable
bool Controller::parseFile(std::string file) {
    // go through each line
    std::string line;
    int lineCount = 1;

    while (getline(this->file, line)) {
        // check format of input
        if (std::regex_match(line,
                             std::regex("^0*[1-9]{1}[0-9]{0,3}( *, *0*[1-9]{1}[0-9]{0,2})* *$"))) {
            // store process values
            std::vector<int> processValues;
            std::stringstream tempStream(line);

            // split string up by commas
            while (tempStream.good()) {
                std::string substr;
                std::getline(tempStream, substr, ',');

                // remove white space
                substr.erase(std::remove(substr.begin(), substr.end(), ' '), substr.end());

                // convert to int before pushing
                processValues.push_back(std::stoi(substr));
            }

            // add to waiting queue
            waitingQueue->add(new ProcessControlBlock(processValues));
        }
        // format is wrong, stop program
        else {
            std::cout << "ERROR: Incorrect format in " << file << " on line " << lineCount
                      << std::endl;
            std::cout << "Usage is: <1-9999>, <1-999>, <1-999>, <1-999>" << std::endl;
            return false;
        }
        lineCount++;
    }

    return true;
}

void Controller::addQueues() {
    readyQueue = new CustomQueue("Ready");
    waitingQueue = new CustomQueue("Waiting");
}

