// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "SupportingFunctions.h"
#include "BuildingNotation.h"
#include "ErrorProcessing.h"
#include <malloc.h>
#include <ctype.h>

// binary operation validation. Input data - user class of operation type
bool_t IsBinar(operation_t stackOper) {
	if ((stackOper == DIVISION) || (stackOper == ADDITION) || (stackOper == SUBTRACTION) || (stackOper == MULTIPLICATION) ||
		(stackOper == DEGREE) || (stackOper == ASSIGNMENT) || (stackOper == LOG) || (stackOper == COMMA)) {
		return CALC_TRUE;
	}
	else {
		return CALC_FALSE;
	}
}

// function for chching whether a symbol belongs to a set of digits
bool_t IsNumeral(char symbol) { // input data - char-symbol
	if (((int)symbol <= NUMERAL_UP) && ((int)symbol >= NUMERAL_DOWN)) {
		return CALC_TRUE;
	}
	else {
		return CALC_FALSE;
	}
}

// function of reading line from input stream for further processing and evaluating expressionn
// Input data - stream pointer (stdin or some file), end-of-file indicator pointer and pointer
// on variable with error code. Output - pointer to the read string
char* ReadLine(FILE* flow, context_t* flowEnd, error_t* wrongs) {
	char* strDelta = NULL; // we enter a new pointer so that at each new step of the loop we can save the data in memory that we are trying to expand
	int letter = 0;
	char* str = NULL; //  pointer that, if there are no errors, will return the function to the programmer in the main program
	int i = 0; //counter of the number of characters in a string is needed for correct allocation of the required amount of memory
	letter = getc(flow);
	while ((letter != '\n') && (letter != (EOF))) { // we read from stream either to end of line, or to the end of file, if line is one or the last in stream
		strDelta = realloc(str, (i + 1) * sizeof(char));
		if (strDelta == NULL) { //checking for a memory allocation error
			*wrongs = ERROR_MEMORY_LINE; // error indicator
		}
		else { 
			str = strDelta; // memory allocation completed, so we can use a pointer to it
			str[i] = (char)letter; // write the curr element of the string to the extended string
			i += 1;
		}
		letter = getc(flow); // char-by-char reading from the stream
	}
	if (*wrongs == ERROR_MEMORY_LINE) { // memory error during reading
		if (letter == (EOF)) {
			*flowEnd = SCENARIO_STREAM_END; //  end of the file was found, there was not enough memory for the last line in the stream
		}
		else {
			*flowEnd = SCENARIO_CONTINUE; // end of the file was not found, we can continue working with the following lines from the stream
		}
		free(str);
		return NULL;
	}
	if (letter == (EOF)) {
		if (i == 0) {
			*flowEnd = SCENARIO_STREAM_END; // end of the stream was found, not at the end of the last line being read
			return NULL;
		}
		else {
			*flowEnd = SCENARIO_LAST_EXPRESSION; // case when the last line in the stream ends with the end of the stream itself, without a newline
		}
	}
	strDelta = realloc(str, (i + 1) * sizeof(char)); //we separately allocate memory for \n
	if (strDelta == NULL) {
		*wrongs = ERROR_MEMORY_LINE;
		free(str);
		return NULL;
	}
	str = strDelta;
	str[i] = 0;  // we write "\n" to the end of our string to know where to read the characters
	return str; // we return the pointer to the read string
}

// the function of checking the string for whether it is a space or a comment
bool_t IfSpaceOrComment(char const* str) {
	int i = 0;
	unsigned char symbol = (unsigned char)str[i]; // in this character we will store the current element of the string
	// reading from stream all the whitespace characters coming first, in order to either understand that it is whitespace, or find non-whitespace characters
	while ((isspace(symbol) != 0) && (symbol != 0)) { 
		i += 1;
		symbol = (unsigned char)str[i];
	}
	// two fractional lines in a row, usually denoting division-a sign of the beginning of the comment
	if ((str[i] == LETTER_DIVISION) && (str[i + 1] == LETTER_DIVISION) || (str[i] == 0)) { 
		return CALC_TRUE; 
	}
	else {
		// if there aren't two slashes immediately after whitespace characters, it means that this is not a comment, and need to treat the string as expression
		return CALC_FALSE;
	}
}

