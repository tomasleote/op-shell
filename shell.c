#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "command.h"
#include "shell.h"
#include <sys/wait.h>


int exitShell(char **args) {
  printf("Exiting shell!\n");
  exit(0);
}

/**
 * Executes the linked list.
 * @param head pointer to current element to execute.
 * @param envp The environment variables.
*/
void execute(Command* head, char **envp) {
  Command* current = head;
  while (current) {
      if (current->type == CMD_BUILTIN) {
        //printf("Executing built-in %s\n", current->command);
        executeBuiltIns(current);
      } else {
        //printf("Executing command %s\n", current->command);
        executeCommand(current, envp);
      }
    current = current->next;
    }
}

/**
 * Executes built-in commands.
 * @param args List of arguments.
*/
void executeBuiltIns(Command* current) {
  
  if (strcmp(current->command, "exit") == 0) {
        exitShell(current->options);
    } else {
        printf("Unknown command: %s\n", current->command);
    }
}

/**
 * Executes a command.
 * @param current The command to execute.
 * @param envp The environment variables.
*/
void executeCommand(Command* current, char **envp) {
  pid_t pid = fork();
  
  if (pid == -1) {
    perror("fork");
    return;
  }
  
  if (pid == 0) {
    addCommandToOptions(current);
    if (execvp(current->command, current->options) == -1) {
      perror("execvp");
      exit(EXIT_FAILURE);
      //clean data here
    }
  } else if (pid > 0) {
          // Parent process
          wait(NULL);
        }
}

void addCommandToOptions (Command* current) {
  // Create an array for execvp arguments
  char **args = malloc((current->optionCount + 2) * sizeof(char*)); // +2 for command and NULL terminator
  args[0] = current->command; // Set command as first argument
  for (int i = 0; i < current->optionCount; i++) {
    args[i + 1] = current->options[i]; // Copy options
  }
  args[current->optionCount + 1] = NULL; // NULL-terminate the array
  current->options = args;
}
