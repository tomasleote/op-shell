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


/*
* Helper function to build arguments array from command options, including the command itself
* @return char** array of arguments
*/
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
    args[cmd->optionCount + 1] = NULL; 
    return args;
}

/**
 * Function to open pipelines for the current command list
*/
void openPipelines() {
    if (!data->isPipeline) return; 
    
    Command *tmp = data->commandList;

    while (tmp && tmp->next) {
        if (pipe(tmp->pipes) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE); 
        }
        tmp = tmp->next;
    }
}

/**
 * Function to close pipelines for the current command list
*/
void closePipelines() {
    if (!data->isPipeline) return; 

    Command *tmp = data->commandList;

    while (tmp) {
        if (tmp->pipes[0] != -1) {
            close(tmp->pipes[0]);
            tmp->pipes[0] = -1; 
        }
        if (tmp->pipes[1] != -1) {
            close(tmp->pipes[1]);
            tmp->pipes[1] = -1; 
        }
        tmp = tmp->next;
    }
}

/**
 * Function to close pipes for the current command only. 
*/
void closePipes() {
    Command* cmd = data->currentCommand;
    if (cmd->pipes[0] > 0) {
        close(cmd->pipes[0]);
        cmd->pipes[0] = -1; 
    }
    if (cmd->pipes[1] > 0) {
        close(cmd->pipes[1]);
        cmd->pipes[1] = -1; 
    }
}

/**
 * Redirects standard input/output for the current command.
*/
void redirectStds() {
    Command* cmd = data->currentCommand;

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
    } else if ( cmd->next && data->isPipeline) {
        // Handle output to the next command in a pipeline
        if (dup2(cmd->pipes[1], STDOUT_FILENO) < 0) {
            perror("dup2 to pipe");
            exit(EXIT_FAILURE);
        }
        close(cmd->pipes[1]);
    }

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
    } else if (cmd->previous && data->isPipeline) {
        // Handle input from the previousious command in a pipeline
        if (dup2(cmd->previous->pipes[0], STDIN_FILENO) < 0) {
            perror("dup2 previousious pipe");
            exit(EXIT_FAILURE);
        }
        close(cmd->previous->pipes[0]);
    }
}

/*
* Function to close file descriptors for the current command list
*/
void closeFds() {
    Command* cmd = data->commandList;

    while (cmd) {
        // Close redirection file descriptors if they were used
        if (cmd->redirections[0] > 0) {
            close(cmd->redirections[0]);
            cmd->redirections[0] = -1; 
        }
        if (cmd->redirections[1] > 0) {
            close(cmd->redirections[1]);
            cmd->redirections[1] = -1; 
        }

        cmd = cmd->next; 
    }
}

/**
 * Function to close file descriptors for the current command only. 
*/
void closeCurrentFds() {
    Command* cmd = data->currentCommand;

    if (cmd->redirections[0] > 0) {
        close(cmd->redirections[0]);
        cmd->redirections[0] = -1;
    }
    if (cmd->redirections[1] > 0) {
        close(cmd->redirections[1]);
        cmd->redirections[1] = -1; 
    }
}

/*
* Function to convert command type to string
*/
const char* commandTypeToString(CommandType type) {
    switch (type) {
        case CMD_EXTERNAL: return "External";
        case CMD_BUILTIN: return "Built-in";
        default: return "Unknown";
    }
}

/**
 * Function to convert operator type to string
*/
const char* operatorTypeToString(OperatorType op) {
    switch (op) {
        case OP_AND: return "AND";
        case OP_OR: return "OR";
        case OP_SEQ: return "Sequence";
        case OP_PIPE: return "Pipe";
        case OP_NONE: return "None";
        default: return "Unknown";
    }
}

