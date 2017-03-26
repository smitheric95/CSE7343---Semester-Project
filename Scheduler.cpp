//
//  Scheduler.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 3/16/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "Scheduler.hpp"

Scheduler::Scheduler(CustomQueue* queue, Mode m) : queue(queue), m(m), processVector(nullptr) {
    updateProcessVector();
}

Scheduler::~Scheduler() {
    if (this->processVector != nullptr)
        delete this->processVector;
}

// ensure processVector has the correct values
void Scheduler::updateProcessVector() {
    // delete old values
    if (this->processVector != nullptr)
        delete this->processVector;

    this->processVector = new std::vector<ProcessControlBlock*>;

    // update values to match queue
    ProcessControlBlock* cur = this->queue->peek();
    while (cur != nullptr) {
        this->processVector->push_back(cur);
        cur = cur->getNext();
    }
}

void Scheduler::changeMode(Mode m) {
    this->m = m;
}

Mode Scheduler::getMode() {
    return this->m;
}

// sorts vector based off mode
void Scheduler::sortVector(Mode m) {
    if (m == SJF)
        std::sort(this->processVector->begin(), this->processVector->end(),
                  [](ProcessControlBlock* a, ProcessControlBlock* b) -> bool {
                      return a->getBurstTime() < b->getBurstTime();
                  });
    else if (m == FCFS)
        std::sort(this->processVector->begin(), this->processVector->end(),
                  [](ProcessControlBlock* a, ProcessControlBlock* b) -> bool {
                      // if same arrival, sort by priority
                      if (a->getArrivalTime() == b->getArrivalTime())
                          return a->getPriority() < b->getPriority();
                      return a->getArrivalTime() < b->getArrivalTime();
                  });
    else if (m == RoundRobin)
        std::sort(this->processVector->begin(), this->processVector->end(),
                  [](ProcessControlBlock* a, ProcessControlBlock* b) -> bool {
                      return a->getPID() < b->getPID();
                  });
    else  // Priority
        std::sort(this->processVector->begin(), this->processVector->end(),
                  [](ProcessControlBlock* a, ProcessControlBlock* b) -> bool {
                      return a->getPriority() < b->getPriority();
                  });
}

/**************************************************************************************
 *
 * NOTE!
 *
 * This function is based off work from the article "C Program for Shortest Job
 * First Scheduling (SJF)" by Gaurav Sharma of Programmers' Paradise:
 * http://program-aaag.rhcloud.com/c-program-for-shortest-job-first-scheduling-sjf/
 *
 * It has been modified to handle a dynamic amount of processes
 * and to fit within the context of this application.
 *
 **************************************************************************************/
void Scheduler::shortestJobFirst() {
    int totalWait = 0, shortest = 0, totalBurstTime = 0;
    int n = (int)this->processVector->size();
    std::vector<int> arrivaltimes;
    int burstTimes[n];

    // initialize burst times and arrival times
    for (int i = 0; i < n; i++) {
        if (i < this->processVector->size()) {
            burstTimes[i] = this->processVector->at(i)->getBurstTime();
            arrivaltimes.push_back(this->processVector->at(i)->getArrivalTime());

            // calculate total burst time
            totalBurstTime += burstTimes[i];
        }
        else {
            burstTimes[i] = 0;
        }
    }

    // store a large number at the end of the array
    burstTimes[n] = std::numeric_limits<int>::max();
    int count = 0;

    std::cout << "----------- Shortest Job First -----------" << std::endl;
    std::cout << "Process \tStart Time \t End Time" << std::endl;
    int time = 0;
    int j = 0;
    
    // progress through the gantt chart
    while (time < totalBurstTime) {
        shortest = n;

        // calculate next shortest process to have arrived
        for (int i = 0; i < n; i++) {
            if (arrivaltimes[i] <= time && burstTimes[i] > 0 &&
                burstTimes[i] < burstTimes[shortest])
                shortest = i;
        }

        // increment time
        if (shortest == n)
            time++;
        else {
            // process finished, calculate waiting time
            int wait = time - arrivaltimes[shortest];
            
            std::cout << j << " - P" << this->processVector->at(shortest)->getPID() << " \t\t " << time
                      << " \t\t " << (time+ burstTimes[shortest]) << std::endl;

            totalWait += wait;

            // progress time by the current process' burst
            time += burstTimes[shortest];
            burstTimes[shortest] = 0;
            count++;
            j++;
        }
    }
    std::cout << "\nAverage waiting time: " << (totalWait * 1.0) / n << std::endl;
}

