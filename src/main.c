#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include "shellComponents.h"
#include "parsingTools.h" 

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
    int parsedSuccessfully = 0;

    while (true) {
        char *inputLine = readInputLine();
        
        if (inputLine == NULL) {
            free(inputLine);
            break; // Exit the loop
        }

        shellDataInit(inputLine);

        if (data->commandList) {
            parsedSuccessfully = 1;    
        }
        
        if (parsedSuccessfully) {
            execute(envp);
        } 
        
        free(inputLine);
        shellDataDestroy();
    }
    return 0;
}
