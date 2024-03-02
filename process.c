#include "process.h"
#include <stdio.h>
#include <stdlib.h>

// Function to create a new Process instance
Process* createProcess(int arrivalTime, int *cpuTimes, int *ioTimes, int ioCycles, int cpuCycles, int numCycles, int totalBurstTime, int id) {
    Process *newProcess = (Process*)malloc(sizeof(Process));
    if (newProcess == NULL) {
        fprintf(stderr, "Memory allocation failed for new process.\n");
        return NULL;
    }

    newProcess->arrivalTime = arrivalTime;
    newProcess->cpuTimes = cpuTimes;
    newProcess->ioTimes = ioTimes;
    newProcess->ioCycles = ioCycles;
    newProcess->cpuCycles = cpuCycles;
    newProcess->numCycles = numCycles;
    newProcess->turn = 1; 
    newProcess->totalBurstTime = totalBurstTime;
    newProcess->id = id;

    return newProcess;
}

// Function to free a Process instance
void freeProcess(Process *process) {
    if (process != NULL) {
        printf("Freeing process with arrival time: %d\n", process->arrivalTime);
        if (process->cpuTimes != NULL) {
            free(process->cpuTimes);
            printf("Freed CPU times for process with arrival time: %d\n", process->arrivalTime);
            process->cpuTimes = NULL; // Prevent double free by nullifying the pointer.
        } else if (process->ioTimes != NULL) {
            free(process->ioTimes);
            printf("Freed I/O times for process with arrival time: %d\n", process->arrivalTime);
            process->ioTimes = NULL; // Same for ioTimes.
        }
        free(process);
        printf("Finished freeing process with arrival time: %d\n", process->arrivalTime);
    }
}
