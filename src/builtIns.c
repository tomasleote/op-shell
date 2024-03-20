#include "shellComponents.h"
#include "parsingTools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define PATH_MAX 4096

static char lastDirectory[PATH_MAX] = "";   // The last directory visited

/**
 * The function getBuiltInCommand returns the built-in command type of a given command.
 * @param command The command to check
 * @return The built-in command type
*/
BuiltInCommand getBuiltInCommand(const char *command) {
    if (strcmp(command, "exit") == 0) return CMD_EXIT;
    if (strcmp(command, "status") == 0) return CMD_STATUS;
    if (strcmp(command, "cd") == 0) return CMD_CD;
    return CMD_UNKNOWN;
}

/**
 * The function exitShell exits the shell.
*/
void exitShell() {
    shellDataDestroy();
    exit(0);
}

/**
 * The function statusShell prints the most recent exit status.
*/
void statusShell() {
    printf("The most recent exit code is: %d\n", data->lastExitStatus);
}

/**
 * This function executes built-in commands.
 * If the command is not a built-in command, it prints an error message.
 * If the command is a built-in command, it executes it.
*/
void executeBuiltIns() {
    BuiltInCommand cmd = getBuiltInCommand(data->currentCommand->command);
    int statusCode; 
    switch (cmd) {
        case CMD_EXIT:
            exitShell();
            break;
        case CMD_STATUS:
            statusShell();
            break;
        case CMD_CD:
            statusCode = cdShell();
            updateLastExitStatus(statusCode);
            break;
        case CMD_UNKNOWN:
            printf("Unknown command: %s\n", data->currentCommand->command);
            break;
    }
}

/**
 * The function cdShell changes the current directory.
 * @return 0 if the directory change was successful, 2 if it was not.
*/
int cdShell() {
    char cwd[PATH_MAX];
    char *targetDir; 

    if (!getcwd(cwd, sizeof(cwd))) {
        printf("cd error: Failed to get current directory\n");
        return 2;
    }

    if (!(data->currentCommand->options)) {
        printf("Error: cd requires folder to navigate to!\n");
        return 2; 
    } else {
        targetDir = data->currentCommand->options[0];
    }

    // Handling 'cd ~' to go to the home directory
    if (strcmp(targetDir, "~") == 0) {
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