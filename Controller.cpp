//
//  Controller.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 3/12/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "Controller.hpp"

Controller::Controller() : readyQueue(nullptr), waitingQueue(nullptr), roundRobinQuantum(0) {
    displayMenu("main");

    // create all necessary queues
    addQueues();

    // prompt user
    init();
}

Controller::~Controller() {
    if (this->readyQueue != nullptr)
        delete this->readyQueue;
    if (this->waitingQueue != nullptr)
        delete this->waitingQueue;
    this->file.close();
}

// simply displays the main menu
void Controller::displayMenu(std::string menu) {
    if (menu == "main") {
        printf("\033c");
        std::cout << " #############################################" << std::endl;
        std::cout << "#  Welcome to EricOS                          #" << std::endl;
        std::cout << "#  Version 1.0                                #" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << "#  Main Menu:                                 #" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << "#  [1] Edit Ready Queue                       #" << std::endl;
        std::cout << "#  [2] Edit Waiting Queue                     #" << std::endl;
        std::cout << "#  [3] Execute processes                      #" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << "#  [0] Exit                                   #" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << " ############################################ " << std::endl << std::endl;
        std::cout << "Please select a mode: ";
    }
    else if (menu == "edit queue") {
        std::cout << " #############################################" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << "#  Edit queue:                                #" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << "#  [1] Print Queue                            #" << std::endl;
        std::cout << "#  [2] Add to Queue                           #" << std::endl;
        std::cout << "#  [3] Delete from Queue                      #" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << "#  [0] Go Back                                #" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << " #############################################" << std::endl << std::endl;
    }
    else if (menu == "add to queue") {
        this->selectedQueue->print();
        std::cout << " #############################################" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << "#  Add to queue:                              #" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << "#  [1] Add from file                          #" << std::endl;
        std::cout << "#  [2] Add from command line                  #" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << "#  [0] Go Back                                #" << std::endl;
        std::cout << "#                                             #" << std::endl;
        std::cout << " #############################################" << std::endl << std::endl;
        std::cout << "Please select a mode: ";
    }
}

void Controller::init() {
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

        // edit the ready/waiting queues
        else if (modeSelection == 1 || modeSelection == 2)
            editQueue(modeSelection);

        // Execute processses
        else if (modeSelection == 3) {
            // prompt for quantum
            promptCount = 0;
            while (this->roundRobinQuantum < 1 || this->roundRobinQuantum > 1000) {
                std::cout << "Please enter a quantum for Round Robin: <1-999>" << std::endl;
                std::cin >> this->roundRobinQuantum;

                // check for errors
                if (promptCount > 0) {
                    std::cout << "Incorrect input. Please try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                }
                promptCount++;
            }

            // execute processes with all four algorithms
            if (!this->readyQueue->isEmpty()) {
                printf("\033c");
                Scheduler main(this->readyQueue);
                main.shortestJobFirst();
                main.updateProcessVector();
                main.firstComeFirstServe();
                main.updateProcessVector();
                main.priority();
                main.updateProcessVector();
                main.roundRobin(this->roundRobinQuantum);
            }
            else {
                std::cout << "Ready Queue is empty. Please go back and add processes." << std::endl;
            }

            // loop till user exits
            std::cout << "\nEnter [0] to go back." << std::endl;
            std::string exit;
            while (true) {
                std::cin >> exit;
                if (exit == "0")
                    break;
            }
        }
        else {
            std::cout << "Incorrect input. Please try again: " << std::endl;
        }

        displayMenu("main");
    }
}

// parse file given by user, build processTable
void Controller::parseFile(std::string file) {  // go through each line
    std::string line;
    int lineCount = 0;
    int validProcesses = 0;

    while (getline(this->file, line)) {
        lineCount++;

        // check format of input
        if (lineIsValid(line)) {
            if (addProcess(line, file, lineCount))
                validProcesses++;
        }
        // format is wrong, don't add process
        else {
            std::cout << "\nERROR: Incorrect format in " << file << " on line " << lineCount
                      << std::endl;
            std::cout << "Usage is: <1-99999>, <0-9999>, <0-9999>, <1-4>" << std::endl << std::endl;
        }
    }

    std::cout << std::endl
              << validProcesses << " processes successfully entered. "
              << (lineCount - validProcesses) << " errors generated.\n"
              << std::endl;
}

void Controller::addQueues() {
    this->readyQueue = new CustomQueue("Ready");
    this->waitingQueue = new CustomQueue("Waiting");
}

// returns true if a line of input is syntactically valid
bool Controller::lineIsValid(const std::string& line) {
    return (std::regex_match(line, std::regex("^(0*[0-9]{1,5})( *, *0*[0-9]{1,4})* *$")) &&
            std::count(line.begin(), line.end(), ',') == 3);
}

