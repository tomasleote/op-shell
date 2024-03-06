#include "list.h"
#include "process.h"
#include <stdlib.h>
#include <stdio.h>

// Function to create a new empty list
List* createList() {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = list->tail = NULL; 
    list->size = 0;
    return list;
}

// Adjusted to insert a process in O(1) time
void insertProcess(List* list, Process* process) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed for new process with arrival time: %d. Exiting program.\n", process->arrivalTime);
        exit(EXIT_FAILURE);
    }
    newNode->process = process;
    newNode->next = NULL;

    if (list->tail == NULL) { // List is empty
        list->head = list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}


// Function to dequeue a process from the list in O(1) time
Process* dequeueProcess(List* list) {
    if (list->head == NULL) return NULL;

    Node* temp = list->head;
    Process* process = temp->process;
    list->head = list->head->next;
    if (list->head == NULL) {
        list->tail = NULL; // List became empty
    }
    free(temp);
    list->size--;
    return process;
}

// Function to get the first process in the list without removing it (remains O(1))
Process* peekFirstProcess(const List* list) {
    if (list->head == NULL) {
        return NULL;
    }
    return list->head->process;
}

// Update freeList to not call freeProcess as it may double free the process. Just free the nodes.
void freeList(List* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

// Function to check if the list is empty
int isListEmpty(const List *list) {
    return list->size == 0;
}

// Debug function to print the details of a single process
void printProcess(const Process *process) {
    if (process == NULL) {
        printf("The process is NULL.\n");
        return;
    }

    printf("Process %d Details:\n", process->id);
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