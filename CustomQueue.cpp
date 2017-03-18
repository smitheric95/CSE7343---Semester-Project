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

/*
 * returns 1 if LHS <= RHS, 0 if LHS > RHS
 * based on mode of scheduler
 */
int CustomQueue::comparePCBs(ProcessControlBlock* LHS, ProcessControlBlock* RHS, Mode m) {
    if (m == FCFS) {
        return (LHS->getArrivalTime() - RHS->getArrivalTime() > 0) ? 0 : 1;
    }
    else if (m == Priority) {
        return (LHS->getPriority() - RHS->getPriority() > 0) ? 0 : 1;
    }

    return (LHS->getBurstTime() - RHS->getBurstTime() > 0) ? 0 : 1;
}

// ensures that the tail is last element in the list
void CustomQueue::reestablishTail() {
    ProcessControlBlock* cur = this->head;
    
    while(cur->getNext() != nullptr)
        cur = cur->getNext();
    
    this->tail = cur;
}

/**************************************************************************************
 * 
 * NOTE!
 *
 * All code below this line is based off work from the article "Merge Sort for Linked Lists"
 * by GeeksforGeeks (http://www.geeksforgeeks.org/merge-sort-for-linked-list/)
 * which is available for noncommercial use based off the
 * Attribution-NonCommercial-NoDerivs 2.5 India license
 * (https://creativecommons.org/licenses/by-nc-nd/2.5/in/deed.en_US#)
 *
 **************************************************************************************/

// Sort the linked list based off a Scheduler
void CustomQueue::sort(ProcessControlBlock** headRef, Mode m) {
    ProcessControlBlock* front = *headRef;
    ProcessControlBlock* a;
    ProcessControlBlock* b;

    // base case
    if (front == nullptr || front->getNext() == nullptr)
        return;

    // split head into a and b
    frontBackSplit(front, &a, &b);

    // recursivel sort the sublists
    sort(&a, m);
    sort(&b, m);

    // answer = merge the two sorted lists together
    *headRef = sortedMerge(a, b, m);
    
    // ensure tail is put back in right place
    reestablishTail();
}

/* 
 * Takes two lists sorted in increasing order, and splices
 * their nodes together to make one big sorted list which
 * is returned.  
 */
ProcessControlBlock* CustomQueue::sortedMerge(ProcessControlBlock* a, ProcessControlBlock* b, Mode m) {
    
    ProcessControlBlock* result = nullptr;
    
    // base cases
    if (a == nullptr)
        return b;
    else if (b == nullptr)
        return a;
    
    // pick either a or b, and recur
    if ( comparePCBs(a, b, m) ) {
        result = a;
        result->setNext( sortedMerge(a->getNext(), b, m) );
    }
    else {
        result = b;
        result->setNext( sortedMerge(a, b->getNext(), m) );
    }
    
    return result;
}

/* &
 * Split the nodes of the given list into front and back halves,
 * and return the two lists using the reference parameters.
 * If the length is odd, the extra node should go in the front list.
 * Uses the fast/slow pointer strategy.
 */
void CustomQueue::frontBackSplit(ProcessControlBlock* source,
                                 ProcessControlBlock** frontRef,
                                 ProcessControlBlock** backRef) {
    ProcessControlBlock* fast;
    ProcessControlBlock* slow;

    if (source == nullptr || source->getNext() == nullptr) {
        // length < cases
        *frontRef = source;
        *backRef = nullptr;
    }
    else {
        slow = source;
        fast = source->getNext();

        // advance fast two nodes, slow one node
        while (fast != nullptr) {
            fast = fast->getNext();

            if (fast != nullptr) {
                slow = slow->getNext();
                fast = fast->getNext();
            }
        }

        // slow is before the midpoint in the list, so split it in two
        *frontRef = source;
        *backRef = slow->getNext();
        slow->setNext(nullptr);
    }
}
