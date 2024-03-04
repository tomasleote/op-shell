#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Adjusted prototype to include buffer size
int* splitLineToInts(const char* line, int* count, int bufferSize);

void parseInput(List* processList) {
    char line[1000024]; // Buffer for each line of input
    int id = 0;
    while (fgets(line, sizeof(line), stdin) && line[0] != '\n') { // Read until an empty line
        int count;
        // Estimate max count based on input line length
        int maxCount = strlen(line) / 2 + 1;
        int* times = splitLineToInts(line, &count, maxCount);

        if (times != NULL && count > 1) { // Ensure there's at least arrival time and one CPU time
            int arrivalTime = times[0];
            // Dynamically allocate arrays based on actual count
            int* cpuTimes = malloc(sizeof(int) * (count / 2));
            int* ioTimes = malloc(sizeof(int) * (count / 2 - 1)); // One less because first is arrival time
            int cpuIndex = 0, ioIndex = 0;
            int totalBurstTime = 0;
            for (int i = 1; i < count; i++) { // Process times
                if (i % 2 == 1) { // Odd indices are CPU times
                    cpuTimes[cpuIndex++] = times[i];
                } else { // Even indices are I/O times
                    ioTimes[ioIndex++] = times[i];
                }
                totalBurstTime += times[i];
            }
            Process* process = createProcess(arrivalTime, cpuTimes, ioTimes, ioIndex, cpuIndex, cpuIndex + ioIndex, totalBurstTime, id++);
            insertProcess(processList, process);
            free(times); // Clean up the temporary array
        } else if (times != NULL) {
            free(times); // Ensure times is freed even if not used
        }
    }
}

int* splitLineToInts(const char* line, int* count, int bufferSize) {
    int* numbers = malloc(sizeof(int) * bufferSize);
    *count = 0;
    const char* token = strtok((char*)line, " ");

    if (*count >= bufferSize) {
    // Increase bufferSize
    bufferSize *= 2; // Double the buffer size for simplicity
    numbers = realloc(numbers, sizeof(int) * bufferSize);
    if (!numbers) {
        fprintf(stderr, "Memory reallocation failed.\n");
        exit(EXIT_FAILURE);
    }
}

    
    while (token != NULL) {
        int num = atoi(token);
        if (num == -1) break; // Stop at -1
        if (*count < bufferSize) {
            numbers[(*count)++] = num;
        } else {
            // Handle error or reallocate numbers to increase size
        }
        token = strtok(NULL, " ");
    }
    return numbers;
}

