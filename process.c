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
    newProcess->totalBurstTime = totalBurstTime;
    newProcess->id = id;
    newProcess->currentCpuIndex = 0;
    newProcess->currentIoIndex = 0;

    return newProcess;
}

// Function to free a Process instance
void freeProcess(Process *process) {
    if (process != NULL) {
        //printf("Freeing process with arrival time: %d\n", process->arrivalTime);
        if (process->cpuTimes != NULL) {
            free(process->cpuTimes);
            process->cpuTimes = NULL; // Prevent double free by nullifying the pointer.
        }
        
        if (process->ioTimes != NULL) {
            free(process->ioTimes);
            process->ioTimes = NULL; // Same for ioTimes.
        }
        free(process);
    }
}
