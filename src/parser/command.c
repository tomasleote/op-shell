#include <stdbool.h>
#include "shellComponents.h"
#include "parsingTools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Function to convert a command type to a string
 * @param type The command type
 * @return The string representation of the command type
*/
char* strdup(const char* s) {
    size_t len = strlen(s) + 1; // +1 for the null terminator
    char* newStr = malloc(len);
    if (newStr == NULL) return NULL; // Check malloc success
    return memcpy(newStr, s, len);
}

/**
 * Function to create a new command
 * @param commandName The name of the command
 * @return The new command
 * @return NULL if the command could not be created
 */
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
    newCommand->pid = -1;

    return newCommand;
}

/*
* Function to add an option to a command
* @param command The command to add the option to
* @param option The option to add
*/
void addOptionToCommand(Command* command, char* option) {
    command->options = realloc(command->options, sizeof(char*) * (command->optionCount + 1));
    if (command->options == NULL) {
        perror("Unable to reallocate memory for options");
        exit(EXIT_FAILURE);
    }
    command->options[command->optionCount] = strdup(option);
    command->optionCount++;
}

/**
 * Function to free a command and its associated resources  
 * @param command The command to free
 * @return NULL
*/
void freeCommand(Command* command) {
    if (command != NULL) {
        free(command->command);
        for (int i = 0; i < command->optionCount; i++) {
            free(command->options[i]);
        }
        free(command->options);
        free(command);
    }
}

/*
* Function to free a list of commands
* @param head The head of the list to free
*/
void freeCommandList(Command* head) {
    while (head != NULL) {
        Command* next = head->next;
        freeCommand(head); // Use existing freeCommand to free individual commands
        head = next;
    }
}

/**
 * Function to append a command to the end of a list
 * @param head The head of the list
 * @param newCommand The command to append
*/
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

/**
 * Function to change the operator of a command
 * @param command The command to change the operator of
 * @param newOp The new operator
*/
void changeOperator(Command* command, OperatorType newOp) {
    if (command != NULL) {
        command->nextOp = newOp;
    } else {
        fprintf(stderr, "Error: Attempted to change operator of NULL command\n");
    }
}

/*
* Function to delete a command from the list
* @param head The head of the list
* @param command The command to delete
*/
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

/**
 * Function to print a command
 * @param command The command to print
*/
void printCommand(Command* node) {
    if (!node) return;

    printf("  %sCommand%s: %s%s%s,\n", YELLOW, WHITE, GREEN, node->command ? node->command : "NULL", WHITE);
    
    if(node->previous) {
        printf("  %sPrevious%s: %s%p%s (%s%s%s),\n", YELLOW, WHITE, PURPLE, (void*)node->previous, WHITE, GREEN, node->previous->command ? node->previous->command : "NULL", WHITE);
    } else {
        printf("  %sPrevious%s: %sNULL%s,\n", YELLOW, WHITE, PURPLE, WHITE);
    }

    if(node->next) {
        printf("  %sNext%s: %s%p%s (%s%s%s),\n", YELLOW, WHITE, PURPLE, (void*)node->next, WHITE, GREEN, node->next->command ? node->next->command : "NULL", WHITE);
    } else {
        printf("  %sNext%s: %sNULL%s,\n", YELLOW, WHITE, PURPLE, WHITE);
    }

    printf("  %sPID%s: %s%d%s,\n", YELLOW, WHITE, GREEN, node->pid, WHITE); 
    printf("  %sType%s: %s%s%s,\n", YELLOW, WHITE, GREEN, commandTypeToString(node->type), WHITE);
    printf("  %sNext Operator%s: %s%s%s,\n", YELLOW, WHITE, GREEN, operatorTypeToString(node->nextOp), WHITE);
    printf("  %sPipes%s: { read: %d, write: %d }\n", YELLOW, WHITE, node->pipes[0], node->pipes[1]);
    printf("  %sRedirections%s: { input: %d, output: %d }\n", YELLOW, WHITE, node->redirections[0], node->redirections[1]);
}

/**
 * Function to print a list of commands
 * @param head The head of the list to print
*/
void printCommandList(Command* head) {
    if (!head) {
        printf("%sEMPTY%s\n", RED, WHITE);
        return;
    }
    Command* temp = head;
    while (temp) {
        printf("%s{%s\n", PURPLE, WHITE);
        printCommand(temp);
        printf("%s}%s", PURPLE, WHITE);
        temp = temp->next;
        if (temp) printf(",\n"); // Separator between commands if there's another command in the list
        else printf("\n");
    }
}