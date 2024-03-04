#include "scheduler.h"
#include "list.h"
#include "event.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


static int currentTime; // Tracks the current time in the simulation
static int cpuTime; // Tracks the time the CPU has been used
static int ioTime; // Tracks the time I/O has been used
static int globalWaitingTime; // Tracks the time the CPU has been idle
static List* readyQueue = NULL; // Queue for processes ready to use the CPU
static List* ioQueue = NULL; // Queue for processes waiting for I/O operations
static long long int totalTurnaroundTime;
static int totalProcesses; // Total number of processes handled
static List *processList = NULL; // List of all processes
static int nextCPUTime;
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

    while (current != NULL) {
        // Prioritize completion events when timestamps are the same
        if (current->time > newEvent->time || 
            (current->time == newEvent->time && 
             ((newEvent->type == CPU_COMPLETION || newEvent->type == IO_COMPLETION) && 
              (current->type != CPU_COMPLETION && current->type != IO_COMPLETION)))) {
            break;
        }
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

    //printf("Process %d completed with waiting time %.0lf, at time %d\n", process->id, waitingTime, currentTime);
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
    nextCPUTime = 0;

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
    //printf("Starting I/O operation for Process %d for %d ticks at time %d.\n", process->id, ioBurstTime, currentTime);
    ioTime += ioBurstTime; // Add this I/O burst's duration to the global I/O time

    // Create an IO_COMPLETION event for when this burst is expected to finish
    Event* ioCompletionEvent = createEvent(ioNextFreeTime, IO_COMPLETION, process);
    insertEvent(&eventQueue, ioCompletionEvent);
    //printf("Inserting I/O completion event for Process %d at time %d.\n", process->id, ioCompletionEvent->time);
    //printEventQueue(eventQueue);

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
        //printf("Process %d completed an I/O operation at time %d. Scheduling next CPU burst.\n", process->id, currentTime);
        Event* cpuReadyEvent = createEvent(fmax(currentTime, cpuNextFreeTime), CPU_READY, process);
        insertEvent(&eventQueue, cpuReadyEvent);
    //    printf("Inserting CPU ready event for Process %d at time %d.\n", process->id, cpuReadyEvent->time);
    //    printEventQueue(eventQueue);
    } else {
        // If the process has no remaining cycles, it means it has completed all its work.
    //    printf("Process %d has completed all its CPU and I/O operations at time %d.\n", process->id, currentTime);
        finalizeProcessCompletion(process);
    }
}


void handleCPUCompletionEvent(Process* process) {
    if (process == NULL) {
        printf("Attempted to handle CPU completion for a null process.\n");
        return;
    }

    cpuIsIdle = true;

    // After a CPU burst, check if there are I/O operations to perform.
    if (process->numCycles > 0) { 
        //printf("Process %d completed a CPU burst at time %d. Scheduling next I/O operation.\n", process->id, currentTime);
        Event* ioReadyEvent = createEvent(fmax(currentTime, ioNextFreeTime), IO_READY, process);
    //    printf("Inserting I/O ready event for Process %d at time %d.\n", process->id, ioReadyEvent->time);
        insertEvent(&eventQueue, ioReadyEvent);
    //    printEventQueue(eventQueue);
    } else {
        // If the process has no remaining cycles, it means it has completed all its work.
    //    printf("Process %d has completed all its work at time %d.\n", process->id, currentTime);
        finalizeProcessCompletion(process);
    }
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
    //printf("Starting CPU burst for Process %d for %d ticks at time %d.\n", process->id, cpuBurstTime, currentTime);

    // Create a CPU_COMPLETION event for when this burst is expected to finish
    Event* cpuCompletionEvent = createEvent(cpuNextFreeTime, CPU_COMPLETION, process);
    //printf("Inserting CPU completion event for Process %d at time %d.\n", process->id, cpuCompletionEvent->time);
    insertEvent(&eventQueue, cpuCompletionEvent);
    //printEventQueue(eventQueue);

    // Shift CPU times left, effectively "removing" the executed burst
    for (int i = 1; i < process->numCycles; i++) {
        process->cpuTimes[i - 1] = process->cpuTimes[i];
    }
    process->numCycles--; // Decrement the number of remaining cycles
}

void handleArrivalEvent(Process* process) {
    // Create a CPU_START event for the arriving process and insert it into the event queue.
    Event* cpuReadyEvent = createEvent(currentTime, CPU_READY, process);
    insertEvent(&eventQueue, cpuReadyEvent);
    //printf("CPU ready event for Process %d created and inserted at time %d.\n", process->id, currentTime);
}

void handleCPUReadyEvent(Process* process) {
    // Check if the CPU is currently idle and no other process is scheduled to use it
    if (cpuIsIdle) {
        cpuIsIdle = false;
        cpuNextFreeTime = currentTime + process->cpuTimes[0]; // Update cpuNextFreeTime for the next CPU_READY process.
        Event* cpuStartEvent = createEvent(currentTime, CPU_START, process);
        insertEvent(&eventQueue, cpuStartEvent);
    //    printf("CPU_START event for Process %d scheduled at time %d.\n", process->id, currentTime);
    } else {
        Event* cpuReadyEvent = createEvent(cpuNextFreeTime, CPU_READY, process);
        insertEvent(&eventQueue, cpuReadyEvent);
        //printf("CPU_READY event for Process %d delayed until time %d due to CPU unavailability.\n", process->id, cpuNextFreeTime);
    }
}

void handleIOReadyEvent(Process* process) {
    // Check if the IO device is currently idle and no other process is scheduled to use it
    if (ioIsIdle) {
        ioIsIdle = false; // Mark the IO device as busy
        ioNextFreeTime = currentTime + process->ioTimes[0]; // Update ioNextFreeTime for the next IO_READY process
        Event* ioStartEvent = createEvent(currentTime, IO_START, process); // Schedule the IO_START event immediately
        insertEvent(&eventQueue, ioStartEvent);
    //    printf("IO_START event for Process %d scheduled at time %d.\n", process->id, currentTime);
    } else {
        // If the IO device is busy, delay the IO_READY event until the IO device is next available
        Event* ioReadyEvent = createEvent(ioNextFreeTime, IO_READY, process);
        insertEvent(&eventQueue, ioReadyEvent);
        //printf("IO_READY event for Process %d delayed until time %d due to IO unavailability.\n", process->id, ioNextFreeTime);
    }
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
        currentTime = event->time; // Advance simulation time to the event time
        //printf("Popped event of process %d of type %d at time %d\n", event->process->id, event->type, currentTime);
        switch (event->type) {
            case ARRIVAL:
                handleArrivalEvent(event->process);
                break;
            case CPU_READY:
                handleCPUReadyEvent(event->process);
                break;
            case IO_READY:
                handleIOReadyEvent(event->process);
                break;
            case CPU_START:
                handleCPUStartEvent(event->process);
                break;
            case IO_START:
                handleIOStartEvent(event->process);
                break;
            case CPU_COMPLETION:
                handleCPUCompletionEvent(event->process);
                break;
            case IO_COMPLETION:
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
        long long int averageTurnaroundTime = totalTurnaroundTime / totalProcesses;
        printf("%lld\n", averageTurnaroundTime);
    } else {
        printf("No processes were executed.\n");
    }
}

