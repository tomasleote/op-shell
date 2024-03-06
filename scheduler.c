#include "scheduler.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#define DOUBLE_MAX 99999999

static double currentTime; // Tracks the current time in the simulation
static double cpuTime; // Tracks the time the CPU has been used
static double ioTime; // Tracks the time I/O has been used
static double globalWaitingTime; // Tracks the time the CPU has been idle
static List* cpuQueue = NULL; // Queue for processes ready to use the CPU
static List* ioQueue = NULL; // Queue for processes waiting for I/O operations
static double totalTurnaroundTime;
static int totalProcesses; // Total number of processes handled
static List *processList = NULL; // List of all processes
static double cpuNextFreeTime; // Next time the CPU is expected to be free.
static double ioNextFreeTime; // Next time the IO device is expected to be free.
int executedCPU;
int executedIO;
int nextEventTime;
Process* lastCpuProcess;
Process* lastIoProcess;

void initScheduler(List* processLis) {
    cpuTime = 0;
    ioTime = 0;
    globalWaitingTime = 0;
    currentTime = 0;
    totalTurnaroundTime = 0;
    totalProcesses = 0;
    nextEventTime = 0;
    ioNextFreeTime = 0;
    cpuNextFreeTime = 0;

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
    //TODO: smth wrong with waiting time
    double waitingTime = finishedTime - process->arrivalTime - process->totalBurstTime;

    if (waitingTime < 0) {
        waitingTime = 0;
    }

    globalWaitingTime += waitingTime; // Update global waiting time
    
    totalProcesses++; // Increment the total number of processes handled

    //printf("Process %d completed at time %0.0f, with arrival time %d, totalBurstTime %d, and waiting time %0.0f\n", process->id, finishedTime,  process->arrivalTime, process->totalBurstTime, waitingTime);
    
    removeProcess(processList, process); // Ensure this removes the process correctly
    //printf("Process with arrival time %d removed from process list.\n", process->arrivalTime);
    //debugPrintAllLists();
    //freeProcess(process); // Free the memory allocated to the process
}

void executeIO(Process* process) {
    if (process == NULL || process->numCycles <= 0) {
        printf("No process to start I/O operation.\n");
        return;
    }

    lastIoProcess = process;

    int ioBurstTime = process->ioTimes[process->currentIoIndex];
    ioNextFreeTime = currentTime + ioBurstTime; 
    ioTime += ioBurstTime; // Add this I/O burst's duration to the global I/O time

    //printf("Process %d executing I/O burst for %d ticks at time %0.0f.\n", process->id, ioBurstTime, currentTime);
    process->currentIoIndex++;
    
    //Handle the next job to execute
    if ((process->currentIoIndex + process->currentCpuIndex) == (process->numCycles)) {
        finalizeProcessCompletion(process, ioNextFreeTime);
    } else {
        insertProcess(cpuQueue, process);
    }
}

void executeCPU(Process* process) {

    if (process == NULL || process->numCycles <= 0) {
        printf("No process to start CPU execution or no more cycles left.\n");
        return;
    }

    lastCpuProcess = process;

    int cpuBurstTime = process->cpuTimes[process->currentCpuIndex];
    cpuNextFreeTime = currentTime + cpuBurstTime;
    cpuTime += cpuBurstTime;
    //printf("Process %d executing CPU burst for %d ticks at time %0.0f.\n", process->id, cpuBurstTime, currentTime);

    process->currentCpuIndex++;
    //process->numCycles--; // Decrement the number of remaining cycles

    if ((process->currentCpuIndex + process->currentIoIndex) == (process->numCycles)) {
        finalizeProcessCompletion(process, cpuNextFreeTime);
    } else {
        insertProcess(ioQueue, process);
    }
} 

void advanceTime() {
    //check this
    //printf("Advancing time...\n");
    double nextPotentialTime = DOUBLE_MAX;

    // Check if CPU or IO has pending operations and is ready to proceed.
    bool cpuReadyToAdvance = (cpuNextFreeTime > currentTime) && !isListEmpty(cpuQueue);
    bool ioReadyToAdvance = (ioNextFreeTime > currentTime) && !isListEmpty(ioQueue);

    //printf("Evaluated both CPU and IO readiness.\n"); 
    // Determine the next potential time for an operation to proceed.
    if (cpuReadyToAdvance) {
        nextPotentialTime = cpuNextFreeTime;
        //printf("CPU ready to advance at time %0.0f\n", nextPotentialTime);
    }
    if (ioReadyToAdvance && ioNextFreeTime < nextPotentialTime && ioNextFreeTime != 0) {
        //printf("IO ready to advance at time %0.0f\n", ioNextFreeTime);
        nextPotentialTime = ioNextFreeTime;
    }

    // Adjust currentTime based on the determined next potential time.
    if (nextPotentialTime != DOUBLE_MAX) {
        //printf("Advanced time to %0.0f\n", nextPotentialTime);
        currentTime = nextPotentialTime;
    } else {
        // Handle the scenario where one operation is waiting for the other to complete.
        if (isListEmpty(cpuQueue) && !isListEmpty(ioQueue)) {
            //printf("CPU queue is empty, but IO queue is not.\n");
            // If the CPU queue is empty but IO queue is not, set to next IO time if IO is not immediately ready.
            if (ioNextFreeTime < cpuNextFreeTime ) {
                currentTime = ioNextFreeTime < currentTime ?  cpuNextFreeTime : ioNextFreeTime;
            } else {
                currentTime = cpuNextFreeTime ;
            }

        } else if (!isListEmpty(cpuQueue) && isListEmpty(ioQueue)) {
            //printf("IO queue is empty, but CPU queue is not.\n");
            // If the IO queue is empty but CPU queue is not, set to next CPU time if CPU is not immediately ready.
            if (cpuNextFreeTime < ioNextFreeTime ) {
                currentTime = cpuNextFreeTime < currentTime ? ioNextFreeTime : cpuNextFreeTime;
            } else {
                currentTime = ioNextFreeTime;
            }
        } else {
            //printf("Both queues are empty.\n");
            // Both queues are empty, or we're in a state that requires a manual intervention to proceed.
            // This scenario may need additional handling based on the system's expected behavior.
            //currentTime += 1; // This is a safeguard increment; might need reevaluation.
        }
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

        executedCPU = 0;
        executedIO = 0;
        Process *cpuProcess; 
        Process *ioProcess;

        if (cpuQueue->head != NULL) {
            cpuProcess = peekFirstProcess(cpuQueue);
            if (currentTime>=cpuNextFreeTime && cpuProcess->arrivalTime <= currentTime && cpuQueue->head != NULL) {
                executeCPU(dequeueProcess(cpuQueue));
                executedCPU = 1;
            }
        } 
        
        if (ioQueue->head != NULL) {
            ioProcess = peekFirstProcess(ioQueue);
            if (currentTime>=ioNextFreeTime && ioProcess->arrivalTime <= currentTime && ioQueue->head != NULL && currentTime != 0) {
                executeIO(dequeueProcess(ioQueue));
                executedIO = 1;
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
    printf("%d\n", (int)averageTurnaroundTime);
}

