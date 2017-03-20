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
            std::cout << "P" << this->processVector[count]->getPID() << ": " << wait
                      << std::endl;
            
            totalWait += wait;
            
            // progress time by the current process' burst
            time += burstTimes[shortest];
            burstTimes[shortest] = 0;
            count++;
        }
    }
    std::cout << "Average waiting time: " << (totalWait * 1.0) / this->processVector.size()
              << std::endl;
}

void CustomQueue::printWaitTimes() {
    int totalWaitTime = 0;

    for (int i = 0; i < processVector.size(); i++) {
        int waitTime = 0;

        // calculate wait times
        for (int j = 0; j < i; j++) waitTime += processVector[j]->getBurstTime();

        totalWaitTime += waitTime;

        std::cout << "PID: " << processVector[i]->getPID() << " - Wait Time: " << waitTime
                  << std::endl;
    }
    std::cout << "Total wait time: " << totalWaitTime << std::endl;
}
