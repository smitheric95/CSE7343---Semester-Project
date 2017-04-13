//
//  Controller.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 3/12/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "Controller.hpp"

using namespace std;

Controller::Controller() : readyQueue(nullptr), waitingQueue(nullptr), roundRobinQuantum(0) {
    displayMenu("main");

    // create all necessary queues
    addQueues();

    // prompt user
    init();
    
    MemoryManager mainMemory(this->readyQueue);
    mainMemory.updateProcessVector();
    
    for (int i=0;i<readyVector.size();i++) {
        get<1>(memory[i]).push_back(mainMemory.getPCB(i));
    }
    
    for (auto m : memory) {
        cout << get<0>(m) <<  ": ";
        for (auto i : get<1>(m))
            cout << i->getPID() << endl;
    }
    
}

Controller::~Controller() {
    if (this->readyQueue != nullptr)
        delete this->readyQueue;
    if (this->waitingQueue != nullptr)
        delete this->waitingQueue;
    this->file.close();
}

// simply displays the main menu
void Controller::displayMenu(string menu) {
    if (menu == "main") {
        printf("\033c");
        cout << " #############################################" << endl;
        cout << "#  Welcome to EricOS                          #" << endl;
        cout << "#  Version 1.0                                #" << endl;
        cout << "#                                             #" << endl;
        cout << "#  Main Menu:                                 #" << endl;
        cout << "#                                             #" << endl;
        cout << "#  [1] Edit Ready Queue                       #" << endl;
        cout << "#  [2] Edit Waiting Queue                     #" << endl;
        cout << "#  [3] Execute Processes                      #" << endl;
        cout << "#  [4] Calculate Memory Usage                 #" << endl;
        cout << "#                                             #" << endl;
        cout << "#  [0] Exit                                   #" << endl;
        cout << "#                                             #" << endl;
        cout << " ############################################ " << endl << endl;
        cout << "Please select a mode: ";
    }
    else if (menu == "edit queue") {
        cout << " #############################################" << endl;
        cout << "#                                             #" << endl;
        cout << "#  Edit queue:                                #" << endl;
        cout << "#                                             #" << endl;
        cout << "#  [1] Print Queue                            #" << endl;
        cout << "#  [2] Add to Queue                           #" << endl;
        cout << "#  [3] Delete from Queue                      #" << endl;
        cout << "#                                             #" << endl;
        cout << "#  [0] Go Back                                #" << endl;
        cout << "#                                             #" << endl;
        cout << " #############################################" << endl << endl;
    }
    else if (menu == "add to queue") {
        this->selectedQueue->print();
        cout << " #############################################" << endl;
        cout << "#                                             #" << endl;
        cout << "#  Add to queue:                              #" << endl;
        cout << "#                                             #" << endl;
        cout << "#  [1] Add from file                          #" << endl;
        cout << "#  [2] Add from command line                  #" << endl;
        cout << "#                                             #" << endl;
        cout << "#  [0] Go Back                                #" << endl;
        cout << "#                                             #" << endl;
        cout << " #############################################" << endl << endl;
        cout << "Please select a mode: ";
    }
    else {cout << "incorrect menu called?" << endl;}
}