// function for converting an operation in an expression to a character that defines the operation in Polish notation
char OperToSymbol(operation_t stackOper) {
	char symbol;
	switch (stackOper) {
		case OPEN_BRACKET:
			symbol = LETTER_OPEN_BRACKET;
			break;
		case CLOSE_BRACKET:
			symbol = LETTER_CLOSE_BRACKET;
			break;
		case DIVISION:
			symbol = LETTER_DIVISION;
			break;
		case MULTIPLICATION:
			symbol = LETTER_MULTIPLICATION;
			break;
		case DEGREE:
			symbol = LETTER_DEGREE;
			break;
		case ADDITION:
			symbol = LETTER_ADDITION;
			break;
		case SUBTRACTION:
			symbol = LETTER_SUBTRACTION;
			break;
		case COS:
			symbol = LETTER_COS;
			break;
		case SIN:
			symbol = LETTER_SIN;
			break;
		case FLOOR:
			symbol = LETTER_FLOOR;
			break;
		case CEIL:
			symbol = LETTER_CEIL;
			break;
		case TG:
			symbol = LETTER_TG;
			break;
		case SQRT:
			symbol = LETTER_SQRT;
			break;
		case LN:
			symbol = LETTER_LN;
			break;
		case ARCSIN:
			symbol = LETTER_ARCSIN;
			break;
		case ARCCOS:
			symbol = LETTER_ARCCOS;
			break;
		case ARCTG:
			symbol = LETTER_ARCTG;
			break;
		case CTG:
			symbol = LETTER_CTG;
			break;
		case UNAR_MINUS:
			symbol = LETTER_UNAR_MINUS;
			break;
		case ASSIGNMENT:
			symbol = LETTER_ASSIGNMENT;
			break;
		case COMMA:
			symbol = LETTER_COMMA;
			break;
		default:
			symbol = LETTER_DASH;
			break;
	}
	return symbol;
}

// function for getting an operation by its symbol in Polish notation or stack
static operation_t ChooseOper(const char* expression, int* i, error_t* wrongs) {
	operation_t back = ADDITION;
	*wrongs = ERROR_INVALID_OPERATION;
	// checking for the presence of a logarithm operation at the current stage of string processing
	if ((expression[*i] == 'l') && (expression[*i + 1] == 'o') && (expression[*i + 2] == 'g')) {
		back = LOG;
		*wrongs = ERROR_NO;
		*i += 2;
	}
	else{
		switch (expression[*i]) {
			case LETTER_OPEN_BRACKET:
				back = OPEN_BRACKET;
				*wrongs = ERROR_NO;
				break;
			case LETTER_CLOSE_BRACKET:
				back = CLOSE_BRACKET;
				*wrongs = ERROR_NO;
				break;
			case LETTER_SUBTRACTION:
				back = SUBTRACTION;
				*wrongs = ERROR_NO;
				break;
			case LETTER_ADDITION:
				back = ADDITION;
				*wrongs = ERROR_NO;
				break;
			case LETTER_MULTIPLICATION:
				back = MULTIPLICATION;
				*wrongs = ERROR_NO;
				break;
			case LETTER_DEGREE:
				back = DEGREE;
				*wrongs = ERROR_NO;
				break;
			case LETTER_DIVISION:
				back = DIVISION;
				*wrongs = ERROR_NO;
				break;
			case LETTER_ASSIGNMENT:
				back = ASSIGNMENT;
				*wrongs = ERROR_NO;
				break;
			case LETTER_COMMA_EXPRESSION:
				back = COMMA;
				*wrongs = ERROR_NO;
				break;
			default:
				*wrongs = ERROR_INVALID_OPERATION;
				break;
		}
	}
	return back;
}

