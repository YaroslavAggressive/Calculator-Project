// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "StackCharFunctions.h"
#include "ErrorProcessing.h"
#include "OperationsWithVariables.h"
#include "SupportingFunctions.h"
#include <malloc.h>

// stack creation function
stack_t* CreateNewStack(error_t* wrongs) { // input data - pointer on variable with error code, output data - pointer on stack structure
	stack_t* newTurn = NULL; // temp pointer , in which value from the function realloc is written 
	newTurn = malloc(1 * sizeof(stack_t)); // allocating memory for the main parameters of the stack
	if (newTurn == NULL) { // was not possible to allocate memory - processing the error
		*wrongs = ERROR_MEMORY_STACK;
		return NULL; // return nullptr, if memory hasn't been allocated
	}
	else {
		newTurn->value = malloc(INPUT_STACK_SIZE * sizeof(char)); // allocating memory for stack elements, in this case default value - 20
		if (newTurn->value == NULL) { // memory for stack hasn't been allocated
			*wrongs = ERROR_MEMORY_STACK;
			free(newTurn); // delete the pointer to the structure itself, why is it needed if the stack of operations is not written to it
			return NULL;
		}
		newTurn->size = INPUT_STACK_SIZE; // storing stack size in the same structure
		newTurn->pointer = 0; // top element of the stack is going on a spree, the stack is still empty
		return newTurn; 
	}
}

// function of writing an element to the stack-to its top
// input data - pointer to the stack structure, a symbol that we write to the stack, and a pointer to a variable with an error code
void PutInStack(stack_t* turn, char oper, error_t* wrongs) {
	char* process_turn = NULL;
	if (turn->pointer < turn->size - 1) {
		turn->value[turn->pointer] = oper; // writing an element to the top of the stack
		turn->pointer += 1; // moving the pointer to the vertex so as not to lose the element
	}
	else { // stack is full =>it is necessary to expand it
		process_turn = realloc(turn->value, (turn->size + 1) * sizeof(char));
		if (process_turn == NULL) { // memory error - it was not possible to expand the stack
			*wrongs = ERROR_MEMORY_STACK;
			return; // error code has been changed => we return to the main program
		}
		else { // have expanded the stack correctly - we change the pointer in the structure and write the corresponding data
			turn->value = process_turn;
			turn->size += 1;
			turn->value[turn->pointer] = oper;
			turn->pointer += 1;
		}
	}
}

// function for removing an element from stack
// input data - pointer to the stack structure
void Pop(stack_t* turn) { 
	turn->value[turn->pointer - 1] = 0; // last element is zipped, the index of the vertex decreases
	turn->pointer -= 1;
}

// function for getting the "top" of the stack (the last pushed element)
char GetLast(const stack_t* turn) { 
	return turn->value[turn->pointer - 1];
}

// function of checking the stack for the presence of elements in it (checking for emptiness)
bool_t CheckCharStack(const stack_t* turn) {
	if (turn->pointer <= 0) { // index of stack top <= 0 => stack is empty
		return CALC_TRUE; 
	}
	else {
		return CALC_FALSE; 
	}
}

// function for clearing the variable stack and stack of operations
// input data - pointers to the structures of the variable stack and the operand stack
void FreeStackChar(stack_t* turn, stack_variables_t* arrOfValues) {
	FreeVariables(arrOfValues);
	if (turn != NULL) {
		if (turn->value != NULL) {
			free(turn->value); // clearing all allocate stack memory by the pointer
		}
		free(turn); // clearing memory on stack structure pointer 
	}
}