// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ErrorProcessing.h"
#include <stdio.h>

// the function of printing an error message (at the request of the programmer) by the error code itself
// input data - source string and a pointer to the variable with the error code
void PrintMistake(char const* input, error_t* mistake) { 
	switch (*mistake) {
		case ERROR_WRITTING_OPERAND:
			printf("%s == ERROR: wrong way of writing operands\n", input);
			break;
		case ERROR_BRACKETS:
			printf("%s == ERROR: too much or not enough brackets\n", input);
			break;
		case ERROR_UNACCEPTABLE_VALUES:
			printf("%s == ERROR: invalid value of input arguments\n", input);
			break;
		case ERROR_INVALID_OPERATION:
			printf("%s == ERROR: undefined operation\n", input);
			break;
		case ERROR_SCIENCE_FORM:
			printf("%s == ERROR: incorrect input of number in science form\n", input);
			break;
		case ERROR_INCORRECT_CONSTANTS:
			printf("%s == ERROR: incorrect constants\n", input);
			break;
		case ERROR_PROBLEMS_WITH_POINT:
			printf("%s == ERROR: too much points\n", input);
			break;
		case ERROR_NOT_ENOUGH_OPERANDS:
			printf("%s == ERROR: not enough operands\n", input);
			break;
		case ERROR_INVALID_INPUT:
			printf("%s == ERROR: incorrect input, rewrite expression\n", input);
			break;
		case ERROR_MEMORY_LINE:
			printf("ERROR: not enough memory\n");
			break;
		case ERROR_MEMORY_STACK:
			printf("%s == ERROR: not enough memory\n", input);
			break;
		case ERROR_VARIABLES:
			printf("%s == ERROR: problems with variables regime\n", input);
			break;
		case ERROR_FILE:
			printf("ERROR: problems with file\n");
			break;
		case ERROR_RUSSIAN_COMMENTS:
			printf("%s == ERROR: invalid symbols, maybe from different language\n", input);
			break;
		default:
			*mistake = ERROR_NO;
			break;
	}
}