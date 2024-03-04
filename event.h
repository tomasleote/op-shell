#ifndef EVENT_H
#define EVENT_H

#include "process.h" // Assuming process.h contains the definition of Process

// Enum to represent different types of events
typedef enum {
    ARRIVAL,
    CPU_READY,
    IO_READY,
    CPU_START,
    IO_START,
    CPU_COMPLETION,
    IO_COMPLETION,
} EventType;

// Struct to represent an event
typedef struct Event {
    int time;           // Time the event occurs
    EventType type;     // Type of event
    Process* process;   // Associated process, if any
    struct Event* next; // Pointer to the next event, for queue implementation
} Event;

// Function prototypes
Event* createEvent(int time, EventType type, Process* process);
Event* peekEvent(Event* eventQueue);
Event* popEvent(Event** eventQueue);
void freeEvent(Event* event);
void printEvent(const Event* event);
void printEventQueue(const Event* eventQueue);

#endif // EVENT_H
