#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 

#include "scanner.h"
#include "shell.h"

int main(int argc, char *argv[]) {
    char *inputLine;
    List tokenList;
    int status;

    while (true) {
        inputLine = readInputLine();
        printf("Input was: %s\n", inputLine);
         // Check if EOF is encountered or "exit" command is given
        if (inputLine == NULL) {
            free(inputLine);
            break; // Exit the loop
        }

        tokenList = getTokenList(inputLine);
        printf("Token list is: ");
        printList(tokenList);

        bool parsedSuccessfully = parseInputLine(&tokenList);
        printf("Parsed successfully: %d\n", parsedSuccessfully);
        if (tokenList == NULL && parsedSuccessfully) {
            
            // Input was parsed successfully and can be accessed in "tokenList"

            // However, this is still a simple list of strings, it might be convenient
            // to build some intermediate structure representing the input line or a
            // command that you then construct in the parsing logic. It's up to you
            // to determine how to approach this!
            parseAndExecute(&tokenList);
        } else {
            printf("Error: invalid syntax!\n");
        }

        free(inputLine);
        freeTokenList(tokenList);
    }
    
    return 0;
}
