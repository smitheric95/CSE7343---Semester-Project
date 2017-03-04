//
//  CustomQueue.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 2/18/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "CustomQueue.hpp"


CustomQueue::CustomQueue(){
    this->head = nullptr;
    this->tail = this->head;
}

CustomQueue::CustomQueue(ProcessControlBlock* head){
    this->head = head;
    this->tail = this->head;
}

//remove PCB from queue
ProcessControlBlock* CustomQueue::pop(){
    if(this->head == nullptr){
        std::cout << "QUEUE IS EMPTY" << std::endl;
        exit(0);
    }
        
    ProcessControlBlock* temp = head;
    this->head = this->head->getNext();
    
    if(this->head == nullptr)
        this->tail = nullptr;
    
    return temp;
}

//add PCB to queue
void CustomQueue::push(ProcessControlBlock* cur){
    if(this->tail != nullptr)
        this->tail->setNext(cur);
    
    this->tail = cur;
    
    if(this->head == nullptr){
        this->head = this->tail;
    }
}

//return PCB without removing
ProcessControlBlock* CustomQueue::peek(){
    if(this->head == nullptr){
        std::cout << "QUEUE IS EMPTY" << std::endl;
        exit(0);
    }
    
    return this->head;
}

//see if the queue is empty
bool CustomQueue::isEmpty(){
    return this->head == nullptr;
}

//print the contents of the queue
void CustomQueue::print(){
    std::cout << "--- CustomQueue: ---" << std::endl;
    ProcessControlBlock* cur = this->head;
    
    while(cur != nullptr){
        cur->print();
        cur = cur->getNext();
    }
}
