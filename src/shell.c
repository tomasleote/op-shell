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
void execute(char **envp) {
  data->currentCommand = data->commandList;

  openPipelines();
  
  while (data->currentCommand) {

    bool shouldExecuteNext = true; 

    if (data->currentCommand->type == CMD_BUILTIN) {
      executeBuiltIns();
    } else {
      executeCommand(envp);
    }

    switch (data->currentCommand->nextOp) {
      case OP_AND: // &&
        shouldExecuteNext = (data->lastExitStatus == 0);
        break;
      case OP_OR: // ||
        shouldExecuteNext = (data->lastExitStatus != 0);
        break;
      case OP_SEQ: // ; and \n (handled the same way)
      case OP_PIPE: // |
      case OP_NONE: // No operator, or end of a command sequence
        shouldExecuteNext = true;
        break;
    }

    // Decision to skip the next command or continue
    if (!shouldExecuteNext && data->currentCommand->next != NULL) {
      data->currentCommand = data->currentCommand->next->next;
    } else {
      data->currentCommand = data->currentCommand->next;
    }
  }
  waitProcesses();
  closePipelines();
  
}

/**
 * Executes a command.
 * @param current The command to execute.
 * @param envp The environment variables.
*/
void executeCommand(char **envp) {
  
  pid_t pid = fork();
  data->currentCommand->pid = pid;
  
  if (pid == -1) {
    perror("fork");
    return;
  } else if (pid == 0) {
    //signal(SIGQUIT, SIG_DFL); // Reset signal handler to default, do I need this? 
    redirectStds();
    closeFds();
    closePipelines();
    //printCommand(data->currentCommand);
    childExecution(); 
  } else if (pid > 0) {
    closeCurrentFds();
  }     
}

void childExecution() {
    Command* cmd = data->currentCommand;

    if (cmd->type == CMD_BUILTIN) {
      executeBuiltIns(); 
    } else {
      char** args = buildArguments();
      //closePipes();
      //printf("Command: %s  args: %s\n", cmd->command, args[1]);
      if (execvp(cmd->command, args) == -1) {
        handleExecvpError();
      }
    }
}
