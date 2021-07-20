// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ErrorProcessing.h"
#include "BuildingNotation.h"
#include "StackCharFunctions.h"
#include "OperationsWithVariables.h"
#include "SupportingFunctions.h"
#include "SupportingBuildingNotationFunctions.h"
#include <malloc.h>
#include <ctype.h>

// function for checking whether the current string element belongs to the Latin alphabet (any register)
// input data - current symbol expression[i] 
// output data - calculator boolean variable
bool_t IsInAlphabet(char const symbol) { 
	if (((symbol >= SYMBOL_SMALL_DOWN) && (symbol <= SYMBOL_SMALL_UP)) ||
		((symbol >= SYMBOL_BIG_DOWN) && (symbol <= SYMBOL_BIG_UP))) {
		return CALC_TRUE;
	}
	else {
		return CALC_FALSE;
	}
}

// function of adding a character to the end of an expression in the form of postfix notation
char* PutInPostfix(char* notation, char symbol, int* size, error_t* wrongs) { 
	char* newNotation = NULL;
	newNotation = realloc(notation, (*size + 1) * sizeof(char));
	if (newNotation == NULL) {
		*wrongs = ERROR_MEMORY_STACK;
		return notation;
	}
	notation = newNotation;
	notation[*size] = symbol;
	*size += 1;
	return notation;
}

// function of selecting the scientific form of a number from the original entered expression
// input data - pointers to the original expression and the expression in Polish notation with its size, a pointer to a variable with an error code
// index of the beginning of a number in a scientific record, and a pointer to the structure of the last recorded element of the expression
void PutInScienceForm(char const* expression, char** postfixExpression, int* size, int* i, error_t* wrongs, string_elems_t* previous) {
	int j;
	bool_t isMinus = CALC_FALSE, isPlus = CALC_FALSE, isScien = CALC_FALSE;
	if ((previous->lastSym == SYM_CONST) && ((expression[*i] == LETTER_E_BIG) || (expression[*i] == LETTER_E_SMALL))) {
		*wrongs = ERROR_INCORRECT_CONSTANTS;
	}
	else
	if (((expression[*i] == LETTER_E_SMALL) || (expression[*i] == LETTER_E_BIG)) && (previous->lastSym == SYM_SCIENCE)) {
		*wrongs = ERROR_SCIENCE_FORM;
	}
	else {
		// beginning of the scientific form of writing a number, is processed already in the computational function itself
		*postfixExpression = PutInPostfix(*postfixExpression, LETTER_SCIENCE_FORM, size, wrongs); 
		if (*wrongs != ERROR_MEMORY_STACK) {
			*postfixExpression = PutInPostfix(*postfixExpression, expression[*i], size, wrongs);
			if (*wrongs != ERROR_MEMORY_STACK) {
				if ((((int)expression[*i + 1] <= NUMERAL_UP) && ((int)expression[*i + 1] >= NUMERAL_DOWN)) ||
					(expression[*i + 1] == LETTER_ADDITION) || (expression[*i + 1] == LETTER_SUBTRACTION)) {
					j = *i + 1;
					while ((((int)expression[j] <= NUMERAL_UP) && ((int)expression[j] >= NUMERAL_DOWN)) || (((expression[j] == LETTER_SUBTRACTION) ||
						(expression[j] == LETTER_ADDITION)) && (isPlus == CALC_FALSE) && (isMinus == CALC_FALSE) && (isScien == CALC_FALSE))) {
						isPlus += (expression[j] == LETTER_ADDITION);
						isMinus += (expression[j] == LETTER_SUBTRACTION);
						isScien += ((int)expression[j] <= NUMERAL_UP) && ((int)expression[j] >= NUMERAL_DOWN);
						if (((expression[j] == LETTER_ADDITION) || (expression[j] == LETTER_SUBTRACTION)) && ((expression[j - 1] == LETTER_ADDITION) || (expression[j - 1] == LETTER_SUBTRACTION))) {// два подряд знака в научной форме, переход вобработку ошибки
							*wrongs = ERROR_MEMORY_STACK;
						}
						*postfixExpression = PutInPostfix(*postfixExpression, expression[j], size, wrongs);
						if (*wrongs == ERROR_MEMORY_STACK) {
							break;
						}
						j += 1;
					}
					if (!((expression[j - 1] >= NUMERAL_DOWN) && (expression[j - 1] <= NUMERAL_UP))) {
						*wrongs = ERROR_SCIENCE_FORM;
					}
					*i = j - 1; 
				}
				else {
					*wrongs = ERROR_SCIENCE_FORM;
				}
			}
		}
	}
}

