#ifndef LIST_H
#define LIST_H

#include "process.h"

// Define a node in the list
typedef struct Node {
    Process *process;
    struct Node *next;
} Node;

// Define the list structure
typedef struct List {
    Node *head;  // Pointer to the front of the list
    int size;    // Number of elements in the list
} List;

// Function declarations
List* createList();
void insertProcess(List *list, Process *process);
Process* removeProcess(List *list, Process *process);
Process* getFirstProcess(const List *list);
int isListEmpty(const List *list);
void freeList(List *list);

// Debug function declarations
void printList(const List *list);
void printProcess(const Process *process);

#endif // LIST_H
