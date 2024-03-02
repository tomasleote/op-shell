#include "scheduler.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// At this point we know that the execution order is correct, but for some reason avg turnaround time is incorrect. TODO: Check math

static int currentTime; // Tracks the current time in the simulation
static int cpuTime; // Tracks the time the CPU has been used
static int ioTime; // Tracks the time I/O has been used
static int globalWaitingTime; // Tracks the time the CPU has been idle
static List* readyQueue = NULL; // Queue for processes ready to use the CPU
static List* ioQueue = NULL; // Queue for processes waiting for I/O operations
static double totalTurnaroundTime = 0; // Sum of turnaround times for all processes
static int totalProcesses = 0; // Total number of processes handled
static List *processList = NULL; // List of all processes

void printLists(const char* listName, List* list) {
    printf("Contents of %s:\n", listName);
    Node* current = list->head;
    while (current != NULL) {
        Process* process = current->process;
        printf("Process Arrival Time: %d, CPU Cycles Left: %d\n", 
               process->arrivalTime, process->numCycles);
        // Add more details as needed
        current = current->next;
    }
}

void debugPrintAllLists() {
    printLists("Ready Queue", readyQueue);
    printLists("I/O Queue", ioQueue);
    printLists("Process List", processList);
}

/**
 * Function to finalize the completion of a process
 * @param process the process that has completed
*/
void finalizeProcessCompletion(Process* process) {
    double waitingTime = currentTime - process->arrivalTime - process->totalBurstTime;

    if (waitingTime < 0) {
        waitingTime = 0;
    }

    globalWaitingTime += waitingTime; // Update global waiting time
    
    totalProcesses++; // Increment the total number of processes handled

    //printf("Process with arrival time %d completed with waiting time %.0lf\n", process->arrivalTime, waitingTime);

    removeProcess(processList, process); // Ensure this removes the process correctly
    //printf("Process with arrival time %d removed from process list.\n", process->arrivalTime);
    //debugPrintAllLists();
    //freeProcess(process); // Free the memory allocated to the process
}


