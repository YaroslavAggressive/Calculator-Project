
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "BuildingNotation.h"
#include "StackCharFunctions.h"
#include "SupportingFunctions.h"

/*
	This file contains functions that are integral elements of the main large complex Build Notation.
	The splitting is made for better readability of the code and at the same time to avoid its duplication
 */

// function of clearing the stack to the nearest opening bracket. In fact, we write out operations with predefined priorities in the postfix notation
// input data -a pointer to a string under construction, a pointer to a variable of its size
// pointers to the error code, stack structure, and information structures about the previous elements of the source expression
void PopStackByOpenBrac(char** postfixExpression, int* size, string_elems_t* previous, error_t* wrongs, stack_t* symbolsStack, bool_t* isAnyNumber) {
	char stackOper; 
	operation_t opSetStack;
	if (previous->lastOper != OPER_NO) { // operand shortage error
		*wrongs = ERROR_BRACKETS;
	}
	else {
		if (CheckCharStack(symbolsStack) != CALC_TRUE) { // condition for exiting the loop - stack is empty
			stackOper = GetLast(symbolsStack); // taking symbol from stack, which corresponds to some operation
			Pop(symbolsStack); // deleting the extracted element
			opSetStack = FindOperOfAllStack(stackOper); // find out which operation was taken out
			if ((opSetStack == OPEN_BRACKET) && (*isAnyNumber == CALC_FALSE)) { // there were no operations in parentheses, so the input is incorrect
				*wrongs = ERROR_NOT_ENOUGH_OPERANDS;
			}
			else {
				*postfixExpression = PutInPostfix(*postfixExpression, LETTER_SPACE, size, wrongs); // put the operation in the line
				if (*wrongs != ERROR_MEMORY_STACK) { // checking for memory errors
					// if brackets are encountered, it is necessary to pull all operations from the stack to the nearest opening bracket
					while ((opSetStack != OPEN_BRACKET)) {
						*postfixExpression = PutInPostfix(*postfixExpression, stackOper, size, wrongs);
						if (*wrongs == ERROR_MEMORY_STACK) {
							return;
						}
						if (CheckCharStack(symbolsStack) == CALC_TRUE) { // if stack is empty, and opening bracket was not found, it means that input is incorrect
							*wrongs = ERROR_BRACKETS;
							return;
						}
						else {
							stackOper = GetLast(symbolsStack); // repeat cycle step
							Pop(symbolsStack);
						}
						opSetStack = FindOperOfAllStack(stackOper);
						// put a space in the string to divide the operations and operands in it
						*postfixExpression = PutInPostfix(*postfixExpression, LETTER_SPACE, size, wrongs);
						if (*wrongs == ERROR_MEMORY_STACK) {
							return;
						}
					}
				}
			}
		}
		else {
			*wrongs = ERROR_BRACKETS;
		}
	}
}

// function of analyzing the operation received from the line and leading it to the stack or a new line according to the situation
// input data - pointers to the new line and its size, a pointer to the operation that was just retrieved
// as well as the stack structure and information about the current and previous operation and a pointer to the error code
void OperationComparsion(char** postfixExpression, int* size, operation_t* opSetStr, stack_t* symbolsStack, operation_info_t* current, error_t* wrongs) {
	int powerBoolean;
	char stackOper, strOper;
	operation_t opSetStack;
	// immediately put a separator in the string so that the tokens do not stick together
	*postfixExpression = PutInPostfix(*postfixExpression, LETTER_SPACE, size, wrongs); 
	if (*wrongs == ERROR_MEMORY_STACK) {
		return;
	}
	stackOper = GetLast(symbolsStack); // getting the operation element from the top of the stack
	opSetStack = FindOperOfAllStack(stackOper); // we are looking at what kind of operation
	// checking for the docking of two degrees and a degree with a unary minus
	powerBoolean = (*opSetStr == DEGREE) && ((opSetStack == DEGREE) || (opSetStack == UNAR_MINUS)); 
	if (((*current == OPER_UNAR) && (IsBinar(opSetStack) == CALC_FALSE)) ||
		(ChoosePriority(*opSetStr) > ChoosePriority(opSetStack)) ||
		(powerBoolean == 1) || ((ChoosePriority(*opSetStr) <= ChoosePriority(opSetStack)) && (opSetStack == OPEN_BRACKET))) { // condition for adding a new operation to the stack
		stackOper = OperToSymbol(*opSetStr); // converting the operation to the one - character format used
		PutInStack(symbolsStack, stackOper, wrongs); // put it on the stack
		if (*wrongs == ERROR_MEMORY_STACK) {
			return;
		}
	}
	else { //priority of operation in stack is lower than or equal to the last character read from the string
		strOper = OperToSymbol(*opSetStr);
		// we throw out all operations from the stack ,the priority of which is higher than the current operation taken from the line
		while ((ChoosePriority(*opSetStr) <= ChoosePriority(opSetStack)) && (CheckCharStack(symbolsStack) != CALC_TRUE) && (opSetStack != OPEN_BRACKET)) {
			Pop(symbolsStack); // remove the top of the stack, since it goes into Polish notation
			*postfixExpression = PutInPostfix(*postfixExpression, stackOper, size, wrongs); // actually inserting the operation into the stack
			if (*wrongs == ERROR_MEMORY_STACK) { // error checking
				return;
			}
			*postfixExpression = PutInPostfix(*postfixExpression, LETTER_SPACE, size, wrongs);
			if (*wrongs == ERROR_MEMORY_STACK) { 
				return;
			}
			stackOper = GetLast(symbolsStack); // getting a new operation
			opSetStack = FindOperOfAllStack(stackOper); 
		}
		PutInStack(symbolsStack, strOper, wrongs);
		if (*wrongs == ERROR_MEMORY_STACK) { 
			return;
		}
	}
}