// determining the priority of the current operation
// input data - current processing operation
priority_t ChoosePriority(operation_t sign) {
	priority_t main = PRIORITY_LOWER;
	switch (sign) {
		case OPEN_BRACKET:
		case CLOSE_BRACKET:
			main = PRIORITY_HIGHEST; // highest priority of operations defined in operation_t
			break;
		case LN: 
		case TG: 
		case CTG: 
		case SIN: 
		case UNAR_MINUS: 
		case ARCTG: 
		case SQRT: // square root of a number
		case CEIL: // rounding up to an integer
		case FLOOR: // rounding down to an integer
		case COS:
		case ARCSIN:
		case ARCCOS:
			main = PRIORITY_UNAR_OPERATIONS; // highest priority after brackets
			break;
		case DEGREE: // raising number to a power
			main = PRIORITY_DEGREE; // priority of exponentiation is higher, than priority of binary operations
			break;
		case MULTIPLICATION: 
		case DIVISION: 
			main = PRIORITY_AVERAGE; // priority must be superior to addition-subtraction operations
			break;
		case ADDITION: 
		case SUBTRACTION: 
			main = PRIORITY_BASE_OPERATIONS;
			break;
		default: // value of priority for operations ASSIGNMENT and COMMA(comma, in this case, is essentially a log operation on an arbitrary basis)
			// priority of these two operations is the lowest, since:
			// 1. Assignment operation should be taken from stack last, since after it the original expression itself begins, value of which is mapped in the program to a variable
			// 2. We take logarithm of values of the expressions to the right and left of comma, that is, in fact, it shouldn't affect construction of the string in any way
			main = PRIORITY_LOWER; 
			break;
	}
	return main;
}

// function for searching for unary operations(from a given list) in the original expression
// input data - pointer to original expression, index of the element that is being manipulated, a pointer to the variable that stores the error code 
 static operation_t ChooseUnarOper(const char* expression, int* i, error_t* wrongs, operation_t output) { 
	operation_t stackOper = output; // variable that returns the current unary operation to the main function
	int j = *i;
	if (expression[j] == LETTER_SUBTRACTION) {
		stackOper = UNAR_MINUS;
		*wrongs = ERROR_NO;
	}
	else
		if ((expression[j] == 's') && (expression[j + 1] == 'i') && (expression[j + 2] == 'n')) { 
			stackOper = SIN;
			*i += 2; // shift the main counter of following the line, so as not to process already viewed characters more than 1 time
			*wrongs = ERROR_NO;
		}
		else
			if ((expression[j] == 'c') && (expression[j + 1] == 'o') && (expression[j + 2] == 's')) { 
				stackOper = COS;
				*i += 2;
				*wrongs = ERROR_NO;
			}
			else
				if ((expression[j] == 'c') && (expression[j + 1] == 't') && (expression[j + 2] == 'g')) { 
					stackOper = CTG;
					*i += 2;
					*wrongs = ERROR_NO;
				}
				else
					if ((expression[j] == 't') && (expression[j + 1] == 'g')) { 
						stackOper = TG;
						*i += 1;
						*wrongs = ERROR_NO;
					}
					else
						if ((expression[j] == 'l') && (expression[j + 1] == 'n')) { 
							stackOper = LN;
							*i += 1;
							*wrongs = ERROR_NO;
						}
						else
							if ((expression[j] == 'a') && (expression[j + 1] == 'r') && (expression[j + 2] == 'c') && (expression[j + 3] == 't') && (expression[j + 4] == 'g')) { 
								stackOper = ARCTG;
								*i += 4;
								*wrongs = ERROR_NO;
							}
							else
								if ((expression[j] == 'a') && (expression[j + 1] == 'r') && (expression[j + 2] == 'c') && (expression[j + 3] == 's') && (expression[j + 4] == 'i') && (expression[j + 5] == 'n')) { 
									stackOper = ARCSIN;
									*i += 5;
									*wrongs = ERROR_NO;
								}
								else
									if ((expression[j] == 'a') && (expression[j + 1] == 'r') && (expression[j + 2] == 'c') && (expression[j + 3] == 'c') && (expression[j + 4] == 'o') && (expression[j + 5] == 's')) { 
										stackOper = ARCCOS;
										*i += 5;
										*wrongs = ERROR_NO;
									}
									else
										if ((expression[j] == 's') && (expression[j + 1] == 'q') && (expression[j + 2] == 'r') && (expression[j + 3] == 't')) { 
											stackOper = SQRT;
											*i += 3;
											*wrongs = ERROR_NO;
										}
										else
											if ((expression[j] == 'c') && (expression[j + 1] == 'e') && (expression[j + 2] == 'i') && (expression[j + 3] == 'l')) { 
												stackOper = CEIL;
												*i += 3;
												*wrongs = ERROR_NO;
											}
											else
												if ((expression[j] == 'f') && (expression[j + 1] == 'l') && (expression[j + 2] == 'o') && (expression[j + 3] == 'o') && (expression[j + 4] == 'r')) {
													stackOper = FLOOR;
													*i += 4;
													*wrongs = ERROR_NO;
												}
												else {
													// it wasnt possible to determine operation, so we determine the error and pass its code by the pointer
													*wrongs = ERROR_INVALID_OPERATION; 
												}
	return stackOper;
}