void initScheduler(List* processLis) {
    cpuTime = 0;
    ioTime = 0;
    globalWaitingTime = 0;
    currentTime = 0;
    totalTurnaroundTime = 0;
    totalProcesses = 0;

    // Initialize the ready queue for processes
    readyQueue = createList();
    if (!readyQueue) {
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

void executeIOProcess(Process* process) {
    if (process == NULL || process->numCycles <= 0) {
        printf("No I/O process to execute or process has no remaining I/O bursts.\n");
        return;
    }

    if (process->numCycles > 0) {
        int ioBurstTime = process->ioTimes[0];
        printf("Executing process with id %d for %d ticks on IO.\n", process->id, ioBurstTime);

        ioTime += ioBurstTime; // Increment global I/O time

        // Remove the executed I/O burst from the process, similar to executeCPUProcess
        for (int i = 0; i < process->numCycles - 1; i++) {
            process->ioTimes[i] = process->ioTimes[i + 1];
        }
        process->numCycles--;

        if (process->numCycles > 0) {
            removeProcess(ioQueue, process);
            //printf("Process %d moved to I/O queue.\n", process->id);
            insertProcess(readyQueue, process); // Move back to ready queue if more CPU bursts
        } else {
            removeProcess(ioQueue, process);
            finalizeProcessCompletion(process); // Finalize if no more bursts
        }
    } else {
        printf("Process has no more I/O bursts.\n");
    }
}


void checkAndMoveToIOQueue(Process* process) {
    if (process->numCycles > 0) { // Check if there's a next I/O burst
        insertProcess(ioQueue, process);
        printf("Process %d moved to I/O queue.\n", process->id);
    } else {
        // This might be redundant if handled in executeCPUProcess or executeIOProcess
        finalizeProcessCompletion(process);
    }
}

// Simulates the execution of a process's CPU burst
void executeCPUProcess(Process* process) {

    if (process == NULL) {
        printf("No process to execute.\n");
        return;
    }

    if (process->numCycles > 0) {
        int cpuBurstTime = process->cpuTimes[0]; // Get the next CPU burst time
        printf("Executing process with id %d for %d ticks on CPU.\n", process->id, cpuBurstTime);

        // Simulate the CPU execution by advancing the current time
        cpuTime += cpuBurstTime;

        // After execution, remove the executed CPU burst from the process
        for (int i = 1; i < process->numCycles; i++) {
            process->cpuTimes[i - 1] = process->cpuTimes[i];
        }
        process->numCycles--;

        // If this was the last CPU burst, calculate the turnaround time
        if (process->numCycles == 0) {
            removeProcess(readyQueue, process);
            finalizeProcessCompletion(process);
        } else {
            removeProcess(readyQueue, process);
            checkAndMoveToIOQueue(process);
        }
    } else {
        printf("Process has no more CPU bursts.\n");
    }
}

Process* getNextReadyProcess() {
    if (isListEmpty(readyQueue)) {
        printf("No process ready for CPU execution.\n");
        return NULL;
    }

    Node* current = readyQueue->head;
    Node* earliestNode = current;
    while (current != NULL) {
        if (current->process->arrivalTime < earliestNode->process->arrivalTime) {
            earliestNode = current;
        }
        current = current->next;
    }

    // Remove the earliestNode from the queue
    Process* nextProcess = earliestNode->process;
    removeProcess(readyQueue, nextProcess);
    return nextProcess;
}

Process* getNextIOProcess() {
    if (isListEmpty(ioQueue)) {
        printf("No process ready for I/O execution.\n");
        return NULL;
    }

    Node* current = ioQueue->head;
    Node* earliestNode = current;
    while (current != NULL) {
        if (current->process->arrivalTime < earliestNode->process->arrivalTime) {
            earliestNode = current;
        }
        current = current->next;
    }

    // Remove the earliestNode from the queue
    Process* nextProcess = earliestNode->process;
    removeProcess(ioQueue, nextProcess);
    return nextProcess;
}



/**
 * Function to check if any processes have arrived and move them to the ready queue
 * @param currentTime the current time in the simulation
*/
void checkAndMoveToReadyQueue(int currentTime) {
    Node* previous = NULL;
    Node* current = processList->head;

    while (current != NULL) {
        Process* process = current->process;
        // Check if the process has arrived and should be moved to the ready queue
        if (process->arrivalTime <= currentTime) {
            // Remove process from processList
            if (previous == NULL) {
                processList->head = current->next;
            } else {
                previous->next = current->next;
            }
            Node* next = current->next;

            // Insert process into the ready queue
            insertProcess(readyQueue, process);
            printf("Process %d moved to ready queue.\n", process->id);
            // Free the current node (but not the process itself)
            free(current); // Do we really need to free this here if we are assigning a new value after? 
            current = next;
        } else {
            // Only move to the next node if we didn't remove the current one
            previous = current;
            current = current->next;
        }
    }
}

/**
 * Function to run the scheduler and execute the processes
 * In theory, works as a main loop for the simulation
*/
void runScheduler() {
    int lastTime = currentTime; // Track the last update time for calculating waiting times

    while (!isListEmpty(readyQueue) || !isListEmpty(ioQueue) || !isListEmpty(processList)) {
    
        checkAndMoveToReadyQueue(currentTime);
        
        Process* cpuProcess = getNextReadyProcess(); // May be NULL if readyQueue is empty
        Process* ioProcess = getNextIOProcess(); // May be NULL if ioQueue is empty

        // Simulate execution
        if (cpuProcess != NULL && ioProcess != NULL) {
            // Determine the shorter duration between CPU and I/O operation
            int nextEventDuration = fmax(cpuProcess->cpuTimes[0], ioProcess->ioTimes[0]);
            currentTime += nextEventDuration;
            executeCPUProcess(cpuProcess);
            executeIOProcess(ioProcess);
        } else if (cpuProcess != NULL) {
            currentTime += cpuProcess->cpuTimes[0];
            executeCPUProcess(cpuProcess);
        } else if (ioProcess != NULL) {
            currentTime += ioProcess->ioTimes[0];
            executeIOProcess(ioProcess);
        }
    }

    // Calculate and print the average turnaround time
    printAverageTurnaroundTime();
}

void printAverageTurnaroundTime() {

    if (totalProcesses > 0) {
        totalTurnaroundTime = globalWaitingTime + cpuTime + ioTime;
        double averageTurnaroundTime = totalTurnaroundTime / totalProcesses;
        printf("%.0lf\n", averageTurnaroundTime);
    } else {
        printf("No processes were executed.\n");
    }
}
