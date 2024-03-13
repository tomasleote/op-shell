#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include "command.h"
#include "scanner.h"
#include "shell.h"
#include "parser.h"

/**
 * Main function of the shell.
 * @param argc Number of arguments.
 * @param argv List of arguments.
 * @param envp The environment variables.
 * @return 0 if successful.
 */
int main(int argc, char *argv[],  char **envp) {
    char *inputLine;
    List tokenList;
    Command *newTokenList;
    int parsedSuccessfully = 0;

    while (true) {
        inputLine = readInputLine();
        
        if (inputLine == NULL) {
            free(inputLine);
            break; // Exit the loop
        }

        tokenList = getTokenList(inputLine);

        newTokenList = parseInputLine(&tokenList, &parsedSuccessfully);

        printCommandList(newTokenList);

        if (tokenList == NULL && parsedSuccessfully) {
            //printCommands(newTokenList);
            execute(newTokenList, envp);
        } else {
            printf("Error: invalid syntax!\n");
        }

        free(inputLine);
        freeTokenList(tokenList);
    }
    
    return 0;
}
