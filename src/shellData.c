
#include "parsingTools.h"
#include "shellComponents.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

shellData *data = NULL;

shellData *getShellData() {
    return data;
}

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
    }

    shellData *data = getShellData();
    data->tokenList = getTokenList(inputLine);
    data->currentToken = data->tokenList;
    data->commandList = parseInputLine(data->tokenList);
    //printData();
}

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

void printData() {
    //printList(data->tokenList);
    printCommandList(data->commandList);
    //printf("Current Command: %s\n", data->currentCommand->command);
    //printf("Current Token: %s\n", data->currentToken->t);
    printf("Input Path: %s\n", data->inputPath != NULL ? data->inputPath : "NULL");
    printf("Output Path: %s\n", data->outputPath != NULL ? data->outputPath : "NULL");
    printf("Is Pipeline: %d\n", data->isPipeline);
}
