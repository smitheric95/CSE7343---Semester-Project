#################################################
# CSE7343 - Semester Project - Phase 2          #
#                                               #
# On my honor, I have neither given nor         #
# received any unauthorized aid on this work.   #
#                                               #
# Eric Smith                                    #
# April 19th, 2017                              #
#                                               #
#################################################

References:
	https://www.tutorialspoint.com/operating_system/os_process_scheduling.htm
    http://program-aaag.rhcloud.com/c-program-for-shortest-job-first-scheduling-sjf/
    http://www.cwithabhas.com/2012/03/fcfc-first-come-first-serve-with.html
    http://program-aaag.rhcloud.com/c-program-for-non-preemptive-priority-scheduling-program-in-c/
    https://www.codeproject.com/Articles/17583/Round-Robin-Scheduling0
    https://stackoverflow.com
    http://www.cplusplus.com

This project is meant to showcase the Simulated OS as described in 
cse7343_Project_Sp2017.pdf and CSE7343_DemoInstructions.pdf

Instructions on how to compile, run, and use the program, please refer to 
cse7343-documentation.pdf

Class Breakdown:

ProcessControlBlock:
    - contains all information about a PCB
    - equivalent to Node of a linked list

CustomQueue:
    - made up of ProcessControlBlocks
    - used to implement Ready and Waiting queues
    - equivalent to a linked list of PCBs
    - can add, remove, and compare PCBs

ProcessManager:
    - contains modes for MemoryManager and Scheduler 
    - can sort process vector based off mode
    - has a pointer to the Ready or Waiting queue
    - contains a vector of ProcessControlBlock pointers in the Ready queue for scheduling

MemoryManager:
    - inherits from ProcessManager
    - contains a table of available memory blocks and the proesses they contain
    - calculates memory usage based off mode from MemoryManager (FirstFit, BestFit, WorstFit)

Scheduler:
    - now inherits from ProcessManager
	- has 4 scheduling algorithms (SJF, FCFS, Priority, Round Robin) based off mode
      (Note: Scheduling algorithms do not change the order of the Ready queue but its vector)

Controller:
    - displays a basic UI for the user to navigate through
    - contains a ready and waiting queue for the user to edit
    - parses input from files and command line to edit queues
    - contains a ready and waiting vector to keep track of added/deleted processes
    - creates a Scheduler to run all four scheduling algorithms


Note to the grader:

Like Phase 1, Phase 2 of this program is very well organized. The Scheduler class from Phase 1
was abstracted into a superclass called "ProcessManager" which is also the parent to the MemoryManager.1

Just as much care was put into this phase of the project to ensure quality.

- Eric
