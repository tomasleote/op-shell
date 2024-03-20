
#include "parsingTools.h"
#include "shellComponents.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

shellData *data = NULL; // Global data

/**
 * Function to get the shell data
 * @return The shell data
*/
shellData *getShellData() {
    return data;
}

/**
 * Function to initialize the shell data and parse the token list and command list
*/
void shellDataInit(char *inputLine) {

    if (data == NULL) {
        data = malloc(sizeof(shellData));
        data->tokenList = NULL;
        data->commandList = NULL;
        data->currentToken = NULL;
        data->currentCommand = NULL;
        data->inputPath = NULL;
        data->outputPath = NULL;
        data->isPipeline = false;	
        data->lastExitStatus = 1; 
    }

    if (inputLine == NULL || strlen(inputLine) == 0) {
        printf("Error: invalid syntax!\n");
        return;
    }

    data->tokenList = getTokenList(inputLine);
    data->currentToken = data->tokenList;
    data->commandList = parseInputLine(data->tokenList);
}

/**
 * Function to destroy the shell data
 * Frees the token list, command list, input path, output path and the shell data
*/
void shellDataDestroy() {
    if (data == NULL) {
        return;
    }

    freeTokenList(data->tokenList);
    freeCommandList(data->commandList);
    free(data->inputPath);
    free(data->outputPath);
    free(data);
    data = NULL;
}

/**
 * Debuging function to print all data
 * Prints the token list, command list, current command, current token, input path, output path and isPipeline
*/
void printData() {
    printList(data->tokenList);
    printCommandList(data->commandList);
    printf("Current Command: %s\n", data->currentCommand->command);
    printf("Current Token: %s\n", data->currentToken->t);
    printf("Input Path: %s\n", data->inputPath != NULL ? data->inputPath : "NULL");
    printf("Output Path: %s\n", data->outputPath != NULL ? data->outputPath : "NULL");
    printf("Is Pipeline: %d\n", data->isPipeline);
}
