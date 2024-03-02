#ifndef PARSER_H
#define PARSER_H

#include "list.h"


// Declares the parseInput function which takes a pointer to a List as its parameter
// This function is responsible for reading input from stdin, parsing it into Process
// instances, and inserting them into the provided List.
void parseInput(List* processList);

#endif // PARSER_H
