
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

    // create all necessary queues
    addQueues();

    handleUserInput();
    ProcessControlBlock* head = waitingQueue->getHead();
    waitingQueue->sort(&head, SJF);
    waitingQueue->print();
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
        // number of times a user has been prompted for a particular command
        int promptCount = 0;

        // exit
        if (modeSelection == 0) {
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        // select input file to build queues
        else if (modeSelection == 1) {
            std::string filename;

            // check the users input
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
            if (parseFile(filename)) {
                std::cout << "Input successfully entered." << std::endl << std::endl;
                this->inputFileParsed = true;
            }
            else {
                std::cout << "Unable to process input." << std::endl << std::endl;
            }
            displayMainMenu(true);
        }
        // select command line to build queues
        else if (modeSelection == 2) {
            std::string commandLine;

            // check the users input
            while (true) {
                if (promptCount == 0)
                    std::cin.ignore();

                // prompt the user to enter file name
                std::cout << "Please enter a command to parse: ([0] to go back)" << std::endl;
                std::getline(std::cin, commandLine);
                std::cin.clear();

                // exit to main menu
                if (commandLine == "0")
                    break;

                // user has entered correct input, process command
                if (lineIsValid(commandLine)) {
                    // process was successfully added
                    if (addProcess(commandLine)) {
                        std::cout << "Process added to waiting queue." << std::endl;
                    }
                    else {
                        std::cout << "Error adding process. Please try again." << std::endl;
                    }
                }
                // user has entered an error
                else {
                    std::cout << "Unable to process: \"" << commandLine << "\"" << std::endl;
                    std::cout << "Usage is: <1-9999>, <1-999>, <1-999>, <1-999>" << std::endl;
                }
                promptCount++;
            }
            promptCount = 0;
            displayMainMenu(true);
        }
        else {
            std::cout << "Incorrect input. Please try again: " << std::endl;
        }
    }
}

// parse file given by user, build processTable
bool Controller::parseFile(std::string file) {
    // go through each line
    std::string line;
    int lineCount = 1;

    while (getline(this->file, line)) {
        // check format of input
        if (lineIsValid(line)) {
            if (!addProcess(line, file, lineCount))
                return false;
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

// edit/add PID and it's value in process table
void Controller::editProcessTable(int PID, int value) {
    std::unordered_map<int, int>::iterator processIndex = this->processTable.find(PID);

    // edit
    if (processIndex != this->processTable.end())
        processIndex->second = value;
    // add
    else
        this->processTable.insert(std::make_pair(PID, value));
}

// returns process status in process table, if process doesn't exist, -1
int Controller::processStatus(int PID) {
    std::unordered_map<int, int>::const_iterator processIndex = this->processTable.find(PID);

    if (processIndex == this->processTable.end()) {
        return -1;
    }
    // return the value associated with the process (0 for waiting, 1 for ready, 2 for completed)
    else {
        return processIndex->second;
    }
}

void Controller::addQueues() {
    readyQueue = new CustomQueue("Ready");
    waitingQueue = new CustomQueue("Waiting");
}

// returns true if a line of input is syntactically valid
bool Controller::lineIsValid(const std::string& line) {
    return (std::regex_match(line,
                             std::regex("^0*[1-9]{1}[0-9]{0,3}( *, *0*[1-9]{1}[0-9]{0,2})* *$")) &&
            std::count(line.begin(), line.end(), ',') == 3);
}

// takes a valid input line and turns into a PCB
// PCB is added to processTable and waitingQueue
// returns true if PCB is sucessfully added
bool Controller::addProcess(std::string line, std::string file, int lineCount) {
    // store process values
    std::vector<int> processValues;
    std::stringstream tempStream(line);

    // split string up by commas
    while (tempStream.good()) {
        std::string substr;
        std::getline(tempStream, substr, ',');

        // remove white space
        substr.erase(std::remove(substr.begin(), substr.end(), ' '), substr.end());

        // convert to int before pushing to vector
        processValues.push_back(std::stoi(substr));
    }

    int curPID = processValues[0];

    // check to see if PCB has been added before
    if (processStatus(curPID) > -1) {
        // passed from input file
        if (lineCount > 0) {
            std::cout << "Error: Duplicate process on line " << lineCount << ". Command ignored."
                      << std::endl;
        }
        // passed from command line
        else {
            std::cout << "Error: Duplicate process. Command ignored." << std::endl;
        }
        return false;
    }
    // priority is too low
    else if (processValues[3] == 0 || processValues[3] > 4) {
        // passed from input file
        if (lineCount > 0) {
            std::cout << "Error: on line " << lineCount
                      << ". Priority must be between 1 and 4. Command ignored." << std::endl;
        }
        // passed from command line
        else {
            std::cout << "Error: Priority must be between 1 and 4. Command ignored." << std::endl;
        }
        return false;
    }

    // add to waiting queue
    waitingQueue->add(new ProcessControlBlock(processValues));

    // add to process table as 'waiting'
    editProcessTable(curPID, 0);

    return true;
}
