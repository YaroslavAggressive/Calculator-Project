// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "StackDoubleFunctions.h"
#include "ErrorProcessing.h"
#include "SupportingFunctions.h"
#include "OperationsWithVariables.h"
#include <malloc.h>
#include <math.h>

// creation function of numbers stack
stack_double_t* CreateNewResultStack(error_t* wrongs) { // input data - pointer on variable with error code
	int i;
	stack_double_t* newStack = malloc(1 * sizeof(stack_double_t)); // alocating memory for stack structure
	if (newStack == NULL) { // checking for allocating memory errors
		*wrongs = ERROR_MEMORY_STACK;
		return NULL;
	}
	else{
		newStack->value = malloc(INPUT_DOUBLE_STACK_SIZE * sizeof(double)); // allocating memory for real numbers
		if (newStack->value == NULL) { 
			*wrongs = ERROR_MEMORY_STACK;
			free(newStack);
			return NULL;
		}
		else{
			// filling the entire pre-allocated stack volume with zeros so that there is no garbage there 
			for (i = 0; i < INPUT_DOUBLE_STACK_SIZE; i++) { 
				newStack->value[i] = 0;
			}
			newStack->size = INPUT_DOUBLE_STACK_SIZE;
			newStack->pointer = 0; // top of the stack is on the first element
			return newStack;
		}
	}
}

// function for deleting an element of the operand stack
// input data - pointer to the structure of the operand stack
void PopDouble(stack_double_t* doubleStack) {
	doubleStack->value[doubleStack->pointer - 1] = 0;
	doubleStack->pointer -= 1;
}

// function for writing digits to a number in the stack
// input data - a pointer to the stack structure, the current digit of the number
// the number of the digit after the point and the indicator of the point itself
// flowEndIndic - indicator of the fractional part of a number. numberAfterflowEnd - the number of digit after the decimal point that we enter
void PutInStackElem(stack_double_t* doubleStack, char symbol, bool_t flowEndIndic, int numberAfterflowEnd) { 
	if (flowEndIndic != 1) {// reading the integer part of the number
		// the point was not met. So, we write down the integer part of the number
		doubleStack->value[doubleStack->pointer] = doubleStack->value[doubleStack->pointer] * 10 + (double)((int)(symbol)-NUMERAL_DOWN);
	}
	else {
		// point is met - we write down the fractional part of the number
		doubleStack->value[doubleStack->pointer] = doubleStack->value[doubleStack->pointer] + (double)((int)(symbol)-NUMERAL_DOWN) * pow(0.1, numberAfterflowEnd);
	}
}

// function for adding a new element to the stack
// input data - pointers to the stack structure and to the variable with the error code
stack_double_t* GoToNextNumber(stack_double_t* doubleStack, error_t* wrongs) {
	double* newflowEnder = NULL; // temporary pointer
	if (doubleStack->pointer == doubleStack->size - 1) { // stack is full
		doubleStack->size += 1;
		if (doubleStack->value != NULL) {
			newflowEnder = realloc(doubleStack->value, (doubleStack->size) * sizeof(double)); // trying to expand the stack
		}
		else {
			*wrongs = ERROR_MEMORY_STACK; // memory error
			return doubleStack;
		}
		if (newflowEnder == NULL) { // memory allocating error
			*wrongs = ERROR_MEMORY_STACK;
			return doubleStack;
		}
		doubleStack->value = newflowEnder; // changing the pointer to a new one, in which the address of the extended memory was written
		doubleStack->value[doubleStack->size - 1] = 0; // while we are taking the last element of the stack, since another function writes to it
	}
	doubleStack->pointer += 1;
	return doubleStack;
}

// function for getting the vertices of the operand stack
// input data - pointer to the stack structure
double GetLastNumber(const stack_double_t* doubleStack) {
	return doubleStack->value[doubleStack->pointer - 1]; // we take the penultimate current element, since the last one contains garbage
}

// function of removing the stack of operands 
// It is used when an error is detected (during the calculation of the result, in this case) or when the function completes successfully
// input data - stack structure and array of declared variables 
void FreeStackDouble(stack_double_t* doubleStack, stack_variables_t* arrOfValues) {
	FreeVariables(arrOfValues);
	if (doubleStack != NULL) {
		if (doubleStack->value != NULL) { // all dynamic memory purges occur after checks for the presence of a dedicated area
			free(doubleStack->value); // clearing an array of numbers in the structure
		}
		free(doubleStack); // clearing the structure itself by pointer
	}
}

// function of checking the stack of operands for the presence of elements inside
// input data- a pointer to the stack structure itself
bool_t CheckDoubleStack(const stack_double_t* doubleStack) {
	if (doubleStack->pointer == 0) {
		return CALC_TRUE; // stack is empty
	}
	else {
		return CALC_FALSE; // there are still numerical elements left in the stack
	}
}

// function of placing a number on the operand stack
// input data - placed number and a pointer to the stack structure
void PutNumberInStackDouble(stack_double_t* doubleStack, double a) {
	doubleStack->value[doubleStack->pointer - 1] = a;
}