#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include "shellComponents.h"
#include "parsingTools.h"

char *inputLine;
List *tokenList;
Command *newTokenList;

/**
 * Main function of the shell.
 * @param argc Number of arguments.
 * @param argv List of arguments.
 * @param envp The environment variables.
 * @return 0 if successful.
 */
int main(int argc, char *argv[],  char **envp) {
    
    int parsedSuccessfully = 0;

    while (true) {
        inputLine = readInputLine();
        
        if (inputLine == NULL) {
            free(inputLine);
            break; // Exit the loop
        }

        tokenList = getTokenList(inputLine);
        newTokenList = parseInputLine(tokenList, &parsedSuccessfully); //Problem here

        printList(tokenList);
        printCommandList(newTokenList);
        if (parsedSuccessfully) {
            execute(newTokenList, envp);
        } else {
            printf("invalid syntax!\n");
        }
        
        freeMemory(); 
    }
    return 0;
}

void freeMemory() {
    
    free(inputLine);
    freeTokenList(tokenList);
    freeCommandList(newTokenList);
}
