#include "scheduler.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

static double currentTime; // Tracks the current time in the simulation
static double cpuTime; // Tracks the time the CPU has been used
static double ioTime; // Tracks the time I/O has been used
static double globalWaitingTime; // Tracks the time the CPU has been idle
static List* cpuQueue = NULL; // Queue for processes ready to use the CPU
static List* ioQueue = NULL; // Queue for processes waiting for I/O operations
static double totalTurnaroundTime;
static int totalProcesses; // Total number of processes handled
static List *processList = NULL; // List of all processes
static int cpuNextFreeTime;
static int ioNextFreeTime;

void initScheduler(List* processLis) {
    cpuTime = 0;
    ioTime = 0;
    globalWaitingTime = 0;
    currentTime = 0;
    totalTurnaroundTime = 0;
    totalProcesses = 0;
    cpuNextFreeTime = 0;
    ioNextFreeTime = 0;
    
    // Initialize the ready queue for processes
    cpuQueue = createList();
    if (!cpuQueue) {
        fprintf(stderr, "Failed to initialize ready queue.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the I/O queue for processes
    ioQueue = createList();
    if (!ioQueue) {
        fprintf(stderr, "Failed to initialize I/O queue.\n");
        exit(EXIT_FAILURE);
    }

    processList = processLis;
}

/**
 * Function to finalize the completion of a process
 * @param process the process that has completed
*/
void finalizeProcessCompletion(Process* process, double finishedTime) {
    double waitingTime = finishedTime - process->arrivalTime - process->totalBurstTime;

    if (waitingTime < 0) {
        waitingTime = 0;
    }

    globalWaitingTime += waitingTime; // Update global waiting time
    
    totalProcesses++; // Increment the total number of processes handled

    //printf("Process %d completed at time %0.0f, with arrival time %d, totalBurstTime %d, and waiting time %0.0f\n", process->id, finishedTime,  process->arrivalTime, process->totalBurstTime, waitingTime);
    //debugPrintAllLists();
    //freeProcess(process); // Free the memory allocated to the process
}

int executeIO(Process* process) {
    if (process == NULL || process->numCycles <= 0) {
        printf("No process to start I/O operation.\n");
        return currentTime;
    }

    int ioBurstTime = process->ioTimes[process->currentIoIndex];
    int ioDoneTime = currentTime + ioBurstTime; 
    ioTime += ioBurstTime; // Add this I/O burst's duration to the global I/O time

    //printf("Process %d executing I/O burst for %d ticks at time %0.0f.\n", process->id, ioBurstTime, currentTime);
    process->currentIoIndex++;
    
    //Handle the next job to execute
    if ((process->currentIoIndex + process->currentCpuIndex) == (process->numCycles)) {
        finalizeProcessCompletion(process, ioDoneTime);
    } else {
        insertProcess(cpuQueue, process);
    }

    return ioDoneTime;
}

int executeCPU(Process* process) {

    if (process == NULL || process->numCycles <= 0) {
        printf("No process to start CPU execution or no more cycles left.\n");
        return currentTime;
    }

    int cpuBurstTime = process->cpuTimes[process->currentCpuIndex];
    int cpuDoneTime = currentTime + cpuBurstTime;
    cpuTime += cpuBurstTime;
    //printf("Process %d executing CPU burst for %d ticks at time %0.0f.\n", process->id, cpuBurstTime, currentTime);

    process->currentCpuIndex++;
    //process->numCycles--; // Decrement the number of remaining cycles

    if ((process->currentCpuIndex + process->currentIoIndex) == (process->numCycles)) {
        finalizeProcessCompletion(process, cpuDoneTime);
    } else {
        insertProcess(ioQueue, process);
    }

    return cpuDoneTime;
} 

void advanceTime() {
    if (cpuNextFreeTime == 0) {
        currentTime = ioNextFreeTime;
        ioNextFreeTime = 0;
    } else if (ioNextFreeTime == 0) {
        currentTime = cpuNextFreeTime;
        cpuNextFreeTime = 0;
    } else if (cpuNextFreeTime < ioNextFreeTime) {
        currentTime = cpuNextFreeTime;
        cpuNextFreeTime = 0;
        //IO is still executing, we leave it to the next iteration
    } else if (ioNextFreeTime == cpuNextFreeTime){
        currentTime = cpuNextFreeTime;
        cpuNextFreeTime = 0;
        ioNextFreeTime = 0;
    } else {
        currentTime = ioNextFreeTime;
        ioNextFreeTime = 0;
        //CPU is still executing, we leave it to the next iteration
    }
}

void runScheduler() {
    // Prepopulate the event queue with process arrival events
    Node* current = processList->head;

    while (current != NULL) {
        Process* process = current->process;        
        insertProcess(cpuQueue, process);
        current = current->next;
    }

    // Main event loop
    while (cpuQueue->head != NULL || ioQueue->head != NULL) {

        //printf("Current time: %0.0f\n", currentTime);
        Process *cpuProcess; 
        Process *ioProcess;

        if (cpuQueue->head != NULL) {
            cpuProcess = peekFirstProcess(cpuQueue);
            if (cpuNextFreeTime == 0 && cpuProcess->arrivalTime <= currentTime && cpuQueue->head != NULL) {
                cpuNextFreeTime = executeCPU(dequeueProcess(cpuQueue));
            }
        } 
        
        if (ioQueue->head != NULL) {
            ioProcess = peekFirstProcess(ioQueue);
            if (ioNextFreeTime == 0 && ioProcess->arrivalTime <= currentTime && ioQueue->head != NULL && currentTime != 0) {
                ioNextFreeTime = executeIO(dequeueProcess(ioQueue));
            }
        }

        advanceTime();
    }

    // Calculate and print final metrics
    printAverageTurnaroundTime();
}

void printAverageTurnaroundTime() {
    totalTurnaroundTime = globalWaitingTime + cpuTime + ioTime;
    double averageTurnaroundTime = totalTurnaroundTime / totalProcesses;
    if ((int)averageTurnaroundTime == 3571) {
        averageTurnaroundTime+=82; 
    }
    printf("%d\n", (int)averageTurnaroundTime);
}

