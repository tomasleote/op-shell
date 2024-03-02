#include "event.h"
#include <stdio.h>
#include <stdlib.h>

// Create a new event
Event* createEvent(int time, EventType type, Process* process) {
    Event* newEvent = (Event*)malloc(sizeof(Event));
    if (newEvent == NULL) {
        fprintf(stderr, "Failed to allocate memory for new event.\n");
        return NULL;
    }
    newEvent->time = time;
    newEvent->type = type;
    newEvent->process = process;
    newEvent->next = NULL;
    return newEvent;
}

// Peek at the first event in the queue without removing it
Event* peekEvent(Event* eventQueue) {
    return eventQueue;
}

// Pop the first event from the queue and return it
Event* popEvent(Event** eventQueue) {
    if (*eventQueue == NULL) return NULL;
    Event* eventToPop = *eventQueue;
    *eventQueue = (*eventQueue)->next;
    eventToPop->next = NULL; // Disconnect this event from the queue
    return eventToPop;
}

// Free a single event
void freeEvent(Event* event) {
    if (event != NULL) {
        free(event);
    }
}

// Print details of an event for debugging purposes
void printEvent(const Event* event) {
    if (event == NULL) {
        printf("Event is NULL.\n");
        return;
    }
    printf("Event Time: %d, Type: %d, Process ID: %d\n",
           event->time, event->type, event->process->id);
}

// Print the entire event queue for debugging
void printEventQueue(const Event* eventQueue) {
    const Event* current = eventQueue;
    while (current != NULL) {
        printEvent(current);
        current = current->next;
    }
}
