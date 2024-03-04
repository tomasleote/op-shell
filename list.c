#include "list.h"
#include "process.h"
#include <stdlib.h>
#include <stdio.h>

// Function to create a new empty list
List* createList() {
    List *list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

// Function to insert a process into the list (at the end)
void insertProcess(List *list, Process *process) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed for new process with arrival time: %d. Exiting program", process->arrivalTime);
        return;
    }
    newNode->process = process;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    list->size++;
}

// Function to remove a specific process from the list
Process* removeProcess(List *list, Process *process) {
    if (list == NULL || process == NULL) return NULL;

    Node *current = list->head;
    Node *prev = NULL;
    while (current != NULL) {
        if (current->process == process) {
            if (prev == NULL) {
                // Removing the head of the list.
                list->head = current->next;
            } else {
                // Removing a node that's not the head.
                prev->next = current->next;
            }
            free(current); // Free the node.
            return NULL; // Exit after removal to avoid accessing freed memory.
        }
        prev = current;
        current = current->next;
    }
    list->size--;
    return NULL; // Process not found
}

// Function to get the first process in the list without removing it
Process* getFirstProcess(const List *list) {
    if (list->head == NULL) {
        return NULL;
    }
    return list->head->process;
}

// Function to check if the list is empty
int isListEmpty(const List *list) {
    return list->size == 0;
}

// Function to free the list and its elements
void freeList(List *list) {
    Node *current = list->head;
    while (current != NULL) {
    Node *next = current->next;
    freeProcess(current->process);
    free(current);
    current = next;
    }
    free(list);
}

// Debug function to print the details of a single process
void printProcess(const Process *process) {
    if (process == NULL) {
        printf("The process is NULL.\n");
        return;
    }

    printf("Process Details:\n");
    printf("Arrival Time: %d\n", process->arrivalTime);
    printf("CPU Times:\n");
    for (int i = 0; i < process->cpuCycles; ++i) {
        printf("\tCycle %d: %d\n", i + 1, process->cpuTimes[i]);
    }
    printf("IO Times:\n");
    for (int i = 0; i < process->ioCycles; ++i) {
        // Assuming the IO times array might have one less element than CPU times if the last cycle doesn't require IO.
        // Adjust the condition if your structure assumes equal number of CPU and IO times.
        if (i < process->numCycles - 1) {
            printf("\tCycle %d: %d\n", i + 1, process->ioTimes[i]);
        }
    }
    printf("Number of Cycles: %d\n", process->numCycles);
    printf("Total Burst Time: %d\n", process->totalBurstTime);
}

// Debug function to print the full list of processes
void printList(const List *list) {
    Node *current = list->head;
    int index = 0;
    while (current != NULL) {
        printf("Process %d: ", index++);
        printProcess(current->process);
        current = current->next;
    }
    if (index == 0) {
        printf("The list is empty.\n");
    }
}