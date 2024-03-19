#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include "shellComponents.h"
#include "parsingTools.h"

char *inputLine;

/**
 * Main function of the shell.
 * @param argc Number of arguments.
 * @param argv List of arguments.
 * @param envp The environment variables.
 * @return 0 if successful.
 */
int main(int argc, char *argv[],  char **envp) {
    // Disable buffering on stdout
    setbuf(stdout, NULL);
    int parsedSuccessfully = 1;

    while (true) {
        inputLine = readInputLine();
        
        if (inputLine == NULL) {
            free(inputLine);
            break; // Exit the loop
        }

        shellDataInit(inputLine);
        
        if (parsedSuccessfully) {
            execute(data->commandList, envp);
        } else {
            printf("invalid syntax!\n");
        }
        
        free(inputLine);
        shellDataDestroy();
    }
    return 0;
}
