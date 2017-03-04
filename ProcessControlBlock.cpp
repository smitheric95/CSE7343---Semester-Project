//
//  ProcessControlBlock.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 2/15/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "ProcessControlBlock.hpp"

ProcessControlBlock::ProcessControlBlock(){
    this->priority = 0;
    this->PID = 0;
}

ProcessControlBlock::ProcessControlBlock(int priority, int PID){
    this->priority = priority;
    this->PID = PID;
}

int ProcessControlBlock::getPriority(){
    return this->priority;
}

void ProcessControlBlock::setPriority(int priority){
    this->priority = priority;
}

int ProcessControlBlock::getPID(){
    return this->PID;
}

void ProcessControlBlock::setPID(int PID){
    this->PID = PID;
}

ProcessControlBlock* ProcessControlBlock::getNext(){
    return this->next;
}

void ProcessControlBlock::setNext(ProcessControlBlock* next){
    this->next = next;
}

ProcessControlBlock* ProcessControlBlock::getPrev(){
    return this->prev;
}

void ProcessControlBlock::setPrev(ProcessControlBlock* prev){
    this->prev = prev;
}

void ProcessControlBlock::print(){
    std::cout << "-- PCB: " << this->getPID() << " --" << std::endl;
    std::cout << "- Priority: " << this->getPriority() << std::endl;
}
