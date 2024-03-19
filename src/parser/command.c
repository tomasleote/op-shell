#include <stdbool.h>
#include "shellComponents.h"
#include "parsingTools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* strdup(const char* s) {
    size_t len = strlen(s) + 1; // +1 for the null terminator
    char* newStr = malloc(len);
    if (newStr == NULL) return NULL; // Check malloc success
    return memcpy(newStr, s, len);
}

// Function to create a new command
Command* createCommand(char* commandName) {
    Command* newCommand = (Command*)malloc(sizeof(Command));
    if (newCommand == NULL) {
        perror("malloc");
        // Free memory
       return NULL;
    }

    newCommand->command = strdup(commandName);
    if (newCommand->command == NULL) {
        perror("strdup");
        // Free memory
        free(newCommand); // Make sure to free allocated command before returning NULL
        return NULL;
    }
    newCommand->next = NULL;
    newCommand->previous = NULL;
    newCommand->type = CMD_EXTERNAL; 
    newCommand->options = NULL;
    newCommand->optionCount = 0;
    newCommand->redirections[0] = -1;
    newCommand->redirections[1] = -1;
    newCommand->pipes[0] = -1;
    newCommand->pipes[1] = -1;
    newCommand->nextOp = OP_NONE;
    newCommand->inputFile = NULL;
    newCommand->outputFile = NULL;

    return newCommand;
}

// Function to add an option to a command
void addOptionToCommand(Command* command, char* option) {
    command->options = realloc(command->options, sizeof(char*) * (command->optionCount + 1));
    if (command->options == NULL) {
        perror("Unable to reallocate memory for options");
        exit(EXIT_FAILURE);
    }
    command->options[command->optionCount] = strdup(option);
    command->optionCount++;
}

// Function to free a command and its associated resources
void freeCommand(Command* command) {
    if (command != NULL) {
        free(command->command);
        for (int i = 0; i < command->optionCount; i++) {
            free(command->options[i]);
        }
        free(command->options);
        free(command->inputFile);
        free(command->outputFile);
        free(command);
    }
}

// Function to free a list of commands
void freeCommandList(Command* head) {
    while (head != NULL) {
        Command* next = head->next;
        freeCommand(head); // Use existing freeCommand to free individual commands
        head = next;
    }
}

// Function to append a command to the end of a list
void appendCommand(Command** head, Command* newCommand) {
    if (*head == NULL) {
        *head = newCommand;
    } else {
        Command* last = *head;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newCommand;
        newCommand->previous = last;
    }
}

void changeOperator(Command* command, OperatorType newOp) {
    if (command != NULL) {
        command->nextOp = newOp;
    } else {
        fprintf(stderr, "Error: Attempted to change operator of NULL command\n");
    }
}

// Function to delete a command from the list
void deleteCommand(Command** head, Command* command) {
    if (*head == NULL || command == NULL) return;

    if (*head == command) {
        *head = command->next;
    }

    if (command->next != NULL) {
        command->next->previous = command->previous;
    }

    if (command->previous != NULL) {
        command->previous->next = command->next;
    }

    freeCommand(command);
}

void printCommandList(const Command* head) {
    printf("Command List:\n");
    while (head != NULL) {
        printf("%sCommand: %s\n", GREEN, head->command);
        printf(" - Type: %s\n", head->type == CMD_BUILTIN ? "Built-in" : "External");
        printf(" - Next Operator: %s\n", head->nextOp == OP_NONE ? "None" : head->nextOp == OP_AND ? "&&" : 
            head->nextOp == OP_OR ? "||" : head->nextOp == OP_SEQ ? ";" : "|");
        printf(" - Next command: %s\n", head->next != NULL ? head->next->command : "None");
        printf(" - Input File: %s\n", head->inputFile != NULL ? head->inputFile : "None");
        printf(" - Output File: %s\n", head->outputFile != NULL ? head->outputFile : "None");
        printf(" - Options:");
        if (head->optionCount > 0) {
            for (int i = 0; i < head->optionCount; i++) {
                printf(" %s", head->options[i]);
            }
            printf("\n");
        } else {
            printf(" None\n");
        }
        if (head->redirections[0] != -1) {
            printf(" - Input Redirection: %d\n", head->redirections[0]);
        }
        if (head->redirections[1] != -1) {
            printf(" - Output Redirection: %d\n", head->redirections[1]);
        }
        if (head->pipes[0] != -1) {
            printf(" - Pipe In: %d\n", head->pipes[0]);
        }
        if (head->pipes[1] != -1) {
            printf(" - Pipe Out: %d\n", head->pipes[1]);
        }
        printf("%s\n", WHITE);
        head = head->next;
    }
}


