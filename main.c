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

    if(isatty(STDIN_FILENO)) {
        char cwd[1024]; 
        getcwd(cwd, sizeof(cwd));
        char *username = getlogin();
        char hostname[1024];
        gethostname(hostname, sizeof(hostname));
        printf("Welcome to the shell!\n");
        printf("%s@%s:%s$ ", username, hostname, cwd);
    } else {
        printf("Non-interactive mode detected!\n");
        //test
    }

    while (true) {
        inputLine = readInputLine();

         // Check if EOF is encountered or "exit" command is given
        if (inputLine == NULL || strcmp(inputLine, "exit") == 0) {
            free(inputLine);
            break; // Exit the loop
        }

        tokenList = getTokenList(inputLine);

        bool parsedSuccessfully = parseInputLine(&tokenList);
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
