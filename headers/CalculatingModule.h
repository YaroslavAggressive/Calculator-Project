#ifndef CALCULATING_MODULE_INCLUDED
#define CALCULATING_MODULE_INCLUDED
#pragma once

#include "StackDoubleFunctions.h"
// constants definitions (they can also be used without these macro substitutions by connecting the built - in library limits. h)
#define e 2.71828182845904523
#define pi 3.1415926535897932 

// a block of functions for directly calculating the value of an expression
double CountResult(const char* post_notation, error_t* wrongs, stack_variables_t* arrOfValues);
double CalculateBinary(double a, double b, operation_t opr, error_t* wrongs);
double CalculateUnar(double a, operation_t opr, error_t* wrongs);
void PutScienceFormIntoStack(stack_double_t* doubleStack, const char* expression, int* i);

#endif