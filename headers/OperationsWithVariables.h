#ifndef VARIABLES_ITEMS_INCLUDED
#define VARIABLES_ITEMS_INCLUDED
#pragma once

#include "ErrorProcessing.h"
#include "SupportingFunctions.h"

#define MIN_VAR_SIZE 10 // initial size of the variable array

// type created to store most of the data about a single variable that can be entered and remembered by the user.
typedef struct {
	double value; // variable value
	char name; // variable name (only single-character variables are implemented)
	bool_t initialized; // indicator of initialization of this variable
} stack_variables_t;

// variable processing block
stack_variables_t* CreateValArr(error_t* wrongs);
stack_variables_t* IncreaseValSize(stack_variables_t* arrOfValues, int* sizeOfVariables, error_t* wrongs);
void FreeVariables(stack_variables_t* arrOfValues);

#endif