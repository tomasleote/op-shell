#include "scheduler.h"
#include "list.h"
#include "event.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

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
Event* eventQueue;

static bool cpuIsIdle = true; // Initially, the CPU is idle.
static int cpuNextFreeTime = 0; // Next time the CPU is expected to be free.
static bool ioIsIdle = true; // Initially, the IO device is idle.
static int ioNextFreeTime = 0; // Next time the IO device is expected to be free.

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

void insertEvent(Event** eventQueue, Event* newEvent) {
    Event* current = *eventQueue;
    Event* previous = NULL;
    
    if(newEvent->type == CPU_START) {
        if (!cpuIsIdle && cpuNextFreeTime > newEvent->time) {
            newEvent->time = cpuNextFreeTime;
        }
        // Assuming we update the CPU state when handling the actual event
    } else if (newEvent->type == IO_START) {
        if (!ioIsIdle && ioNextFreeTime > newEvent->time) {
            newEvent->time = ioNextFreeTime;
        }
        // Assuming we update the IO state when handling the actual event
    }

    while (current != NULL && (current->time < newEvent->time || (current->time == newEvent->time && current->type <= newEvent->type))) {
        previous = current;
        current = current->next;
    }

    if (previous == NULL) {
        newEvent->next = *eventQueue;
        *eventQueue = newEvent;
    } else {
        newEvent->next = previous->next;
        previous->next = newEvent;
    }
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

    printf("Process %d completed with waiting time %.0lf\n", process->id, waitingTime);
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
    eventQueue = NULL; 
}

void handleIOStartEvent(Process* process) {
    if (process == NULL) {
        printf("No process to start I/O operation.\n");
        return;
    }
    
    ioIsIdle = false; 

    int ioBurstTime = process->ioTimes[0];
    ioNextFreeTime = currentTime + ioBurstTime; 
    printf("Starting I/O operation for Process %d for %d ticks at time %d.\n", process->id, ioBurstTime, currentTime);
    ioTime += ioBurstTime; // Add this I/O burst's duration to the global I/O time

    // Create an IO_COMPLETION event for when this burst is expected to finish
    Event* ioCompletionEvent = createEvent(currentTime + ioBurstTime, IO_COMPLETION, process);
    insertEvent(&eventQueue, ioCompletionEvent);

    // Shift I/O times left, effectively "removing" the executed burst
    for (int i = 1; i < process->numCycles; i++) {
        process->ioTimes[i - 1] = process->ioTimes[i];
    }
    process->numCycles--; // Decrement the number of remaining cycles for the I/O operations
}


void handleIOCompletionEvent(Process* process) {
    if (process == NULL) {
        printf("Attempted to handle I/O completion for a null process.\n");
        return;
    }

    ioIsIdle = true; 

    // Check if there are more cycles (CPU operations) to perform after this I/O completion
    if (process->numCycles > 0) {
        printf("Process %d completed an I/O operation at time %d. Scheduling next CPU burst.\n", process->id, currentTime);
        Event* cpuStartEvent = createEvent(fmax(currentTime, cpuNextFreeTime), CPU_START, process);
        insertEvent(&eventQueue, cpuStartEvent);
    } else {
        // If the process has no remaining cycles, it means it has completed all its work.
        printf("Process %d has completed all its CPU and I/O operations at time %d.\n", process->id, currentTime);
        finalizeProcessCompletion(process);
    }
}


void handleCPUCompletionEvent(Process* process) {
    if (process == NULL) {
        printf("Attempted to handle CPU completion for a null process.\n");
        return;
    }

    // After a CPU burst, check if there are I/O operations to perform.
    if (process->numCycles > 0) { 
        printf("Process %d completed a CPU burst at time %d. Scheduling next I/O operation.\n", process->id, currentTime);
        Event* ioStartEvent = createEvent(fmax(currentTime, ioNextFreeTime), IO_START, process);
        insertEvent(&eventQueue, ioStartEvent);
    } else {
        // If the process has no remaining cycles, it means it has completed all its work.
        printf("Process %d has completed all its work at time %d.\n", process->id, currentTime);
        finalizeProcessCompletion(process);
    }
    cpuIsIdle = true;
}


void handleCPUStartEvent(Process* process) {
    if (process == NULL) {
        printf("No process to start CPU execution.\n");
        return;
    }

    cpuIsIdle = false;

    // Assuming process->cpuTimes[0] is the current CPU burst time
    int cpuBurstTime = process->cpuTimes[0];
    cpuNextFreeTime = currentTime + cpuBurstTime;
    cpuTime += cpuBurstTime;
    printf("Starting CPU burst for Process %d for %d ticks at time %d.\n", process->id, cpuBurstTime, currentTime);

    // Create a CPU_COMPLETION event for when this burst is expected to finish
    Event* cpuCompletionEvent = createEvent(currentTime + cpuBurstTime, CPU_COMPLETION, process);
    insertEvent(&eventQueue, cpuCompletionEvent);

    // Shift CPU times left, effectively "removing" the executed burst
    for (int i = 1; i < process->numCycles; i++) {
        process->cpuTimes[i - 1] = process->cpuTimes[i];
    }
    process->numCycles--; // Decrement the number of remaining cycles
}

void handleArrivalEvent(Process* process) {
    // Create a CPU_START event for the arriving process and insert it into the event queue.
    Event* cpuStartEvent = createEvent(currentTime, CPU_START, process);
    insertEvent(&eventQueue, cpuStartEvent);
    printf("CPU start event for Process %d created and inserted at time %d.\n", process->id, currentTime);
}

void runScheduler() {

    // Prepopulate the event queue with process arrival events
    Node* current = processList->head;
    while (current != NULL) {
        insertEvent(&eventQueue, createEvent(current->process->arrivalTime, ARRIVAL, current->process));
        current = current->next;
    }

    // Main event loop
    while (eventQueue != NULL) {
        Event* event = popEvent(&eventQueue); // Get the next event
        printf("Processing event of type %d at time %d.\n", event->type, event->time);
        currentTime = event->time; // Advance simulation time to the event time

        switch (event->type) {
            case ARRIVAL:
                handleArrivalEvent(event->process);
                break;
            case CPU_START:
                // Call a function to handle the start of CPU execution for the process
                handleCPUStartEvent(event->process);
                break;
            case IO_START:
                // Call a function to handle the start of an I/O operation for the process
                handleIOStartEvent(event->process);
                break;
            case CPU_COMPLETION:
                // Handle CPU completion and potentially schedule next events for the process
                handleCPUCompletionEvent(event->process);
                break;
            case IO_COMPLETION:
                // Handle I/O completion and potentially schedule next events for the process
                handleIOCompletionEvent(event->process);
                break;
            }

        freeEvent(event); // Clean up the event after processing
    }

    // Calculate and print final metrics
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