// takes a valid input line and turns into a PCB
// PCB is added to processTable and waitingQueue
// returns true if PCB is sucessfully added
bool Controller::addProcess(std::string line,
                            std::string file,
                            int lineCount,
                            std::string position) {
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
    if ((std::find(this->waitingVector.begin(), this->waitingVector.end(), curPID) !=
             this->waitingVector.end() &&
         this->selectedQueue == this->waitingQueue) ||
        (std::find(this->readyVector.begin(), this->readyVector.end(), curPID) !=
             this->readyVector.end() &&
         this->selectedQueue == this->readyQueue)) {
            
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
    else if (processValues[0] < 1) {
        // passed from input file
        if (lineCount > 0) {
            std::cout << "Error: on line " << lineCount
            << ". PID must be from <0-99999>. Command ignored." << std::endl;
        }
        // passed from command line
        else {
            std::cout << "Error: PID must be from <0-99999>. Command ignored." << std::endl;
        }
        return false;
    }
    // add to selected queue
    if (this->selectedQueue->add(new ProcessControlBlock(processValues), position)) {
        // add to process table
        if (this->selectedQueue == this->readyQueue)
            this->readyVector.push_back(curPID);
        else
            this->waitingVector.push_back(curPID);
    }
    // given position was too large
    else {
        std::cout << "Error. Given position was too large." << std::endl;
        return false;
    }

    return true;
}

int Controller::getRoundRobinQuantum() {
    return this->roundRobinQuantum;
}
void Controller::setRoundRobinQuantum(int q) {
    this->roundRobinQuantum = q;
}

std::string Controller::getSchedulingMode(Mode m) {
    std::string arr[4] = {"SJF", "FCFS", "Priority", "Round Robin"};
    return arr[m];
}

// allows user to view queue
void Controller::editQueue(int queueSelection) {
    // select queue to use
    if (queueSelection == 1)
        this->selectedQueue = readyQueue;
    else
        this->selectedQueue = waitingQueue;

    // print queue menu
    printf("\033c");
    displayMenu("edit queue");

    std::string modeSelection = "0";  // reset mode selection
    int promptCount = 0;
    std::cout << "Select operation to perform on " << this->selectedQueue->getName() << " queue: ";

    while (std::cin >> modeSelection) {
        if (modeSelection == "0") {
            printf("\033c");
            break;
        }

        // print
        else if (modeSelection == "1") {
            printf("\033c");
            this->selectedQueue->print();
        }

        // add to queue
        else if (modeSelection == "2") {
            printf("\033c");
            displayMenu("add to queue");
            std::cin.clear();
            std::cin >> modeSelection;

            // select input file to build queues
            if (modeSelection == "1") {
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

                printf("\033c");

                // The file was found, parse it
                if (filename != "0")
                    parseFile(filename);
            }
            // select command line to build queues
            else if (modeSelection == "2") {
                std::string commandLine;

                // check the users input
                while (true) {
                    if (promptCount == 0)
                        std::cin.ignore();

                    // prompt the user to enter file name
                    std::cout << "Please enter a process to parse: ([0] to go back)" << std::endl;
                    std::getline(std::cin, commandLine);
                    std::cin.clear();

                    // exit to main menu
                    if (commandLine == "0") {
                        printf("\033c");
                        break;
                    }
                    // user has entered correct input, process command
                    if (lineIsValid(commandLine)) {
                        // prompt user to enter process at position
                        std::cout << "\nEnter the position to add the process:" << std::endl;
                        std::cout << "(Positions indexed at 0. Enter \"default\" to add to tail.)"
                                  << std::endl;
                        std::string position;
                        std::getline(std::cin, position);
                        std::cin.clear();

                        // position is a digit or was given as 'default' and process was
                        // successfully added
                        if ((position == "default" ||
                             std::all_of(position.begin(), position.end(), ::isdigit)) &&
                            addProcess(commandLine, std::string(), 0, position)) {
                            std::cout << "\nProcess added to the " << this->selectedQueue->getName()
                                      << " queue at position: " << position << "." << std::endl;
                        }
                        else {
                            std::cout << "\nNo processes were added to the queue. Please try again."
                                      << std::endl;
                        }
                    }
                    // user has entered an error
                    else {
                        std::cout << "Unable to process: \"" << commandLine << "\"" << std::endl;
                        std::cout << "Usage is: <1-99999>, <0-9999>, <0-9999>, <1-4>\n"
                                  << std::endl;
                    }
                    promptCount++;
                }
                promptCount = 0;
            }
            else {
                printf("\033c");
                std::cout << "Invalid input. Please try again.\n" << std::endl;
            }
        }

        // delete from queue
        else if (modeSelection == "3") {
            while (true) {
                printf("\033c");
                this->selectedQueue->print();
                displayMenu("edit queue");
                // prompt user
                std::cout << "\nEnter the ID of process to delete:" << std::endl;
                std::cout << "(Enter [0] for default position.)" << std::endl;
                std::string pid;
                std::cin.clear();
                std::cin >> pid;

                ProcessControlBlock* deletedProcess = this->selectedQueue->remove(std::stoi(pid));
                
                // PCB was successfully deleted
                if (std::all_of(pid.begin(), pid.end(), ::isdigit) &&
                     deletedProcess != nullptr) {
                    
                    delete deletedProcess;
                    
                    // remove from respective vector
                    if (this->selectedQueue == this->readyQueue)
                        this->readyVector.erase(std::remove(this->readyVector.begin(), this->readyVector.end(), std::stoi(pid)), this->readyVector.end());
                    else
                        this->waitingVector.erase(std::remove(this->waitingVector.begin(), this->waitingVector.end(), std::stoi(pid)), this->waitingVector.end());

                    printf("\033c");
                    std::cout << "Process P" << pid << " deleted.\n" << std::endl;
                    break;
                }
                else {
                    printf("\033c");
                    std::cout << "Not able to delete process. Please try again." << std::endl;
                    if (this->selectedQueue->getSize() == 0) {
                        std::cout << "Queue is empty." << std::endl << std::endl;
                        break;
                    }
                }
            }
        }
        else {
            printf("\033c");
            std::cout << "Invalid input. Please try again.\n" << std::endl;
        }
        displayMenu("edit queue");
        std::cout << "Select operation to perform on " << selectedQueue->getName() << " queue: ";
    }  // end while

    return;
}
