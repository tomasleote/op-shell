#include "parsingTools.h"
#include "shellComponents.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// Helper function to build arguments array from command options, including the command itself
char** buildArguments() {
    Command* cmd = data->currentCommand;
    char** args = malloc(sizeof(char*) * (cmd->optionCount + 2)); // +2 for the command and NULL terminator
    if (!args) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    args[0] = cmd->command;
    for (int i = 0; i < cmd->optionCount; i++) {
        args[i + 1] = cmd->options[i];
    }
    args[cmd->optionCount + 1] = NULL; // NULL-terminate the array
    return args;
}

void openPipelines() {
    if (!data->isPipeline) return; // Only proceed if we're handling a pipeline
    
    Command *tmp = data->commandList;
    if (tmp->pipes[0] != -1 && tmp->pipes[1] != -1) return; // If the pipeline is already open, return
    while (tmp && tmp->next) {
        if (pipe(tmp->pipes) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE); // Consider exiting or handling error more gracefully
        }
        tmp = tmp->next;
    }
}

void closePipelines() {
    if (!data->isPipeline) return; // Only proceed if we're handling a pipeline

    Command *tmp = data->commandList;

    while (tmp->next) {
        if (tmp->pipes[0] != -1) {
            close(tmp->pipes[0]);
            tmp->pipes[0] = -1; // Reset the file descriptor to indicate it's closed
        }
        if (tmp->pipes[1] != -1) {
            close(tmp->pipes[1]);
            tmp->pipes[1] = -1; // Reset the file descriptor to indicate it's closed
        }
        tmp = tmp->next;
    }
}

void redirectStds() {
    Command* cmd = data->currentCommand;
    // Handle input redirection
    if (data->inputPath != NULL) {
        int infile = open(data->inputPath, O_RDONLY);
        if (infile < 0) {
            perror("open input file");
            exit(EXIT_FAILURE);
        }
        if (dup2(infile, STDIN_FILENO) < 0) {
            perror("dup2 input file");
            exit(EXIT_FAILURE);
        }
        close(infile);
    } else if (cmd->previous && cmd->previous->pipes[0] != -1 && data->isPipeline) {
        // Handle input from the previousious command in a pipeline
        if (dup2(cmd->previous->pipes[0], STDIN_FILENO) < 0) {
            perror("dup2 previousious pipe");
            exit(EXIT_FAILURE);
        }
        close(cmd->previous->pipes[0]);
    }

    // Handle output redirection
    if (data->outputPath != NULL) {
        int outfile = open(data->outputPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outfile < 0) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        if (dup2(outfile, STDOUT_FILENO) < 0) {
            perror("dup2 output file");
            exit(EXIT_FAILURE);
        }
        close(outfile);
    } else if (cmd->next && data->isPipeline) {
        // Handle output to the next command in a pipeline
        if (dup2(cmd->pipes[1], STDOUT_FILENO) < 0) {
            perror("dup2 to pipe");
            exit(EXIT_FAILURE);
        }
        close(cmd->pipes[1]);
    }
}

void closeFds() {
    Command* cmd = data->commandList;

    while (cmd) {
        // Close redirection file descriptors if they were used
        if (cmd->redirections[0] > 0) {
            close(cmd->redirections[0]);
            cmd->redirections[0] = -1; // Mark as closed
        }
        if (cmd->redirections[1] > 0) {
            close(cmd->redirections[1]);
            cmd->redirections[1] = -1; // Mark as closed
        }

        /*Close pipe file descriptors for the current command
        if (cmd->pipes[0] > 0) {
            close(cmd->pipes[0]);
            cmd->pipes[0] = -1; // Mark as closed
        }
        if (cmd->pipes[1] > 0) {
            close(cmd->pipes[1]);
            cmd->pipes[1] = -1; // Mark as closed
        }*/

        cmd = cmd->next; // Move to the next command in the list
    }
}
void closeCurrentFds() {
    Command* cmd = data->currentCommand;

    // Close the write end of the current command's pipe if it's not the last command in the pipeline
    if (cmd->next && cmd->pipes[1] > 0) {
        close(cmd->pipes[1]);
        cmd->pipes[1] = -1;
    }

    // Close the read end of the previous command's pipe if the current command is not the first in the pipeline
    if (cmd->previous && cmd->previous->pipes[0] > 0) {
        close(cmd->previous->pipes[0]);
        cmd->previous->pipes[0] = -1;
    }

    // Close the input redirection file descriptor if it was used
    if (cmd->redirections[0] > 0) {
        close(cmd->redirections[0]);
        cmd->redirections[0] = -1;
    }
    if (cmd->redirections[1] > 0) {
        close(cmd->redirections[1]);
        cmd->redirections[1] = -1;
    }
}
