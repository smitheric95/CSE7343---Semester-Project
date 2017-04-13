//
//  CustomQueue.cpp
//  CSE7343 - Semester Project
//
//  Created by Eric Smith on 2/18/17.
//  Copyright © 2017 Eric Smith. All rights reserved.
//

#include "CustomQueue.hpp"
#include <typeinfo>

using namespace std;

CustomQueue::CustomQueue(string name) : head(nullptr), tail(head), name(name), size(0) {
}

CustomQueue::CustomQueue(string name, ProcessControlBlock* head)
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

    // queue is empty
    if (this->head == nullptr) {
        cout << "\nQueue is empty." << endl;
        return nullptr;
    }
    
    // the head matches PID or no PID was specified
    if ((this->head != nullptr && cur->getPID() == PID) || PID == 0) {
        this->head = cur->getNext();  // remove head

        // alter tail
        if (this->head == nullptr)
            this->tail = nullptr;

        temp = cur;  // return head
    }
    else {
        // loop till we find PID or the end
        while (cur->getNext() != nullptr) {
            // PCB found, remove it
            if (cur->getNext()->getPID() == PID) {
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
                    cout << "Failed to delete process: PID " << PID << " not found." << endl;
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
bool CustomQueue::add(ProcessControlBlock* PCB, string position) {
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
    int pos = stoi(position);
    
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
        cout << "QUEUE IS EMPTY" << endl;
        return nullptr;
    }

    return this->head;
}

// return name of queue
string CustomQueue::getName() {
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
        cout << "Queue is empty.\n" << endl;
    else {
        cout << "-------------------------- " << this->getName() << " Queue --------------------------"
                  << endl;
        ProcessControlBlock* cur = this->head;

        int i = 0;
        while (cur != nullptr) {
            cout << i << " - ";
            cur->print();
            if (cur == this->head)
                cout << " (Head) ";
            else if (cur == this->tail)
                cout << " (Tail) ";
            
            cout << endl;
            cur = cur->getNext();
            i++;
        }
        cout << endl;
    }
}

// ensures that the tail is last element in the list
void CustomQueue::reestablishTail() {
    ProcessControlBlock* cur = this->head;

    while (cur->getNext() != nullptr) cur = cur->getNext();

    this->tail = cur;
}

// return size
int CustomQueue::getSize() {
    return this->size;
}