// stack emptying function. It is used at the end of working with an expression, at the end of entering a variable
void ReleaseStack(stack_t* symbolsStack, char** postfixExpression, int*size, error_t* wrongs) {
	char stackOper;
	operation_t opSetStack;
	// the remaining characters from the stack are simply written to a new line separated by a space, they come out in a new order
	while (CheckCharStack(symbolsStack) != CALC_TRUE) {
		// we divide the tokens in postfix notation so that we can calculate the values of the resulting expression
		*postfixExpression = PutInPostfix(*postfixExpression, LETTER_SPACE, size, wrongs); 
		if (*wrongs == ERROR_MEMORY_STACK) {
			break;
		}
		stackOper = GetLast(symbolsStack);
		if (((opSetStack = FindOperOfAllStack(stackOper)) == CLOSE_BRACKET) || (opSetStack == OPEN_BRACKET)) {
			*wrongs = ERROR_BRACKETS;
			break;
		}
		*postfixExpression = PutInPostfix(*postfixExpression, stackOper, size, wrongs);
		if (*wrongs == ERROR_MEMORY_STACK) {
			break;
		}
		Pop(symbolsStack);
	}
	*postfixExpression = PutInPostfix(*postfixExpression, LETTER_SPACE, size, wrongs);
}

// error checking function in the mode of storing temporary variables
// input data - pointer on postfix expression, expression size, pointers on structures, storing data about the previous operation and the mode of variables
static void EndOfVariablesRegime(char** postfixExpression, int* size, values_check_t* check, error_t* wrongs, string_elems_t* previous) { 
	if ((check->valRegime != CALC_TRUE) || (check->valAssign == CALC_FALSE)) { 
		// the end of the variable mode was met, but no variable value assignment was detected = = error
		*wrongs = ERROR_VARIABLES;
	}
	else {
		*postfixExpression = PutInPostfix(*postfixExpression, LETTER_SPACE, size, wrongs);
		if (*wrongs != ERROR_MEMORY_STACK) { // there are no memory errors - we reset all data about the previous read characters
			check->valEnd = CALC_TRUE;
			check->valRegime = CALC_FALSE;
			check->valAssign = CALC_FALSE;
			previous->lastSym = SYM_NO;
			previous->lastOper = OPER_NO;
		}
	}
}

// function that checks for errors at the end of one of the main functions BuildNotation
// input data - pointers to a variable that stores the number of operations found in an expression, to a variable with an error code
// to structures that store data about the last character in a string, the mode of entering and storing variables, logarithm operations
// as well as the type variable itself that stores all data about the entered variables
static void EndErrorChecking(log_t* logOper, stack_variables_t* process, int* numOfVariables, string_elems_t* previous, values_check_t* check, error_t* wrongs) {
	if (logOper->numOfCommas != logOper->numOfLogs) { // discrepancy between the number of logarithms and the number of commas
		*wrongs = ERROR_INVALID_INPUT;
	}
	else
		if ((process[*numOfVariables - 1].initialized != CALC_TRUE) && (*numOfVariables != 0)) { // uninitialized variable
			*wrongs = ERROR_VARIABLES;
		}
		else
			if ((previous->lastOper == OPER_UNAR) || (previous->lastOper == OPER_BINAR)) { // lack of operands
				*wrongs = ERROR_NOT_ENOUGH_OPERANDS;
			}
			else
				if ((check->valAssign == CALC_TRUE) || (check->valEnd == CALC_TRUE)) {
					*wrongs = ERROR_VARIABLES;
				}
				else {
					*wrongs = ERROR_NO;  // no errors found
				}
}

