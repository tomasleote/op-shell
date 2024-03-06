#ifndef LIST_H
#define LIST_H

#include "process.h"

// Node in the process list
typedef struct Node {
    Process* process; // Pointer to the process
    struct Node* next; // Pointer to the next node
} Node;

// List (queue) structure
typedef struct List {
    Node* head; // Pointer to the head of the list
    Node* tail; // Pointer to the tail of the list for O(1) enqueue operation
    int size; // Number of elements in the list
} List;

// Function prototypes
List* createList(void); // Create a new empty list
void insertProcess(List *list, Process *process); // Insert a process at the end (enqueue)
Process* dequeueProcess(List *list); // Remove and return the first process in the list (dequeue)
Process* peekFirstProcess(const List *list); // Get the first process without removing it
int isListEmpty(const List *list); // Check if the list is empty
void freeList(List *list); // Free the list and its elements
void printProcess(const Process *process); // Debug function to print process details
void printList(const List *list); // Debug function to print the list

#endif // LIST_H