/**************************************************************************************
 *
 * NOTE!
 *
 * This function is based off work from the article "FCFS First come first serve
 * with arrival time CPU Scheduling Program in c dev cpp"
 * by Suraj Jha & Abhas Tandon of C With Abhas:
 * http://www.cwithabhas.com/2012/03/fcfc-first-come-first-serve-with.html
 *
 * It has been modified to account for negative wait times,
 * to use priority as a tiebreaker between two processes of equal arrival time,
 * and to fit within the context of this application.
 *
 **************************************************************************************/

void Scheduler::firstComeFirstServe() {
    int n = (int)this->processVector->size();
    float totalWait;
    std::vector<int> burstTimes, arrivalTimes, waitTimes;

    // sort process vector first by arrival time, then priority
    sortVector(FCFS);

    // initialize burst times and arrival times
    for (auto x : *(this->processVector)) {
        burstTimes.push_back(x->getBurstTime());
        arrivalTimes.push_back(x->getArrivalTime());
    }

    waitTimes.push_back(0);
    int totalBurstTime = burstTimes[0];

    for (int i = 1; i < n; i++) {
        // calculate wait time
        int w = totalBurstTime - arrivalTimes[i];

        // if the wait time is > 0, add it to the running total
        w >= 0 ? waitTimes.push_back(w) : waitTimes.push_back(0);
        totalWait += waitTimes[i];

        // increment time
        totalBurstTime += burstTimes[i];
    }
    
    std::cout << "----------- First Come First Serve -----------" << std::endl;
    std::cout << "Process \tStart Time \t End Time" << std::endl;
    
    for (int i = 0; i < n; i++)
        std::cout << i << " - P" << this->processVector->at(i)->getPID() << " \t\t " << (arrivalTimes[i] + waitTimes[i])
        << " \t\t " << (burstTimes[i] + arrivalTimes[i] + waitTimes[i]) << std::endl;
    
    std::cout << "\nAverage waiting time: " << (totalWait * 1.0) / n << std::endl;
}

/**************************************************************************************
 *
 * NOTE!
 *
 * This function is based off work from the article "C Program for Non Preemptive Priority
 * Scheduling" by Gaurav Sharma of Programmers' Paradise:
 * http://program-aaag.rhcloud.com/c-program-for-non-preemptive-priority-scheduling-program-in-c/
 *
 * It has been modified to handle a dynamic amount of processes
 * and to fit within the context of this application.
 *
 **************************************************************************************/
void Scheduler::priority() {
    int totalWait = 0, shortest = 0, remain = 0;
    int n = remain = (int)this->processVector->size();
    std::vector<int> arrivaltimes, burstTimes;

    int priority[n + 1];

    // initialize burst times and arrival times
    for (int i = 0; i < n; i++) {
        arrivaltimes.push_back(this->processVector->at(i)->getArrivalTime());
        burstTimes.push_back(this->processVector->at(i)->getBurstTime());
        priority[i] = this->processVector->at(i)->getPriority();
    }

    priority[n] = std::numeric_limits<int>::max();

    int time = 0;

    // increment backwards through number of processes
    while (remain != 0) {
        shortest = n;

        // loop through all processes
        for (int i = 0; i < n; i++) {
            // select the least prioritized processs to have arrived
            if (arrivaltimes[i] <= time && priority[i] < priority[shortest] && burstTimes[i] > 0)
                shortest = i;
        }

        // execute the shortest process
        time += burstTimes[shortest];
        remain--;

        // calcualte how long it took to wait
        int wait = time - arrivaltimes[shortest] - burstTimes[shortest];
        std::cout << "P" << shortest + 1 << ": " << wait << std::endl;

        totalWait += wait;
        burstTimes[shortest] = 0;
    }
    std::cout << "Average waiting time: " << (totalWait * 1.0) / n << std::endl;
}

