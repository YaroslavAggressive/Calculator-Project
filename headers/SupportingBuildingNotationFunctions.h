#ifndef BUIDING_NOTATION_SUPPORTING_INCLUDED
#define BUIDING_NOTATION_SUPPORTING_INCLUDED
#pragma once

// block of functions allocated separately from other modules for better readability of the program
void PopStackByOpenBrac(char** postfixExpression, int* size, string_elems_t* previous, error_t* wrongs, stack_t* symbolsStack, bool_t* isAnyNumber);
void ProcessingOperation(char** postfixExpression, int* size, operation_t* opSetStr, string_elems_t* previous, stack_t* symbolsStack, bool_t* isAnyNumber, error_t* wrongs);
void OperationComparsion(char** postfixExpression, int* size, operation_t* opSetStr, stack_t* symbolsStack, operation_info_t* current, error_t* wrongs);
void ProcessingVariables(char** postfixExpression, int* size, stack_variables_t** process, char comparingSym, int* numOfVariables, int* varSize, error_t* wrongs);

#endif

// comments remastered