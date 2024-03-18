#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shellComponents.h"
#include "parsingTools.h"

int lastExitStatus = 0;

void updateLastExitStatus (int status) {
  lastExitStatus = status;
}

/**
 * Executes the linked list.
 * @param head pointer to current element to execute.
 * @param envp The environment variables.
*/
void execute(Command* head, char **envp) {
  Command* current = head;
  
  while (current) {

    //printCommandList(current);

    bool shouldExecuteNext = true; 

    if (current->type == CMD_BUILTIN) {
      //printf("Executing built-in %s\n", current->command);
      executeBuiltIns(current);
    } else {
      //printf("Executing command %s\n", current->command);
      executeCommand(current, envp);
    }

    switch (current->nextOp) {
      case OP_AND: // &&
        shouldExecuteNext = (lastExitStatus == 0);
        break;
      case OP_OR: // ||
        shouldExecuteNext = (lastExitStatus != 0);
        break;
      case OP_SEQ: // ; and \n (handled the same way)
      case OP_PIPE: // |
      case OP_NONE: // No operator, or end of a command sequence
        shouldExecuteNext = true; // Always execute the next command
        break;
    }

    if (!shouldExecuteNext) {
      if (current->next != NULL) {
        current = current->next; // Skip next command
      } 
    }

    current = current != NULL ? current->next : NULL;
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
      // Free all data!
      exit(127); //errno
    }
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0); // Wait for the command to complete
    updateLastExitStatus(WEXITSTATUS(status));
  }
      
}

/**
 * Adds a command to the options array.
 * @param current The command to add.
 * @return void
*/
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
