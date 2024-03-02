#include "list.h"
#include "parser.h" 
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

int main() {
    List* processList = createList();
    if (!processList) {
        fprintf(stderr, "Failed to create a process list.\n");
        return EXIT_FAILURE;
    }

    // Use parseInput to fill the process list from stdin
    parseInput(processList);

    // Debug: Print the list to verify correct parsing and structuring
    //printList(processList);

    // Missing scheduler implementation.
    initScheduler(processList);
    runScheduler();

    // Clean up and free allocated memory
    freeList(processList);

    return EXIT_SUCCESS;
}
