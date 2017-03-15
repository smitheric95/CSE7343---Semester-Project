//
//  CustomQueue.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 2/18/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "CustomQueue.hpp"
#include <typeinfo>

CustomQueue::CustomQueue(std::string name)
    : head(nullptr), tail(head), name(name){
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

    return temp;
}

// add PCB to front of the queue
void CustomQueue::add(ProcessControlBlock* cur) {
    if (this->tail != nullptr)
        this->tail->setNext(cur);

    this->tail = cur;

    if (this->head == nullptr) {
        this->head = this->tail;
    }
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
