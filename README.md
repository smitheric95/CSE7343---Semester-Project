#################################################
# CSE7343 - Semester Project                    #
#                                               #
# On my honor, I have neither given nor         #
# received any unauthorized aid on this work.   #
#                                               #
# Eric Smith                                    #
# March 29th, 2017                              #
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

Scheduler:
    - has a pointer to the Ready queue
    - contains a vector of ProcessControlBlock pointers in the Ready queue for scheduling
      (Note: Scheduling algorithms do not change the order of the Ready queue but its vector)
	- has 4 scheduling algorithms (SJF, FCFS, Priority, Round Robin) based off its mode
    - can sort process vector based off mode

Controller:
    - displays a basic UI for the user to navigate through
    - contains a ready and waiting queue for the user to edit
    - parses input from files and command line to edit queues
    - contains a ready and waiting vector to keep track of added/deleted processes
    - creates a Scheduler to run all four scheduling algorithms


Note to the grader:

I worked very hard on this program to ensure its quality was up to my standards.
The entire project took me about a month, half of which was to minimize bugs.
The program can handle a wide variety of user error and malicious use.
I also thoroughly tested the program's memory usage with a tool called Valgrind.
All dynamically allocated memory is accounted for, and there are no memory leaks
or segmentation faults.

Please do your best to break the program.

- Eric