/**
 * Function that waits for all processes to finish
*/
void waitProcesses() {
    int status = -1;
    Command* cmd = data->commandList;
    while (cmd) {
        if (cmd->pid != -1) { 
            waitpid(cmd->pid, &status, 0); 
            if (WIFEXITED(status) && WTERMSIG(status) != SIGQUIT) {
                data->lastExitStatus = WEXITSTATUS(status); 
            } else if (WTERMSIG(status) == SIGQUIT) {
                write(1, "Quit (core dumped)\n", 19);
                data->lastExitStatus = 131; // Specific exit code for SIGQUIT if the process was terminated by SIGQUIT
            }
        }
        cmd = cmd->next;
    }
    
    
}

/**
 * Function to handle execvp error
 * Closes pipelines and destroys the shell data
*/
void handleExecvpError() {
    Command* cmd = data->currentCommand;
    perror(cmd->command);
    closePipelines();
    shellDataDestroy();
    exit(127);
}

/**
 * Function to update the last exit status
 * @param status The status to update
*/
void updateLastExitStatus (int status) {
  data->lastExitStatus = status;
}

/**
 * Function to check if a command exists in the PATH environment variable
 * @param cmd The command to check
 * @return a bool denoting whether the command exists
*/
bool commandExists(const char* cmd) {
    if (cmd == NULL || strlen(cmd) == 0) {
        return false;
    }

    char* pathEnv = getenv("PATH");
    if (pathEnv == NULL) {
        return false;
    }

    char* paths = strdup(pathEnv);
    if (paths == NULL) {
        return false; 
    }

    char* token = strtok(paths, ":");
    while (token != NULL) {
        char fullPath[PATH_MAX];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", token, cmd);
        if (access(fullPath, X_OK) == 0) {
            free(paths); 
            return true; 
        }
        token = strtok(NULL, ":");
    }

    free(paths); 
    return false; // Command not found
}

/**
 * Function to check if the syntax of the command is valid
 * @return a bool denoting whether the syntax is valid
*/
bool isValidSyntax() {
    bool openQuote = false;
    char quoteChar = '\0';
    List *tmp = data->currentToken; 
    bool firstToken = true; 

    while (tmp) {
        char *token = tmp->t; 
        
        if (firstToken) {
            if (token == NULL || strlen(token) == 0) {
                printf("Error: invalid syntax!\n");
                return false;
            }
            
            if (!commandExists(tmp->t)) {
                printf("Error: command not found!\n");
                return false;
            }
            
            firstToken = false;
        }

        // Check for the presence of quotes in the token
        for (int i = 0; token && token[i] != '\0'; i++) {
            if (token[i] == '"' || token[i] == '\'') {
                if (!openQuote) {
                    openQuote = true;
                    quoteChar = token[i];
                } else if (quoteChar == token[i]) {
                    openQuote = false;
                    quoteChar = '\0'; 
                }
            }
        }
        tmp = tmp->next;
    }

    if (openQuote) {
        printf("Error: invalid syntax!\n");
        return false;
    }

    return true;
}

/**
 * Checks whether the input string \param s is an operator.
 * @param s input string.
 * @return a bool denoting whether the current string is an operator.
 */
bool isOperator(char *s) {
  
  char *operators[] = {
      "&",
      "&&",
      "||",
      ";",
      "<",
      ">",
      "|",
      NULL};

  for (int i = 0; operators[i] != NULL; i++) {
    if (strcmp(s, operators[i]) == 0) {
      return true;
    } 
  }
  
  return false;
}

/**
 * The function acceptToken checks whether the current token matches a target identifier,
 * and goes to the next token if this is the case.
 * @param lp List pointer to the start of the tokenlist.
 * @param ident target identifier
 * @return a bool denoting whether the current token matches the target identifier.
 */
bool acceptToken(char *ident) {
  if (!data->currentToken)
    return false;
  if (strcmp(data->currentToken->t, ident) == 0) {
    // Advance the global currentToken after matching
    data->currentToken = data->currentToken->next;
    return true;
  }
  return false;
}

