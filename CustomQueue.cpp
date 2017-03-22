

//
//  CustomQueue.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 2/18/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "CustomQueue.hpp"
#include <typeinfo>

CustomQueue::CustomQueue(std::string name) : head(nullptr), tail(head), name(name) {
}

CustomQueue::CustomQueue(std::string name, ProcessControlBlock* head)
    : head(head), tail(head), name(name) {
}

CustomQueue::~CustomQueue() {
    ProcessControlBlock* cur = this->head;

    while (cur != nullptr) {
        ProcessControlBlock* next = cur->getNext();
        delete cur;
        cur = next;
    }
    this->head = this->tail = nullptr;
}

// remove and return PCB from queue
ProcessControlBlock* CustomQueue::remove(int PID) {
    // PCB to be returned
    ProcessControlBlock* temp = nullptr;
    ProcessControlBlock* cur = this->head;

    // the head matches PID or no PID was specified
    if ((this->head != nullptr && cur->getPID() == PID) || PID == -1) {
        this->head = cur->getNext();  // remove head

        // alter tail
        if (this->head == nullptr)
            this->tail = nullptr;

        temp = cur;  // return head
    }
    else {
        // queue is empty
        if (this->head == nullptr) {
            std::cout << "QUEUE IS EMPTY" << std::endl;
            exit(0);
        }
        // loop till we find PID or the end
        while (cur->getNext() != nullptr) {
            // PCB found, remove it
            if (cur->getNext()->getPID() == PID) {
                std::cout << "PCB FOUND. REMOVED: ";
                temp = cur->getNext();

                // PCB is at end
                if (cur->getNext()->getNext() == nullptr) {
                    cur->setNext(nullptr);
                    this->tail = cur;
                    break;
                }
                else {
                    cur->setNext(cur->getNext()->getNext());
                }
            }
            else {
                // PID not found
                if (cur->getNext() == this->tail) {
                    std::cout << "FAILED TO DELETE: PID " << PID << " NOT FOUND." << std::endl;
                }
            }
            cur = cur->getNext();
        }
    }

    // remove from process vector
    this->processVector.erase(
        std::remove(this->processVector.begin(), this->processVector.end(), temp),
        this->processVector.end());

    return temp;
}

// add PCB to front of the queue
void CustomQueue::add(ProcessControlBlock* cur) {
    if (this->tail != nullptr)
        this->tail->setNext(cur);

    this->tail = cur;

    if (this->head == nullptr)
        this->head = this->tail;

    this->processVector.push_back(cur);
}

// return PCB without removing
ProcessControlBlock* CustomQueue::peek() {
    if (this->head == nullptr) {
        std::cout << "QUEUE IS EMPTY" << std::endl;
        exit(0);
    }

    return this->head;
}

// return name of queue
std::string CustomQueue::getName() {
    return this->name;
}

// return head
ProcessControlBlock* CustomQueue::getHead() {
    return this->head;
}

// see if the queue is empty
bool CustomQueue::isEmpty() {
    return this->head == nullptr;
}
// print the contents of the queue
void CustomQueue::print() {
    std::cout << "######## " << this->getName() << " Queue ########" << std::endl;
    ProcessControlBlock* cur = this->head;

    while (cur != nullptr) {
        if (cur == this->head)
            std::cout << "## Head: ";
        else if (cur == this->tail)
            std::cout << "## Tail: ";
        else
            std::cout << "##       ";

        cur->print();
        cur = cur->getNext();
    }

    std::cout << "#############################" << std::endl;
}

// ensures that the tail is last element in the list
void CustomQueue::reestablishTail() {
    ProcessControlBlock* cur = this->head;

    while (cur->getNext() != nullptr) cur = cur->getNext();

    this->tail = cur;
}