// error checking function for writing numbers to an expression 
// input data - pointers to indicator of the beginning of expression, to the error code variable, to type that stores data about the previous character of string,
// on types that store data about the recording of variables and the last appearance of a number in a string
static void NumberErrorProcessing(string_elems_t* previous, bool_t* startExpr, num_check_t* numberCheck, values_check_t* check, error_t* wrongs) {
	if ((numberCheck->numIndic == CALC_TRUE) && (previous->lastSym == SYM_SPACE)) { // space character is entered inside the operator
		*wrongs = ERROR_WRITTING_OPERAND; // operand processing error found
	}
	else
		if (numberCheck->pointQuan > 1) { // erroneous writing of a number in decimal form (too much dots)
			*wrongs = ERROR_PROBLEMS_WITH_POINT;
		}
		else
			// error checking: an attempt to calculate the logarithm on the natural basis without allocating arguments in parentheses
			if ((previous->lastOper == OPER_NO) && (check->valEnd == CALC_FALSE) &&
				(*startExpr == CALC_TRUE) && (numberCheck->numIndic == CALC_FALSE) || (previous->lastSym == SYM_LOG)) {
				*wrongs = ERROR_INVALID_INPUT;
			}
			else
				// attempt to add a numeric value to constants or variables in the form of digits
				if ((previous->lastSym == SYM_CONST) || (previous->lastSym == SYM_VARIABLE)) { 
					*wrongs = ERROR_INVALID_INPUT;
				}
				else {
					*wrongs = ERROR_NO; // no errors found
				}
}

// error checking function when entering operations
// input data - pointer to operation, and pointers with data structures about the previous operation, the mode of variables, a pointer to the error code
static void OperationErrorProcessing(operation_t* opSetStr, values_check_t* check, string_elems_t* previous, bool_t* startExpr, error_t* wrongs) {
	if ((*opSetStr == ASSIGNMENT) && (check->valRegime == CALC_FALSE)) {
		*wrongs = ERROR_VARIABLES;
	}
	else
		if ((*opSetStr == OPEN_BRACKET) && (previous->lastOper == OPER_NO) &&
			(*startExpr == CALC_TRUE) && (check->valEnd == CALC_FALSE)) {
			*wrongs = ERROR_INVALID_INPUT;
		}
		else {
			*wrongs = ERROR_NO;
		}
}

