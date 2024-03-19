
#include "parsingTools.h"
#include "shellComponents.h"
#include <stddef.h>
#include <stdlib.h>

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
    }

    shellData *data = getShellData();
    data->tokenList = getTokenList(inputLine);
    data->currentToken = data->tokenList;
    data->commandList = parseInputLine(data->tokenList);
    //printList(data->tokenList);
    //printCommandList(data->commandList);
}

void shellDataDestroy() {
    if (data == NULL) {
        return;
    }
    freeTokenList(data->tokenList);
    freeCommandList(data->commandList);
    free(data);
    data = NULL;
}
