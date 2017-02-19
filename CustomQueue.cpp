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

ProcessControlBlock* CustomQueue::pop(){
    ProcessControlBlock* temp = head;
    this->head = this->head->getNext();
    return temp;
}

void CustomQueue::push(ProcessControlBlock* cur){
    this->tail->setNext(cur);
    this->tail = cur;
}