// search function for operations from the entire list(binary and unary)
// input data - source expression, a pointer to the index of the processed element, a pointer to a variable with an error code
operation_t FindOperOfAll(const char* expression, int* i, bool_t startExpr, string_elems_t* previous, error_t* wrongs) { 
	operation_t output = ADDITION;
	*wrongs = ERROR_INVALID_OPERATION;
	output = ChooseOper(expression, i, wrongs);
	if ((*wrongs == ERROR_INVALID_OPERATION) ||
		((output == SUBTRACTION) && ((previous->lastOper == OPER_BINAR) || (previous->lastOper == OPER_OPEN_BRAC))) ||
		(((previous->lastOper == OPER_UNAR) || (startExpr == CALC_FALSE)) && (output == SUBTRACTION))){
		output = ChooseUnarOper(expression, i, wrongs, output);
	}
	return output;
}

// function for determining an operation by symbol
// input data - symbol (from operations stack)
// output data - operation (in user type)
operation_t FindOperOfAllStack(char symbol) {
	operation_t back = ADDITION;
	switch (symbol) {
		case LETTER_OPEN_BRACKET:
			back = OPEN_BRACKET;
			break;
		case LETTER_CLOSE_BRACKET:
			back = CLOSE_BRACKET;
			break;
		case LETTER_SUBTRACTION:
			back = SUBTRACTION;
			break;
		case LETTER_ADDITION:
			back = ADDITION;
			break;
		case LETTER_MULTIPLICATION:
			back = MULTIPLICATION;
			break;
		case LETTER_DEGREE:
			back = DEGREE;
			break;
		case LETTER_DIVISION:
			back = DIVISION;
			break;
		case LETTER_COS:
			back = COS;
			break;
		case LETTER_SIN:
			back = SIN;
			break;
		case LETTER_FLOOR:
			back = FLOOR;
			break;
		case LETTER_CEIL:
			back = CEIL;
			break;
		case LETTER_TG:
			back = TG;
			break;
		case LETTER_SQRT:
			back = SQRT;
			break;
		case LETTER_LN:
			back = LN;
			break;
		case LETTER_ARCSIN:
			back = ARCSIN;
			break;
		case LETTER_ARCCOS:
			back = ARCCOS;
			break;
		case LETTER_ARCTG:
			back = ARCTG;
			break;
		case LETTER_CTG:
			back = CTG;
			break;
		case LETTER_UNAR_MINUS:
			back = UNAR_MINUS;
			break;
		case LETTER_ASSIGNMENT:
			back = ASSIGNMENT;
			break;
		case LETTER_COMMA:
			back = LOG;
			break;
		default:
			back = ADDITION;
			break;
	}
	return back;
}