// function of processing the current last operation in the source expression and writing it to the stack of operations
// input data - a pointer to an expression in Polish notation, its size, a variable with an error code
// stack of operations, pointer to the previous operation, a boolean variable with information about the last element of the expression
void ProcessingOperation(char** postfixExpression, int* size, operation_t* opSetStr, string_elems_t* previous, stack_t* symbolsStack, bool_t* isAnyNumber, error_t* wrongs) {
	operation_info_t current;
	if (*opSetStr != CLOSE_BRACKET) {
		if (*opSetStr == OPEN_BRACKET) {
			current = OPER_OPEN_BRAC;
		}
		else
			if (previous->lastSym == SYM_LOG) {
				*wrongs = ERROR_INVALID_INPUT;
				return;
			}
			else {
				if (IsBinar(*opSetStr) == CALC_TRUE) {
					current = OPER_BINAR;
				}
				else {
					current = OPER_UNAR;
				}
			}
	}
	else {
		current = OPER_NO;
	}
	if (((current == OPER_BINAR) && ((previous->lastOper != OPER_NO) || (*isAnyNumber == CALC_FALSE)))) {
		*wrongs = ERROR_INVALID_INPUT;
		return;
	}
	if (((*opSetStr == CLOSE_BRACKET) || (current == OPER_BINAR)) && (previous->lastOper == OPER_BINAR)) {
		*wrongs = ERROR_NOT_ENOUGH_OPERANDS;
		return;
	}
	if ((*opSetStr != CLOSE_BRACKET)) {
		if ((*size > 0) && (postfixExpression[0][*size - 1] != LETTER_SPACE)) {
			*postfixExpression = PutInPostfix(*postfixExpression, LETTER_SPACE, size, wrongs);
			if (*wrongs == ERROR_MEMORY_STACK) {
				return;
			}
		}
		OperationComparsion(postfixExpression, size, opSetStr, symbolsStack, &current, wrongs);
		if (*wrongs > ERROR_NO) {
			return;
		}
	}
	else { // encounter a closing parenthesis, you need to throw all operations from the stack before the opening parenthesis, or output an error message
		PopStackByOpenBrac(postfixExpression, size, previous, wrongs, symbolsStack, isAnyNumber);
		if (*wrongs > ERROR_NO) {
			return;
		}
	}
	previous->lastOper = current;
}

// function of processing variables in the appropriate mode and their entry into the Polish notation
// input data - a pointer to an expression in Polish notation, its size, stack of operands, int variable with number of varibles in calculator
// and variable with error code
void ProcessingVariables(char** postfixExpression, int* size, stack_variables_t** process, char comparingSym, int* numOfVariables, int* varSize, error_t* wrongs) {
	int j;
	char varSymbol;
	*wrongs = ERROR_NO;
	if ((process[0][*numOfVariables - 1].initialized != CALC_TRUE) && (*numOfVariables != 0)) {
		return;
	}
	*postfixExpression = PutInPostfix(*postfixExpression, LETTER_VARIABLES, size, wrongs); // indicator of the variable that is processed in the function of finding the result
	if (*wrongs == ERROR_MEMORY_STACK) {
		return;
	}
	for (j = 0; j < *numOfVariables; j++) {
		if (comparingSym == process[0][j].name) {
			break;
		}
	}
	if (j == *numOfVariables) {
		if (*numOfVariables == *varSize) {
			*process = IncreaseValSize(*process, varSize, wrongs);
			if (*wrongs == ERROR_VARIABLES) {
				return;
			}
		}
		*numOfVariables += 1;
		process[0][*numOfVariables - 1].name = comparingSym;
		varSymbol = (char)(*numOfVariables + NUMERAL_DOWN);
		*postfixExpression = PutInPostfix(*postfixExpression, varSymbol, size, wrongs); // put the variable number in the string
		if (*wrongs == ERROR_MEMORY_STACK) {
			return;
		}
	}
	else {
		varSymbol = (char)((j + 1) + NUMERAL_DOWN);
		*postfixExpression = PutInPostfix(*postfixExpression, varSymbol, size, wrongs);
		if (*wrongs == ERROR_MEMORY_STACK) {
			return;
		}
	}
}

