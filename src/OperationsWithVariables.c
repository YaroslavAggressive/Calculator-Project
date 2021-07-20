// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "OperationsWithVariables.h"
#include "ErrorProcessing.h"
#include "SupportingFunctions.h"
#include <stdio.h>
#include <malloc.h>

// function for creating a stack of expression operands
stack_variables_t* CreateValArr(error_t* wrongs) {
	stack_variables_t* newArr = malloc(MIN_VAR_SIZE * sizeof(stack_variables_t));
	if (newArr == NULL) {
		*wrongs = ERROR_MEMORY_STACK;
		return NULL;
	}
	else {
		int i;
		for (i = 0; i < MIN_VAR_SIZE; i++) {
			newArr[i].value = 0;
			newArr[i].name = 0;
			newArr[i].initialized = CALC_FALSE;
		}
		return newArr;
	}
}

// additional memory allocation function for adding new operands to the stack
stack_variables_t* IncreaseValSize(stack_variables_t* arrOfValues, int* sizeOfVariables, error_t* wrongs) {
	stack_variables_t* newArr = NULL;
	newArr = realloc(arrOfValues, (*sizeOfVariables + 1) * sizeof(stack_variables_t));
	if (newArr == NULL) {
		*wrongs = ERROR_MEMORY_STACK;
		return arrOfValues;
	}
	else {
		arrOfValues = newArr;
		*sizeOfVariables += 1;
		arrOfValues[*sizeOfVariables - 1].name = 0;
		arrOfValues[*sizeOfVariables - 1].value = 0;
		arrOfValues[*sizeOfVariables - 1].initialized = CALC_FALSE;
		return arrOfValues;
	}
}

// function for freeing memory allocated for the operand stack
void FreeVariables(stack_variables_t* arrOfValues) {
	if (arrOfValues != NULL) {
		free(arrOfValues); // clear memory with variables array
		arrOfValues = NULL;
	}
}