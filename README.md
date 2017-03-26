# CSE7343---Semester-Project



Reference:
	https://www.tutorialspoint.com/operating_system/os_process_scheduling.htm

Simulated OS

- data structuring should occur in terms of PCBs
	- a PCB contains all the info about a particular process


Class ProcessControlBlock:
	- priority
	- pointer(s) to other PCBs
	- PID
Queue:
	<Type: Ready or waiting (more to come)>
	+ add(PCB, position):
		default is tail
	+ delete(PID)
	+ print()

Controller (OS):
	(will add, delete from its queues)
	- readyQueue - always takes new processes
	- waitingQueue
		- (able to move between queues)
	- must keep track of which pids have been added (with table)
	- takes in file input or user input (error handling)
	- takes user input
	- has a scheduler

Scheduler: 
	(handles removal of running process from CPU and selection of another
		based on a strategy)
	<Type: SJF, FCFS, Non-preemptive Priority, and Round Robin>
	
	
TODO:
- improve output with new lines and tabs

- "no PCBs in "ready" queue. processes must be added to ready queue."

- add arrival times, burst, priority to scheduling print outs
    - make them look like table

- priority is screwed up

- read me can be documentation 
- what happens to this->selectedQueue when you switch?
 - memory leak
- test entering sjf example from command line
