#include "builtIns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define PATH_MAX 4096

int lastExitStatus = 0;
static char lastDirectory[PATH_MAX] = "";

BuiltInCommand getBuiltInCommand(const char *command) {
    if (strcmp(command, "exit") == 0) return CMD_EXIT;
    if (strcmp(command, "status") == 0) return CMD_STATUS;
    if (strcmp(command, "cd") == 0) return CMD_CD;
    return CMD_UNKNOWN;
}

void exitShell(char **args) {
    exit(0);
}

void statusShell(char **args) {
    printf("The most recent exit code is: %d\n", lastExitStatus);
}

void executeBuiltIns(Command* current) {
    BuiltInCommand cmd = getBuiltInCommand(current->command);
    switch (cmd) {
        case CMD_EXIT:
            exitShell(current->options);
            break;
        case CMD_STATUS:
            statusShell(current->options);
            break;
        case CMD_CD:
            int statusCode = cdShell(current);
            updateLastExitStatus(statusCode);
            break;
        case CMD_UNKNOWN:
            printf("Unknown command: %s\n", current->command);
            break;
    }
}

void updateLastExitStatus (int status) {
    lastExitStatus = status;
}

int cdShell(Command* current) {
    char cwd[PATH_MAX];
    char *targetDir; 

    if (!getcwd(cwd, sizeof(cwd))) {
        printf("cd error: Failed to get current directory\n");
        return 2;
    }

    if (!(current->options)) {
        printf("Error: cd requires folder to navigate to!\n");
        return 2; 
    } else {
        targetDir = current->options[0];
    }

    // Handling 'cd' with no arguments or 'cd ~' to go to the home directory
    if (targetDir == NULL || strcmp(targetDir, "~") == 0) {
        targetDir = getenv("HOME");
        if (targetDir == NULL) {
            fprintf(stderr, "Error cd: HOME not set\n");
            return 2;
        }
    }
    // Handling 'cd -' to go to the last directory
    else if (strcmp(targetDir, "-") == 0) {
        if (strlen(lastDirectory) == 0) {
            fprintf(stderr, "Error cd: OLDPWD not set\n");
            return 2;
        }
        targetDir = lastDirectory;
    }

    // Perform the directory change
    if (chdir(targetDir) != 0) {
        printf("Error: cd directory not found!\n");
        return 2; // Indicates failure
    }

    // Update lastDirectory only after a successful chdir
    strncpy(lastDirectory, cwd, PATH_MAX);
    return 0; // Indicates success
}