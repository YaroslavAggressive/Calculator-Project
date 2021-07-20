#ifndef SUPPORTING_PROTOTYPES_H_INCLUDED
#define SUPPORTING_PROTOTYPES_H_INCLUDED
#pragma once

#include "ErrorProcessing.h"
#include <stdio.h>

// macro substitutions rom ASCII-table for finding numbers in a string
#define NUMERAL_DOWN 48 // lower limit of the numbers in ASCII table 
#define NUMERAL_UP 57 // upper limit of the numbers in ASCII table 

// macro substitutions for finding Latin characters of both registers in the original expression
#define SYMBOL_SMALL_DOWN 97 // lower limit of lowercase letters in ASCII table 
#define SYMBOL_SMALL_UP 122 // upper limit of lowercase letters in ASCII table 
#define SYMBOL_BIG_DOWN 65 // upper limit of the uppercase letters in ASCII table 
#define SYMBOL_BIG_UP 90 // upper limit of the uppercase letters in ASCII table 

// user boolean type, 0 or 1
typedef enum {
	CALC_FALSE = 0,
	CALC_TRUE
} bool_t;

// type intended to indicate the cases of program termination in the event of a meeting of the end of the thread
typedef enum {
	SCENARIO_CONTINUE = 0, //end of the line is met, without the end of the input, so we continue working with the program
	SCENARIO_STREAM_END, // end of the stream is met - we exit the program
	SCENARIO_LAST_EXPRESSION // end of the stream is met at the end of the last expression/empty line/comment line
} context_t;

// auxiliary routines for the result counting function
bool_t IsNumeral(char symbol);
bool_t IsInAlphabet(char const symbol);

// a block of actions used exclusively for working with I / O (in the interface module)
int IfSpaceOrComment(char const* str);
char* ReadLine(FILE* flow, context_t* flowEnd, error_t* wrongs);

#endif