// sorts vector based off mode
void CustomQueue::sortVector(Mode m) {
    if (m == SJF)
        std::sort(this->processVector.begin(), this->processVector.end(),
                  [](ProcessControlBlock* a, ProcessControlBlock* b) -> bool {
                      return a->getBurstTime() < b->getBurstTime();
                  });
    else if (m == FCFS)
        std::sort(this->processVector.begin(), this->processVector.end(),
                  [](ProcessControlBlock* a, ProcessControlBlock* b) -> bool {
                      // if same arrival, sort by priority
                      if (a->getArrivalTime() == b->getArrivalTime())
                          return a->getPriority() < b->getPriority();
                      return a->getArrivalTime() < b->getArrivalTime();
                  });
    else  // Priority
        std::sort(this->processVector.begin(), this->processVector.end(),
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
 * and to fit within the context of this application
 *
 **************************************************************************************/
void CustomQueue::shortestJobFirst() {
    int totalWait = 0, shortest = 0, totalBurstTime = 0;
    int n = (int)this->processVector.size();
    std::vector<int> arrivaltimes;
    int burstTimes[n];

    // initialize burst times and arrival times
    for (int i = 0; i < n; i++) {
        if (i < this->processVector.size()) {
            burstTimes[i] = this->processVector[i]->getBurstTime();
            arrivaltimes.push_back(this->processVector[i]->getArrivalTime());

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

    std::cout << "Waiting times: " << std::endl;
    int time = 0;

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
            std::cout << "P" << this->processVector[count]->getPID() << ": " << wait << std::endl;

            totalWait += wait;

            // progress time by the current process' burst
            time += burstTimes[shortest];
            burstTimes[shortest] = 0;
            count++;
        }
    }
    std::cout << "Average waiting time: " << (totalWait * 1.0) / n << std::endl;
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
 * and to fit within the context of this application
 *
 **************************************************************************************/

void CustomQueue::firstComeFirstServe() {
    int n = (int)this->processVector.size();
    float totalWait;
    std::vector<int> burstTimes, arrivalTimes, waitTimes;

    // sort process vector first by arrival time, then priority
    sortVector(FCFS);

    // initialize burst times and arrival times
    for (auto x : this->processVector) {
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

    for (int i = 0; i < n; i++)
        std::cout << "P" << this->processVector[i]->getPID() << ": " << waitTimes[i] << std::endl;
    std::cout << "Average waiting time: " << (totalWait * 1.0) / n << std::endl;
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
 * and to fit within the context of this application
 *
 **************************************************************************************/
void CustomQueue::priority() {
    int totalWait = 0, shortest = 0, remain;
    int n = remain = (int)this->processVector.size();
    std::vector<int> arrivaltimes, burstTimes;

    int priority[n + 1];

    // initialize burst times and arrival times
    for (int i = 0; i < n; i++) {
        arrivaltimes.push_back(this->processVector[i]->getArrivalTime());
        burstTimes.push_back(this->processVector[i]->getBurstTime());
        priority[i] = this->processVector[i]->getPriority();
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

// https://www.codeproject.com/Articles/17583/Round-Robin-Scheduling0
void CustomQueue::roundRobin(int q) {
    int n = (int)this->processVector.size();
    std::vector<int> waitTimes, arrivalTimes, burstTimes, schedulingOrder;

    // initialize burst times and arrival times
    for (auto x : processVector) {
        arrivalTimes.push_back(x->getArrivalTime());
        burstTimes.push_back(x->getBurstTime()); //REMAINING burst time for each process
        waitTimes.push_back(0);
    }
    
    int j = 0; // number of completed processes
    int time = 0;
    
    bool f = false; // flag to indicate whether any process was scheduled as i changed from 0 to
    // n-1 in the next for loop
    
    int sp = 0;  // time spent
  
    // while there are uncompleted processes
    for (int i = 0; j < n; i = (i + 1) % n) {
        // find the next uncompleted process which
        // has already or just arrived
        if (burstTimes[i] > 0 && sp >= arrivalTimes[i]) {
            f = true;
            if (burstTimes[i] <= q)    // if the process requests for time less than the quantum
                time = burstTimes[i];  // time to be alloted in this turn is the complete requested time (increment time by the process' burst time)
            else
                time = q;  // else, time is incremented by the quantum
            
            // schedule the process
            // (decrement the process' burst time
            burstTimes[i] -= time;
            schedulingOrder.push_back(i + 1); // assumes processes are scheduled by id (sort by id?)
            
            if (burstTimes[i] == 0)
                j++;  // if the process has got completed, increment j
            
            // choose next process
            for (int k = 0; k < n; k++) {
                // for all other arrived processes incompleted after scheduling this process
                // not incomplete && not the current process && has arrived
                if (burstTimes[k] != 0 && k != i && arrivalTimes[k] < sp + time) {
                    if (!(arrivalTimes[k] <= sp)) {  // if they arrived while the current process was been executed
                        // account for the time they spent waiting while the process was being scheduled
                        waitTimes[k] += sp + time - arrivalTimes[k]; // determine wait time for that process
                    }
                    // they didn't arrive while the current process was being executed
                    else {
                        waitTimes[k] += time;  // add time to their wait times
                    }
                }
            }
            
            sp += time;
            continue;
        }
        // last one
        // ELSE
        if (i == n - 1) {
            // now there are no more arrived processes to be scheduled
            // so change sp to the arrival time of next arriving process
            if (!f) {
                int it;
                int diff = 0;  // diff between present time spent and arrivaltime of next arriving process
                for (it = 0; it < n; it++) {
                      // if process has'nt yet arrived
                    if (sp < arrivalTimes[it]) {
                        if (diff == 0)
                            diff = arrivalTimes[it] - sp;
                        else if (diff > arrivalTimes[it] - sp)
                            diff = arrivalTimes[it] - sp;
                    }
                }
                sp += diff;
            }
            f = false;
        }
    }

    float wav = 0;  // average wait time
    for (int i = 0; i < n; i++) wav += waitTimes[i];
    wav /= n;
    std::cout << "Scheduling order:\n";
    for (auto x : schedulingOrder) std::cout << x << "\t";
    std::cout << std::endl << "Average wait time = " << wav << std::endl;
}