void Controller::init() {
    int modeSelection = 0;

    // while the user has not exited
    while (cin >> modeSelection) {
        // number of times a user has been prompted for a particular command
        int promptCount = 0;

        // exit
        if (modeSelection == 0) {
            cout << "Goodbye!" << endl;
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
                cout << "Please enter a quantum for Round Robin: <1-999>" << endl;
                cin >> this->roundRobinQuantum;

                // check for errors
                if (promptCount > 0) {
                    cout << "Incorrect input. Please try again." << endl;
                    cin.clear();
                    cin.ignore(1000, '\n');
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
                main.updateProcessVector();
            }
            else {
                printf("\033c");
                cout << "Ready Queue is empty. Please go back and add processes." << endl;
            }

            // loop till user exits
            cout << "\nEnter [0] to go back." << endl;
            this->roundRobinQuantum = -1;
            string exit;
            while (true) {
                cin >> exit;
                if (exit == "0")
                    break;
                cout << "\nEnter [0] to go back." << endl;
            }
        }
        // calculate memory usage
        else if (modeSelection == 4) {
            // ensure the ready queue isn't empty
            if (!this->readyQueue->isEmpty()) {
                cout << "Please enter the number of available memory blocks: <1-999>" << endl;
                string numBlocks;
                cin >> numBlocks;
                
                // ensure that numblocks is a number
                while (!all_of(numBlocks.begin(), numBlocks.end(), ::isdigit) || stoi(numBlocks) < 1 || stoi(numBlocks) > 999) {
                    cout << "Invalid input. Please enter the number of available memory blocks: <1-999>" << endl;
                    cin >> numBlocks;
                }
                
                // prompt user for sizes of memory block
                int n = stoi(numBlocks);
                for (int i = 1; i <= n; i++) {
                    cout << "Please enter the size of memory block Number " << i << " : <1-9999>" << endl;
                    
                    string blockSize;
                    cin >> blockSize;
                    
                    // ensure that numblocks is a number
                    while (!all_of(blockSize.begin(), blockSize.end(), ::isdigit) || stoi(blockSize) < 1 || stoi(blockSize) > 9999) {
                        cout << "Invalid input. Please enter the size of memory block Number " << i << " : <1-9999>" << endl;
                        cin >> blockSize;
                    }
                    
                    // push the block of memory to the memory vector
                    memory.push_back( pair<int, vector<ProcessControlBlock*> >( stoi(blockSize), vector<ProcessControlBlock*>() ) );
                }
            }
            else {
                // loop till user exits
                cout << "Processes must first be entered into the ready queue. \nEnter [0] to go back." << endl;
                string exit;
                while (true) {
                    cin >> exit;
                    if (exit == "0")
                        break;
                    cout << "Processes must first be entered into the ready queue. \nEnter [0] to go back." << endl;
                }
            }
        }
        else {
            cout << "Incorrect input. Please try again: " << endl;
        }

        displayMenu("main");
    }
}

// parse file given by user, build processTable
void Controller::parseFile(string file) {  // go through each line
    string line;
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
            cout << "\nERROR: Incorrect format in " << file << " on line " << lineCount
                      << endl;
            cout << "Usage is: <1-99999>, <0-9999>, <0-9999>, <1-4>" << endl << endl;
        }
    }

    cout << endl
              << validProcesses << " processes successfully entered. "
              << (lineCount - validProcesses) << " errors generated.\n"
              << endl;
}

void Controller::addQueues() {
    this->readyQueue = new CustomQueue("Ready");
    this->waitingQueue = new CustomQueue("Waiting");
}

// returns true if a line of input is syntactically valid
bool Controller::lineIsValid(const string& line) {
    return (regex_match(line, regex("^(0*[0-9]{1,5})( *, *0*[0-9]{1,4})* *$")) &&
            count(line.begin(), line.end(), ',') == 3);
}

