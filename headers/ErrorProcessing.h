#ifndef ERROR_PCESSING_INCLUDED
#define ERROR_PCESSING_INCLUDED
#pragma once

// user type, which stores the error code (defined by the programmer) returned from the main functions of the program to the interface module
typedef enum {
	ERROR_NO = -1, // no errors in the string. It is made negative in order to distinguish it by one logical expression from any error
	ERROR_WRITTING_OPERAND = 0,  // error writing operands (separated by a space, with letters of different alphabets, etc.)
	ERROR_NOT_ENOUGH_OPERANDS, // a type of input error, a lack of operands
	ERROR_INVALID_OPERATION,  // operation definition error
	ERROR_PROBLEMS_WITH_POINT,  // error entering the fractional part of the number, iterating through the points
	ERROR_SCIENCE_FORM, // error entering the scientific form of the number
	ERROR_BRACKETS, // error entering brackets
	ERROR_UNACCEPTABLE_VALUES, // DL error. The specified operations cannot be calculated for these operands
	ERROR_INCORRECT_CONSTANTS, // error entering predefined constants
	ERROR_RUSSIAN_COMMENTS, // error with the alphabet, characters other than Latin
	ERROR_INVALID_INPUT, // error entering the expression. Incorrect order of operations, problems with entering operands, etc.
	ERROR_MEMORY_LINE, // error allocating memory for a string read from a stream before processing it and counting the results
	ERROR_MEMORY_STACK, // error allocating memory for all kinds of stacks
	ERROR_VARIABLES, // error working with the variable mode
	ERROR_FILE // error opening a file if it is submitted as an argument to the main function (the file is missing, or its name is entered incorrectly)
} error_t;

// function for displaying the result to the user
void PrintMistake(char const* input, error_t* mistake);

#endif