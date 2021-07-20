#ifndef BUILDING_NOTATION_MODULE_INCLUDED
#define BUILDING_NOTATION_MODULE_INCLUDED
#pragma once

#include "StackCharFunctions.h"
#include "OperationsWithVariables.h"
#include "ErrorProcessing.h"
#include "SupportingFunctions.h"

#define POSTFIXSIZE 10 // initial size of a new line in the form of a postfix notation

// defining macro-priority substitutions corresponding to various operations
#define PRIORITY_HIGHEST 5 // bracket priority
#define PRIORITY_UNAR_OPERATIONS 4 // priority for unary operations
#define PRIORITY_DEGREE 3 // priority separately for exponentiating a number
#define PRIORITY_AVERAGE 2 // priority for division and multiplication
#define PRIORITY_BASE_OPERATIONS 1 // priority of basic binary operations - addition and subtraction
#define PRIORITY_LOWER 0 // priority for the natural base logarithm operation and assigning a value to a variable

// defining macro substitutions of character constants for translating operations into a single-character " stack format"
#define LETTER_OPEN_BRACKET '('
#define LETTER_CLOSE_BRACKET ')'
#define LETTER_DIVISION '/'
#define LETTER_MULTIPLICATION '*'
#define LETTER_DEGREE '^'
#define LETTER_ADDITION '+'
#define LETTER_SUBTRACTION '-'
#define LETTER_COS 'C'
#define LETTER_SIN 'S'
#define LETTER_FLOOR 'F'
#define LETTER_CEIL 'D'
#define LETTER_TG 'T'
#define LETTER_SQRT 'Q'
#define LETTER_LN 'L'
#define LETTER_ARCSIN 'R'
#define LETTER_ARCCOS 'N'
#define LETTER_ARCTG 'G'
#define LETTER_CTG 'Z'
#define LETTER_UNAR_MINUS 'U'
#define LETTER_ASSIGNMENT '='
#define LETTER_COMMA 'P' // this is the same notation for an arbitrary logarithm, since in fact it is one operation
#define LETTER_COMMA_EXPRESSION ',' // separation of arguments of an arbitrary logarithm at the input to the program from the user
#define LETTER_DASH 'E'
#define LETTER_SPACE ' ' // for dividing lexemes in a new line of postfix notation
#define LETTER_VARIABLES 'v' // variable label in postfix notation
#define LETTER_POINT '.'
#define LETTER_E_SMALL 'e' // designation of both the exponent and the scientific form of the record
#define LETTER_E_BIG 'E' // alternative designation of the scientific form of the record
#define LETTER_PI_FIRST 'p' // first symbol of Pi
#define LETTER_PI_SECOND 'i' // second symbol of Pi
#define LETTER_VARIABLES_END ';' // end of variable declaration mode
#define LETTER_SCIENCE_FORM 's' // marking a number in the scientific form of writing in postfix notation - a new line

typedef int priority_t; // type for the priority of operations, so as not to get confused in the program code

// type of all operations, that calculator can find in expressions
typedef enum {
	ADDITION = 0, 
	SUBTRACTION, 
	DIVISION, 
	MULTIPLICATION,	
	DEGREE,	
	OPEN_BRACKET, 
	CLOSE_BRACKET,  
	// then there are unary operations , with a higher priority and all right-associative ones
	UNAR_MINUS,	// unary minus-the operation of changing the sign of a number
	SIN, 
	COS, 
	ARCCOS,	
	ARCSIN,	
	TG,	
	CTG, 
	ARCTG, 
	LN,	
	FLOOR, // operation of finding the rounding of the fractional part of a number in a smaller direction
	CEIL, // operations for finding the rounding of the fractional part of a number up
	SQRT, // operation of finding the square root of a number
	// operations in the variable storage mode
	ASSIGNMENT,	// assigning a value to a variable in the local storage mode
	// the only operation on C5
	LOG, // we enter the logarithm in the list of possible operations, but we do not enter it in the Polish notation
	COMMA // binary comma operation. In fact, this is the operation of finding the logarithm
} operation_t;

// integer type created to remember the last operation encountered in the string
typedef enum {
	OPER_NO = 0, // the last character in the string was a number, a constant or a variable
	OPER_BINAR, // last element encountered in the line is an operation. This last operation in the string is binary
	OPER_UNAR, // last element encountered in the line is an operation. This last operation in the string is unary
	OPER_OPEN_BRAC // last element encountered in the line is an operation. This last operation in the line is exactly the opening (!) bracket
} operation_info_t;

// the type created to remember the last character (not operation!)
typedef enum {
	SYM_NO = 0, // means that the previous character was an operation that opens a bracket, the beginning of a string, or the end of a variable mode
	SYM_CONST, // previous element of the string is one of the constants e or pi
	SYM_NUMBER, // last element of the string is a number (digit or dot)
	SYM_SPACE, // previous symbol was "space"
	SYM_SCIENCE, // indicator of presence of a scientific form of writing a number with the last encountered symbol
	SYM_VARIABLE, // last character (element) strings are a variable
	SYM_LOG // last element of the string is the logarithm (sequence "log")
} symbol_t;

// a type created for tracking the main elements of storing variables by line
typedef struct {
	bool_t valEnd; // indicator for closing the variable storage mode
	bool_t valRegime; // indicator of operation in variable mode
	bool_t valAssign; // indicator of the location of the assignment operation in the line
} values_check_t;

// type that includes two integer fields-they count the number of logarithms and the number of commas (for tracking errors)
typedef struct {
	int numOfLogs; // counter of "log" - current symbol sequences in input expression
	int numOfCommas; // the comma counter in the logarithm itself
} log_t;

// structure that keeps symbol and operation in types, initialised above
typedef struct {
	symbol_t lastSym; // symbol (current or previous)
	operation_info_t lastOper; // operation (current or previous)
} string_elems_t;

// structure for keeping data about the presence of an operand in the string and the correctness of its input
typedef struct {
	bool_t numIndic; // indicator of the presence of a number by the last element
	int pointQuan; // counter for the number of points, so as not to enter too many of them and correctly track the error
} num_check_t;

// block of functions for forming postfix notation
char* PutInPostfix(char* notation, char symbol, int* size, error_t* wrongs);
void PutInScienceForm(char const* expression, char** postfixExpression, int* size, int* i, error_t* wrongs, string_elems_t* previous);
void ReleaseStack(stack_t* symbolsStack, char** postfixExpression, int* size, error_t* wrongs);
char* BuildNotation(const char* expression, error_t* wrongs, stack_variables_t** arrOfValues);

priority_t ChoosePriority(operation_t sign); 
char OperToSymbol(operation_t stackOper);
operation_t FindOperOfAll(const char* expression, int* i, bool_t startExpr, string_elems_t* previous, error_t* wrongs);
operation_t FindOperOfAllStack(char symbol);
bool_t IsBinar(operation_t stackOper);

#endif