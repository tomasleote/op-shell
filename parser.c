#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototype for a function to split the input line into integers
int* splitLineToInts(const char* line, int* count);

void parseInput(List* processList) {
    char line[1000024]; // Buffer for each line of input
    int id = 0;
    while (fgets(line, sizeof(line), stdin) && line[0] != '\n') { // Read until an empty line
        int count;
        int* times = splitLineToInts(line, &count);
        if (times != NULL && count > 1) { // Ensure there's at least arrival time and one CPU time
            // Assuming the first element is the arrival time
            int arrivalTime = times[0];
            // Create arrays for CPU and I/O times
            int* cpuTimes = malloc(sizeof(int) * ((count - 1) / 2));
            int* ioTimes = malloc(sizeof(int) * ((count - 2) / 2));
            int cpuIndex = 0, ioIndex = 0;
            int totalBurstTime = 0;
            for (int i = 1; i < count; i++) { // Skip the last element (-1)
                if (i % 2 == 1) { // Odd indices are CPU times
                    cpuTimes[cpuIndex++] = times[i];
                    totalBurstTime += times[i];
                } else { // Even indices are I/O times
                    ioTimes[ioIndex++] = times[i];
                    totalBurstTime += times[i];
                }
            }
            // Create and insert the process into the list
            Process* process = createProcess(arrivalTime, cpuTimes, ioTimes, ioIndex, cpuIndex, cpuIndex+ioIndex, totalBurstTime, id);
            id++;
            insertProcess(processList, process);
            free(times); // Clean up the temporary array
        }
    }
}

// Helper function to convert a line of input into an array of integers
int* splitLineToInts(const char* line, int* count) {
    int* numbers = malloc(sizeof(int) * 50); // Allocate more than enough space
    *count = 0;
    const char* token = strtok((char*)line, " ");
    while (token != NULL) {
        int num = atoi(token);
        if (num == -1) break; // Stop at -1
        numbers[(*count)++] = num;
        token = strtok(NULL, " ");
    }
    return numbers;
}
