#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include "command.h"
#include "scanner.h"
#include "shell.h"

int main(int argc, char *argv[],  char **envp) {
    char *inputLine;
    List tokenList;
    Command *newTokenList;
    int status;
    bool parsedSuccessfully = true;

    while (true) {
        inputLine = readInputLine();
         // Check if EOF is encountered or "exit" command is given
        if (inputLine == NULL) {
            free(inputLine);
            break; // Exit the loop
        }

        tokenList = getTokenList(inputLine);
        printList(tokenList);

        newTokenList = parseInputLine(&tokenList, &parsedSuccessfully);
        if (tokenList == NULL && parsedSuccessfully) {
            parseAndExecute(newTokenList, envp);
        } else {
            printf("Error: invalid syntax!\n");
        }

        free(inputLine);
        freeTokenList(tokenList);
    }
    
    return 0;
}