// construction of the Polish notation
// input data - original expression, pointer on variable with error code
// output data - error code in case of incorrect input of the expression
char* BuildNotation(const char* expression, error_t* wrongs, stack_variables_t** arrOfValues) { 
	char* postfixExpression = NULL; // variable for result expression in postfix notation
	// counters for working with source string, size of output string and the crutch in order to distinguish unary operation and logarithm from variables mode
	int i = 0, iPoint, size = 0; 
	log_t logOper; // counting logarithms
	operation_t opSetStr; // variable is used to determine the operation for the currently processed string element
	bool_t isAnyNumber = CALC_FALSE, startExpr = CALC_FALSE; // indicator of the presence of at least one operand in the expression
	values_check_t check; // structure for tracking errors in the variable storage mode
	string_elems_t previous;
	num_check_t numberCheck;
	operation_info_t  current = OPER_NO;	// structures for tracking the last operation read from the source expression
	stack_t* symbolsStack = NULL;
	stack_variables_t* process = NULL; // stack for operation signs
	int numOfVariables = 0, varSize = MIN_VAR_SIZE; // number of filled variable cells and the size of the memory allocated to variables, respectively
	symbolsStack = CreateNewStack(wrongs); // building a stack of operations
	if (*wrongs > ERROR_NO) {
		return postfixExpression;
	}
	process = CreateValArr(wrongs); // initially, we allocate memory for one variable
	if (*wrongs > ERROR_NO) {
		FreeStackChar(symbolsStack, process);
		return postfixExpression;
	}
	numberCheck.numIndic = CALC_FALSE;
	numberCheck.pointQuan = 0;
	previous.lastOper = OPER_NO;
	previous.lastSym = SYM_NO;
	logOper.numOfCommas = 0;
	logOper.numOfLogs = 0;
	numOfVariables = 0; 
	check.valAssign = CALC_FALSE;
	check.valEnd = CALC_FALSE;
	check.valRegime = CALC_FALSE;
	while (expression[i] != 0) { // condition for the end of processing a line is its end
		if (isspace((unsigned char)expression[i]) == 0) { // skipping whitespace characters
			if (IsNumeral(expression[i]) || (expression[i] == LETTER_POINT)) {
				numberCheck.pointQuan += (expression[i] == LETTER_POINT);   // appearance of the fractional part of a number, a dot 
				NumberErrorProcessing(&previous, &startExpr, &numberCheck, &check, wrongs);
				if (*wrongs > ERROR_NO) {
					FreeStackChar(symbolsStack, process);
					return postfixExpression;
				}
				postfixExpression = PutInPostfix(postfixExpression, expression[i], &size, wrongs); // updating postfix expression
				if (*wrongs == ERROR_MEMORY_STACK) {
					FreeStackChar(symbolsStack, process);
					return postfixExpression;
				}
				check.valEnd = CALC_FALSE;
				previous.lastOper = OPER_NO;
				numberCheck.numIndic = CALC_TRUE;
				isAnyNumber = CALC_TRUE; // at least one number is encountered => this means that the string is not a space
				startExpr = CALC_TRUE; // at least one operand was found in the input string
				previous.lastSym = SYM_NUMBER;
			}
			else
				if (IsInAlphabet(expression[i]) == CALC_TRUE) { // if found symbols, necessary to determine what they are
					check.valEnd = CALC_FALSE;
					if (((expression[i] == LETTER_E_SMALL) || (expression[i] == LETTER_E_BIG)) && (numberCheck.numIndic == CALC_TRUE)) {
						// processing of the scientific form of writing a number
						if (previous.lastSym != SYM_NUMBER) { // error handling when entering a number in the scientific form of the recor
							*wrongs = ERROR_SCIENCE_FORM;
							FreeStackChar(symbolsStack, process);
							return postfixExpression;
						}
						// function of adding the scientific form of a number to the Polish notation
						PutInScienceForm(expression, &postfixExpression, &size, &i, wrongs, &previous); 
						if (*wrongs > ERROR_NO) { // an error was found in the processing function, which means that we are interrupting the program
							FreeStackChar(symbolsStack, process);
							return postfixExpression;
						}
						numberCheck.pointQuan += 1;
						numberCheck.numIndic = CALC_TRUE;
						previous.lastSym = SYM_SCIENCE;
						previous.lastOper = OPER_NO;
					}
					else {
						if ((expression[i] == LETTER_E_SMALL) || ((expression[i] == LETTER_PI_FIRST) && (expression[i + 1] == LETTER_PI_SECOND))) {
							if ((previous.lastOper == OPER_NO) && (startExpr == CALC_TRUE)) { // if either of the two constants is entered repeatedly, error is returned
								*wrongs = ERROR_INVALID_INPUT;
								FreeStackChar(symbolsStack, process);
								return postfixExpression;
							}
							postfixExpression = PutInPostfix(postfixExpression, expression[i], &size, wrongs);
							if (*wrongs == ERROR_MEMORY_STACK) { // error, not enough memory to build Polish notation
								FreeStackChar(symbolsStack, process);
								return postfixExpression;
							}
							if (expression[i] == LETTER_PI_FIRST) {
								i += 1;
								postfixExpression = PutInPostfix(postfixExpression, expression[i], &size, wrongs);
								if (*wrongs == ERROR_MEMORY_STACK) { 
									FreeStackChar(symbolsStack, process);
									return postfixExpression;
								}
							}
							previous.lastOper = OPER_NO;
							previous.lastSym = SYM_CONST;
							numberCheck.numIndic = CALC_TRUE;
						}
						else {
							if ((check.valRegime == CALC_TRUE) && ((previous.lastOper == OPER_NO)) && (previous.lastSym != SYM_SPACE) ||
								((previous.lastSym != SYM_NO) && (previous.lastSym != SYM_SPACE))) {
								*wrongs = ERROR_VARIABLES;
								FreeStackChar(symbolsStack, process);
								return postfixExpression;
							}
							iPoint = i;
							opSetStr = FindOperOfAll(expression, &i, startExpr, &previous, wrongs);
							if (*wrongs == ERROR_INVALID_OPERATION) { // processing variables if no operation is found
								NumberErrorProcessing(&previous, &startExpr, &numberCheck, &check, wrongs);
								if (*wrongs > ERROR_NO) {
									FreeStackChar(symbolsStack, process);
									return postfixExpression;
								}
								i = iPoint;
								*wrongs = ERROR_NO;
								ProcessingVariables( &postfixExpression, &size, &process, expression[i], &numOfVariables, &varSize, wrongs);
								if (*wrongs > ERROR_NO) {
									FreeStackChar(symbolsStack, process);
									return postfixExpression;
								}
								check.valRegime = CALC_TRUE;
								numberCheck.numIndic = CALC_TRUE;
								numberCheck.pointQuan += 1;
								previous.lastOper = OPER_NO;
								previous.lastSym = SYM_VARIABLE;
							}
							else {
								if (IsBinar(opSetStr) == CALC_TRUE) { // previous operation in the string was unary
									current = OPER_BINAR;
								}
								else {
									current = OPER_UNAR;
								}
								if ((current == OPER_UNAR) && (isAnyNumber == CALC_TRUE) && (previous.lastOper == OPER_NO)) {
									*wrongs = ERROR_INVALID_INPUT;
									FreeStackChar(symbolsStack, process);
									return postfixExpression;
								}
								if (opSetStr != LOG) {
									OperationComparsion(&postfixExpression, &size, &opSetStr, symbolsStack, &current, wrongs);
									if (*wrongs > ERROR_NO) {
										FreeStackChar(symbolsStack, process);
										return postfixExpression;
									}
									previous.lastSym = SYM_NO;
								}
								else {
									logOper.numOfLogs += 1;
									previous.lastSym = SYM_LOG;
								}
								previous.lastOper = current;
								numberCheck.numIndic = CALC_FALSE;
							}
						}
					}
					startExpr = CALC_TRUE;
					isAnyNumber = CALC_TRUE;
				}
				else
					if (expression[i] == LETTER_VARIABLES_END) {
						EndOfVariablesRegime(&postfixExpression, &size, &check, wrongs, &previous); // end of the variable mode, you need to remember
						if (*wrongs > ERROR_NO){
							FreeStackChar(symbolsStack, process);
							return postfixExpression;
						}
						ReleaseStack(symbolsStack, &postfixExpression, &size, wrongs);
						if (*wrongs > ERROR_NO) {
							FreeStackChar(symbolsStack, process);
							return postfixExpression;
						}
						startExpr = CALC_FALSE;
						isAnyNumber = CALC_FALSE;
						numberCheck.numIndic = CALC_FALSE;
						numberCheck.pointQuan = 0;
					}
					else {
						opSetStr = FindOperOfAll(expression, &i, startExpr, &previous, wrongs);
						if (*wrongs == ERROR_INVALID_OPERATION) {
							FreeStackChar(symbolsStack, process);
							return postfixExpression;
						}
						OperationErrorProcessing(&opSetStr, &check, &previous, &startExpr, wrongs);
						if (*wrongs > ERROR_NO) {
							FreeStackChar(symbolsStack, process);
							return postfixExpression;
						}
						if (opSetStr == COMMA) {
							logOper.numOfCommas += 1;
						}
						if (opSetStr == ASSIGNMENT) {
							process[numOfVariables - 1].initialized = CALC_TRUE;
							check.valAssign = CALC_TRUE;
						}
						ProcessingOperation(&postfixExpression, &size, &opSetStr, &previous, symbolsStack, &isAnyNumber, wrongs);
						if (*wrongs > ERROR_NO) {
							FreeStackChar(symbolsStack, process);
							return postfixExpression;
						}
						previous.lastSym = SYM_NO;
						startExpr = CALC_TRUE;
						check.valEnd = CALC_FALSE;
						numberCheck.numIndic = CALC_FALSE; 
						// if a sign is found, it means that the operand has ended, the search for the point starts from the beginning
						numberCheck.pointQuan = 0;
					}
		}
		else {
			previous.lastSym = SYM_SPACE; // last character read was a space character 
		}
		i += 1;
	}
	EndErrorChecking(&logOper, process, &numOfVariables, &previous, &check, wrongs);
	if (*wrongs > ERROR_NO) {
		FreeStackChar(symbolsStack, process);
		return postfixExpression;
	}
	ReleaseStack(symbolsStack, &postfixExpression, &size, wrongs);
	if (*wrongs > ERROR_NO) {
		FreeStackChar(symbolsStack, process);
		return postfixExpression;
	}
	if (process != NULL) {
		for (i = 0; i < numOfVariables; i++) {
			process[i].initialized = CALC_FALSE;
		}
		*arrOfValues = process;
	}
	postfixExpression[size - 1] = 0;
	free(symbolsStack->value);
	free(symbolsStack);
	return postfixExpression; // returning a pointer to a postfix expression
}