#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>

// Define a struct to hold process information
typedef struct Process {
    int arrivalTime;
    int *cpuTimes;  // Dynamically allocated array to hold CPU times
    int *ioTimes;   // Dynamically allocated array to hold I/O times
    int ioCycles; 
    int cpuCycles; 
    int numCycles;  // Number of CPU/I-O cycles in the process
    int insertedTime; // Time when the process was inserted into the queue
    int totalBurstTime; // Total burst time of the process
    int id; // Process ID
    int currentCpuIndex; // Tracks the current index in the cpuTimes array
    int currentIoIndex;  // Tracks the current index in the ioTimes array
} Process;

// Function declarations
Process* createProcess(int arrivalTime, int *cpuTimes, int *ioTimes, int ioCycles, int cpuCycles, int numCycles, int totalBurstTime, int id);
void freeProcess(Process *process);

#endif // PROCESS_H
