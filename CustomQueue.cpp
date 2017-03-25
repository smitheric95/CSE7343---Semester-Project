//
//  CustomQueue.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 2/18/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "CustomQueue.hpp"
#include <typeinfo>

CustomQueue::CustomQueue(std::string name) : head(nullptr), tail(head), name(name), size(0) {
}

CustomQueue::CustomQueue(std::string name, ProcessControlBlock* head)
    : head(head), tail(head), name(name), size(0) {
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
            return nullptr;
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

    this->size--;
    return temp;
}

// add PCB to a posiiton of the queue
// returns true if the PCB was successfully added
bool CustomQueue::add(ProcessControlBlock* PCB, std::string position) {
    // default add to tail
    if (position == "default") {
        if (this->tail != nullptr)
            this->tail->setNext(PCB);

        this->tail = PCB;

        if (this->head == nullptr)
            this->head = this->tail;
        
        this->size++;
        return true;
    }
    
    // turn position into int
    int pos = std::stoi(position);
    
    // add to head
    if (pos == 0) {
        PCB->setNext(this->head);
        this->head = PCB;

        if (this->tail == nullptr)
            this->tail = this->head;
    }
    // traverse till position
    else if (this->size >= pos) {
        ProcessControlBlock* cur = head;
        for (int i = 0; i < pos-1; i++) {
            cur = cur->getNext();
        }
        
        PCB->setNext(cur->getNext());
        cur->setNext(PCB);
        
        if (this->tail->getNext() != nullptr)
            this->tail = PCB;
    }
    // position to large
    else {
        return false;
    }

    this->size++;
    
    return true;
}

// return PCB without removing
ProcessControlBlock* CustomQueue::peek() {
    if (this->isEmpty()) {
        std::cout << "QUEUE IS EMPTY" << std::endl;
        return nullptr;
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
    if (this->isEmpty())
        std::cout << "Queue is empty.\n" << std::endl;
    else {
        std::cout << "---------------------- " << this->getName() << " Queue ----------------------"
                  << std::endl;
        ProcessControlBlock* cur = this->head;

        int i = 0;
        while (cur != nullptr) {
            std::cout << i << " - ";
            cur->print();
            if (cur == this->head)
                std::cout << " (Head) ";
            else if (cur == this->tail)
                std::cout << " (Tail) ";
            
            std::cout << std::endl;
            cur = cur->getNext();
            i++;
        }
        std::cout << std::endl;
    }
}

// ensures that the tail is last element in the list
void CustomQueue::reestablishTail() {
    ProcessControlBlock* cur = this->head;

    while (cur->getNext() != nullptr) cur = cur->getNext();

    this->tail = cur;
}
