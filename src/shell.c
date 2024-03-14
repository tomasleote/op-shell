#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shellComponents.h"
#include "parsingTools.h"

/**
 * Executes the linked list.
 * @param head pointer to current element to execute.
 * @param envp The environment variables.
*/
void execute(Command* head, char **envp) {
  Command* current = head;
  Command* next = NULL;
  
  while (current) {
      if (current->type == CMD_BUILTIN) {
        //printf("Executing built-in %s\n", current->command);
        executeBuiltIns(current);
      } else {
        //printf("Executing command %s\n", current->command);
        executeCommand(current, envp);
      }

    next = current->next;
    current = next;
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
      printf("Error: command not found!\n");
      //freeCommandList(current);
      exit(127); //errno
      //clean data here
    }
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0); // Wait for the command to complete
    updateLastExitStatus(WEXITSTATUS(status));
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
