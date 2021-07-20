#ifndef STACK_DOUBLE_PROTOTYPES_H_INCLUDED
#define STACK_DOUBLE_PROTOTYPES_H_INCLUDED
#pragma once

#include "ErrorProcessing.h"
#include "OperationsWithVariables.h"
#define INPUT_DOUBLE_STACK_SIZE 10 // initial operant stack size

// a structural type that stores all data about the variable stack
// used in counting result function CountResult
typedef struct {
	double* value; // a pointer to the stack itself, which stores the values of the operands in the desired order
	int pointer; // stack top
	int size; // stack size
} stack_double_t;

// a block of functions for working with the operant (numeric) stack
stack_double_t* CreateNewResultStack(error_t* wrongs);
void FreeStackDouble(stack_double_t* doubleStack, stack_variables_t* arrOfValues);
void PopDouble(stack_double_t* doubleStack);
void PutInStackElem(stack_double_t* doubleStack, char symbol, bool_t flowEndIndic, int numberAfterflowEnd);
stack_double_t* GoToNextNumber(stack_double_t* doubleStack, error_t* wrongs);
double GetLastNumber(const stack_double_t* doubleStack);
void PutNumberInStackDouble(stack_double_t* doubleStack, double a);
bool_t CheckDoubleStack(const stack_double_t* doubleStack);

#endif
