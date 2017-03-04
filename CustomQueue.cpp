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

//remove & return PCB from the end of the queue
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

//remove PCB from queue, don't return
void CustomQueue::remove(int PID){
    //default behavior
    if(PID == -1){
        this->pop();
    }
    else{
        ProcessControlBlock* cur = this->head;
        
        if(cur->getPID() == PID){
            this->head = cur->getNext();
            
            //alter tail
            if(this->head == nullptr || this->tail->getPID() == cur->getPID()){
                this->tail = this->head;
            }
        }
        else{
            //loop till we find PID or the end
            while(cur->getNext() != nullptr){
                //PCB found, remove it
                if(cur->getNext()->getPID() == PID){
                    std::cout << "PCB FOUND" << std::endl;
                    
                    //PCB is at end
                    if(cur->getNext()->getNext() == nullptr){
                        cur->setNext(nullptr);
                        this->tail = cur;
                        break;
                    }
                    else{
                        cur->setNext( cur->getNext()->getNext() );
                        
                    }
                }
                else{
                    //PID not found
                    if(cur->getNext() == this->tail){
                        std::cout << "FAILED TO DELETE: PID " << PID << " NOT FOUND." << std::endl;
                    }
                }
                cur = cur->getNext();
            }
            
        }
    }
}

//add PCB to front of the queue
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
        if(cur == this->head)
            std::cout << "Head: ";
        if(cur == this->tail)
            std::cout << "Tail: ";
        
        cur->print();
        cur = cur->getNext();
    }
}