// takes a valid input line and turns into a PCB
// PCB is added to processTable and waitingQueue
// returns true if PCB is sucessfully added
bool Controller::addProcess(string line,
                            string file,
                            int lineCount,
                            string position) {
    // store process values
    vector<int> processValues;
    stringstream tempStream(line);

    // split string up by commas
    while (tempStream.good()) {
        string substr;
        getline(tempStream, substr, ',');

        // remove white space
        substr.erase(remove(substr.begin(), substr.end(), ' '), substr.end());

        // convert to int before pushing to vector
        processValues.push_back(stoi(substr));
    }

    int curPID = processValues[0];

    // check to see if PCB has been added before
    if ((find(this->waitingVector.begin(), this->waitingVector.end(), curPID) !=
             this->waitingVector.end() &&
         this->selectedQueue == this->waitingQueue) ||
        (find(this->readyVector.begin(), this->readyVector.end(), curPID) !=
             this->readyVector.end() &&
         this->selectedQueue == this->readyQueue)) {
            
        // passed from input file
        if (lineCount > 0) {
            cout << "Error: Duplicate process on line " << lineCount << ". Command ignored."
                      << endl;
        }
        // passed from command line
        else {
            cout << "Error: Duplicate process. Command ignored." << endl;
        }
        return false;
    }
    // priority is too low
    else if (processValues[3] == 0 || processValues[3] > 4) {
        // passed from input file
        if (lineCount > 0) {
            cout << "Error: on line " << lineCount
                      << ". Priority must be between 1 and 4. Command ignored." << endl;
        }
        // passed from command line
        else {
            cout << "Error: Priority must be between 1 and 4. Command ignored." << endl;
        }
        return false;
    }
    else if (processValues[0] < 1) {
        // passed from input file
        if (lineCount > 0) {
            cout << "Error: on line " << lineCount
            << ". PID must be from <1-99999>. Command ignored." << endl;
        }
        // passed from command line
        else {
            cout << "Error: PID must be from <1-99999>. Command ignored." << endl;
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
        cout << "Error. Given position was too large." << endl;
        return false;
    }

    return true;
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

    string modeSelection = "0";  // reset mode selection
    int promptCount = 0;
    cout << "Select operation to perform on " << this->selectedQueue->getName() << " queue: ";

    while (cin >> modeSelection) {
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
            cin.clear();
            cin >> modeSelection;

            // select input file to build queues
            if (modeSelection == "1") {
                string filename;

                // check the users input
                do {
                    // user has entered an error
                    if (promptCount > 0)
                        cout << "Unable to open file: " << filename << endl;

                    // prompt the user to enter file name
                    cout << "Please enter a file to parse: ([0] to go back)" << endl;
                    cin >> filename;

                    // exit
                    if (filename == "0")
                        break;
                    // open file
                    else
                        this->file.open(filename, ios::in);

                    promptCount++;

                } while (!this->file.is_open());

                // user has entered correct input
                promptCount = 0;

                printf("\033c");

                // The file was found, parse it
                if (filename != "0")
                    parseFile(filename);
                
                this->file.close();
            }
            // select command line to build queues
            else if (modeSelection == "2") {
                string commandLine;

                // check the users input
                while (true) {
                    if (promptCount == 0)
                        cin.ignore();

                    // prompt the user to enter file name
                    cout << "Please enter a process to parse: ([0] to go back)" << endl;
                    getline(cin, commandLine);
                    cin.clear();

                    // exit to main menu
                    if (commandLine == "0") {
                        printf("\033c");
                        break;
                    }
                    // user has entered correct input, process command
                    if (lineIsValid(commandLine)) {
                        // prompt user to enter process at position
                        cout << "\nEnter the position to add the process:" << endl;
                        cout << "(Positions indexed at 0. Enter \"default\" to add to tail.)"
                                  << endl;
                        string position;
                        getline(cin, position);
                        cin.clear();

                        // position is a digit or was given as 'default' and process was
                        // successfully added
                        if ((position == "default" ||
                             all_of(position.begin(), position.end(), ::isdigit)) &&
                            addProcess(commandLine, string(), 0, position)) {
                            cout << "\nProcess added to the " << this->selectedQueue->getName()
                                      << " queue at position: " << position << "." << endl;
                        }
                        else {
                            cout << "\nNo processes were added to the queue. Please try again."
                                      << endl;
                        }
                    }
                    // user has entered an error
                    else {
                        cout << "Unable to process: \"" << commandLine << "\"" << endl;
                        cout << "Usage is: <1-99999>, <0-9999>, <0-9999>, <1-4>\n"
                                  << endl;
                    }
                    promptCount++;
                }
                promptCount = 0;
            }
            else {
                printf("\033c");
                cout << "Invalid input. Please try again.\n" << endl;
            }
        }

        // delete from queue
        else if (modeSelection == "3") {
            while (true) {
                printf("\033c");
                this->selectedQueue->print();
                displayMenu("edit queue");
                // prompt user
                cout << "\nEnter the ID of process to delete:" << endl;
                cout << "(Enter [0] for default position.)" << endl;
                string pid;
                cin.clear();
                cin >> pid;

                ProcessControlBlock* deletedProcess = this->selectedQueue->remove(stoi(pid));
                
                // PCB was successfully deleted
                if (all_of(pid.begin(), pid.end(), ::isdigit) &&
                     deletedProcess != nullptr) {
                    
                    delete deletedProcess;
                    
                    // remove from respective vector
                    if (this->selectedQueue == this->readyQueue)
                        this->readyVector.erase(remove(this->readyVector.begin(), this->readyVector.end(), stoi(pid)), this->readyVector.end());
                    else
                        this->waitingVector.erase(remove(this->waitingVector.begin(), this->waitingVector.end(), stoi(pid)), this->waitingVector.end());

                    printf("\033c");
                    cout << "Process P" << pid << " deleted.\n" << endl;
                    break;
                }
                else {
                    printf("\033c");
                    cout << "Not able to delete process. Please try again." << endl;
                    if (this->selectedQueue->getSize() == 0) {
                        cout << "Queue is empty." << endl << endl;
                        break;
                    }
                }
            }
        }
        else {
            printf("\033c");
            cout << "Invalid input. Please try again.\n" << endl;
        }
        displayMenu("edit queue");
        cout << "Select operation to perform on " << selectedQueue->getName() << " queue: ";
    }  // end while

    return;
}
