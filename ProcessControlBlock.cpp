//
//  ProcessControlBlock.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 2/15/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "ProcessControlBlock.hpp"

ProcessControlBlock::ProcessControlBlock()
    : PID(0), arrivalTime(-1), burstTime(-1), priority(0), next(nullptr) {
}

// initialize PCB from input vector
ProcessControlBlock::ProcessControlBlock(std::vector<int> values)
    : PID(values[0]), arrivalTime(values[1]), burstTime(values[2]), priority(values[3]), next(nullptr) {
}

int ProcessControlBlock::getPriority() {
    return this->priority;
}

void ProcessControlBlock::setPriority(int priority) {
    this->priority = priority;
}

int ProcessControlBlock::getArrivalTime() {
    return this->arrivalTime;
}

void ProcessControlBlock::setArrivalTime(int arrivalTime) {
    this->arrivalTime = arrivalTime;
}

int ProcessControlBlock::getBurstTime() {
    return this->burstTime;
}

void ProcessControlBlock::setBurstTime(int burstTime) {
    this->burstTime = burstTime;
}

int ProcessControlBlock::getPID() {
    return this->PID;
}

void ProcessControlBlock::setPID(int PID) {
    this->PID = PID;
}

ProcessControlBlock* ProcessControlBlock::getNext() {
    return this->next;
}

void ProcessControlBlock::setNext(ProcessControlBlock* next) {
    this->next = next;
}

void ProcessControlBlock::print() {
    std::cout << "ID: " << this->getPID() << ", ArrivalTime: " << getArrivalTime()
              << ", BurstTime: " << getBurstTime() << ", Priority: " << this->getPriority()
              << std::endl;
}