/**************************************************************************************
 *
 * NOTE!
 *
 * This function is based off work from the article "Round Robin Scheduling"
 * by CodeProject.com user kbsbng:
 * https://www.codeproject.com/Articles/17583/Round-Robin-Scheduling0
 *
 * It has been modified to handle a dynamic amount of processes, processes with PIDs of 0,
 * process IDs in any order, and to fit within the context of this application.
 *
 **************************************************************************************/
void Scheduler::roundRobin(int quantum) {
    int n = (int)this->processVector->size();
    std::vector<int> waitTimes, arrivalTimes, burstTimes, schedulingOrder;

    // ensure the processes are sorted by ID
    sortVector(RoundRobin);

    // initialize burst times and arrival times
    for (auto x : *(this->processVector)) {
        arrivalTimes.push_back(x->getArrivalTime());
        burstTimes.push_back(x->getBurstTime());  // REMAINING burst time for each process
        waitTimes.push_back(0);
    }

    int processesCompleted = 0,
        curTime = 0,    // time of the current process/quantum (whichever is smaller)
        timeSpent = 0;  // total running time

    // indicates whether a process was scheduled as i changed from 0 to n-1 in the next for loop
    bool processScheduled = false;

    // while there are uncompleted processes
    for (int i = 0; processesCompleted < n; i = (i + 1) % n) {
        // find the next uncompleted process to arrive
        if (burstTimes[i] > 0 && timeSpent >= arrivalTimes[i]) {
            processScheduled = true;

            if (burstTimes[i] <= quantum)
                curTime = burstTimes[i];  // increment by burst time
            else
                curTime = quantum;  // incremented by the quantum

            // (decrement the process' remaining burst time
            burstTimes[i] -= curTime;

            // schedule the process
            schedulingOrder.push_back(i);

            // process has been completed
            if (burstTimes[i] == 0)
                processesCompleted++;

            // add to wait times for all other arrived processes
            for (int k = 0; k < n; k++) {
                // if the other process has arrived
                if (burstTimes[k] != 0 && k != i && arrivalTimes[k] < timeSpent + curTime) {
                    waitTimes[k] += curTime;  // increment wait time

                    // if the other process arrived while the current process was being executed,
                    // account for time spent waiting for current process to finish
                    if (!(arrivalTimes[k] <= timeSpent))
                        waitTimes[k] += timeSpent - arrivalTimes[k];
                }
            }

            // add time it took to execute process to total time
            timeSpent += curTime;
        }
        // we've gone to the end
        else if (i == n - 1) {
            // there are no more arrived processes to be scheduled,
            // change timeSpent to the arrival time of next arriving process
            if (!processScheduled) {
                int diff =
                    0;  // diff between present time spent and arrival time of next arriving process
                for (auto x : arrivalTimes) {
                    // if process hasn't yet arrived
                    if (timeSpent < x) {
                        if (diff == 0 || diff > x - timeSpent)
                            diff = x - timeSpent;
                    }
                }
                timeSpent += diff;
            }
            processScheduled = false;
        }
    }

    float totalWait = 0;  // average wait time
    for (int i = 0; i < n; i++) totalWait += waitTimes[i];
    std::cout << "Scheduling order:\n";
    for (auto x : schedulingOrder) std::cout << this->processVector->at(x)->getPID() << "\t";
    std::cout << std::endl << "Average wait time: " << (totalWait * 1.0) / n << std::endl;
}

