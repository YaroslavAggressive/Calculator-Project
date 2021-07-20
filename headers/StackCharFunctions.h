#ifndef STACK_CHAR_PROTOTYPES_H_INCLUDED
#define STACK_CHAR_PROTOTYPES_H_INCLUDED
#pragma once

#include "ErrorProcessing.h"
#include "OperationsWithVariables.h"
#define INPUT_STACK_SIZE 10 // initial size of the operation stack

// a structural type that stores all data about the operation stack
// used in function BuildNotation
typedef struct {
	char* value; //  a pointer to the stack itself consisting of symbols denoting operations
	int pointer;// stack top
	int size; // stack size
} stack_t;

// block of functions only for working with sign stacks
stack_t* CreateNewStack(error_t* wrongs);
void PutInStack(stack_t* turn, char oper, error_t* wrongs);
void Pop(stack_t* turn);
char GetLast(const stack_t* turn);
bool_t CheckCharStack(const stack_t* turn);
void FreeStackChar(stack_t* turn, stack_variables_t* arrOfValues);

#endif