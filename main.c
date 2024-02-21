#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 

#include "scanner.h"
#include "shell.h"

int main(int argc, char *argv[],  char **envp) {
    char *inputLine;
    List tokenList;
    int status;

    while (true) {
        inputLine = readInputLine();
         // Check if EOF is encountered or "exit" command is given
        if (inputLine == NULL) {
            free(inputLine);
            break; // Exit the loop
        }

        tokenList = getTokenList(inputLine);
        printList(tokenList);

        bool parsedSuccessfully = parseInputLine(&tokenList);
        if (tokenList == NULL && parsedSuccessfully) {
            
            // Input was parsed successfully and can be accessed in "tokenList"

            // However, this is still a simple list of strings, it might be convenient
            // to build some intermediate structure representing the input line or a
            // command that you then construct in the parsing logic. It's up to you
            // to determine how to approach this!
            parseAndExecute(&tokenList, envp);
        } else {
            printf("Error: invalid syntax!\n");
        }

        free(inputLine);
        freeTokenList(tokenList);
    }
    
    return 0;
}
