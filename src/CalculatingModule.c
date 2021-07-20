// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "StackDoubleFunctions.h"
#include "BuildingNotation.h"
#include "ErrorProcessing.h"
#include "OperationsWithVariables.h"
#include "CalculatingModule.h"
#include "SupportingFunctions.h"
#include <ctype.h>
#include <math.h>

// function of placing a number written in the scientific form of writing into the stack of numbers 
// input data - string in Polish notation, a pointer to the index of the current character and a pointer to the stack structure
void PutScienceFormIntoStack(stack_double_t* doubleStack, const char* expression, int* i) {
	double power = 0, result = 0;
	int point = *i + 1;
	bool_t isSign = (expression[point] == LETTER_ADDITION) || (expression[point] == LETTER_SUBTRACTION);
	if (isSign == CALC_TRUE) { // if there is a sign in the scientific form, we immediately skip it and the symbols " e " and " E"
		*i += 2;
		for (; (isspace(expression[*i]) == 0) && (expression[*i] != 0); (*i)++) {
			power = power * 10 + (int)expression[*i] - NUMERAL_DOWN;
		}
		switch (expression[point]) {
			case LETTER_ADDITION: // the degree of 10 is negative - we divide the number by 10 to the desired degree
				result = pow(10, power);
				// figure to the scientific form is already written to the stack, just multiply by the corresponding power of 10
				doubleStack->value[doubleStack->pointer - 1] *= result;
				break;
			case LETTER_SUBTRACTION: // the power of 10 is positive - multiply the number by 10 to the desired degree
				result = pow(10, -power);
				doubleStack->value[doubleStack->pointer - 1] *= result;
				break;
			default:
				result = 0;
				break;
		}
	}
	else {
		*i += 1; //skip the "e" and " E"
		for (; (isspace(expression[*i]) == 0) && (expression[*i] != 0); (*i)++) {
			power = power * 10 + (int)expression[*i] - NUMERAL_DOWN; // we add numbers to the desired places in the number
		}
		result = pow(10, power);
		doubleStack->value[doubleStack->pointer - 1] *= result; 
	}
}

// function that calculates all the unary operations indicated in the program
// input data - operands, an operation, and a pointer to a variable with an error code
// output data - the result of the operation
double CalculateBinary(double a, double b, operation_t opr, error_t* wrongs) {
	double result = 0;
	// perform the necessary actions with two double-numbers, depending on the operation
	switch (opr) {
		case (ADDITION):
			result = a + b;
			break;
		case (SUBTRACTION) :
			result = a - b; // a strict order of arguments is required here
			break;
		case (MULTIPLICATION):
			result = a * b;
			break;
		case (DIVISION):
			if (b != 0) {
				result = a / b; // a strict order of arguments is required here
			}
			else {
				*wrongs = ERROR_UNACCEPTABLE_VALUES;
				return result;
			}
			break;
		case (DEGREE):
			result = pow(a, b);
			break;
		case (LOG):
			if ((a > 0) && (b > 0)) {
				result = log(b) / log(a);
			}
			else {
				*wrongs = ERROR_UNACCEPTABLE_VALUES;
				return result;
			}
			break;
		default:
			*wrongs = ERROR_UNACCEPTABLE_VALUES;
			break;
	}
	return result;
}

// function that calculates all the unary operations indicated in the program 
// input data - operand, operation, and pointer to a variable with an error code
// output data - result of the operation
double CalculateUnar(double a, operation_t opr, error_t* wrongs) {
	double result = 0;
	switch (opr) {
		case SQRT:
			if (a >= 0) {
				result = sqrt(a);
			}
			else {
				*wrongs = ERROR_UNACCEPTABLE_VALUES;
			}
			break;
		case LN:
			if (a > 0) {
				result = log(a);
			}
			else {
				*wrongs = ERROR_UNACCEPTABLE_VALUES;
			}
			break;
		case TG:
			if ((sin(a) != 1) && (sin(a) != -1)) {
				result = tan(a);
			}
			else {
				*wrongs = ERROR_UNACCEPTABLE_VALUES;
			}
			break;
		case CTG:
			if ((cos(a) != 1) && (cos(a) != -1)) {
				result = 1 / tan(a);
			}
			else {
				*wrongs = ERROR_UNACCEPTABLE_VALUES;
			}
			break;
		case FLOOR:
			result = floor(a);
			break;
		case CEIL:
			result = ceil(a);
			break;
		case ARCTG:
			result = atan(a);
			break;
		case SIN:
			if ((cos(a) != 1) && (cos(a) != -1)) {
				result = sin(a);
			}
			else {
				result = 0;
			}
			break;
		case COS:
			if ((sin(a) != 1) && (sin(a) != -1)) {
				result = cos(a);
			}
			else {
				result = 0;
			}
			break;
		case ARCSIN:
			if ((a <= 1) && (a >= -1)) {
				result = atan(a / sqrt(1 - a * a));
			}
			else {
				*wrongs = ERROR_UNACCEPTABLE_VALUES;
			}
			break;
		case ARCCOS:
			if ((a <= 1) && (a >= -1)) {
				result = 2 * atan(sqrt((1 - a) / (1 + a)));
			}
			else {
				*wrongs = ERROR_UNACCEPTABLE_VALUES;
			}
			break;
		case UNAR_MINUS:
			result = -a;
			break;
		default:
			result = 0;
			break;
	}
	return result;
}


// main function of finding the result of calculating the entered expression 
// input data - new line (our expression, only in postfix notation), an array of variables and a pointer to the variable error indicator
// output data - numerical result of calculating the expression
double CountResult(const char* postfixNotation, error_t* wrongs, stack_variables_t* arrOfValues) { 
	int i = 0, varNumber = 0, lastUninit = 0; // counter for moving along a line and a counter for the number of a variable in the array
	double intermed; // variable for the intermediate result of calculating the last two operands
	double result = 0; // value returned by the function(aka the last element of the stack)
	stack_double_t* numbersStack = NULL; // a pointer to the stack where we store the operands of the expression
	operation_t curOperStr; // current operation that was read last from the string
	bool_t isBinary = CALC_FALSE; //  binary definition flag/unarity of the operation
	bool_t pointIndic = CALC_FALSE; // indicator of the presence of a point-the beginning of the fractional part of the number
	int numberAfterPoint = 0; // number of digits after the decimal point
	numbersStack = CreateNewResultStack(wrongs);
	if (*wrongs == ERROR_MEMORY_STACK) {
		return result;
	}
	while (postfixNotation[i] != 0){
		if (isspace(postfixNotation[i]) == 0) {
			pointIndic += (postfixNotation[i] == LETTER_POINT);
			if (((int)(postfixNotation[i]) <= NUMERAL_UP) && ((int)(postfixNotation[i]) >= NUMERAL_DOWN) || (postfixNotation[i] == LETTER_POINT)) {
				if (postfixNotation[i] != LETTER_POINT) {
					if (pointIndic > 0) {
						numberAfterPoint += 1;
					}
					PutInStackElem(numbersStack, postfixNotation[i], pointIndic, numberAfterPoint);
				}
				else {
					if ((postfixNotation[i + 1] != LETTER_SCIENCE_FORM) && (IsNumeral(postfixNotation[i + 1]) == CALC_FALSE) &&
						(IsNumeral(postfixNotation[i - 1]) == CALC_FALSE)) {
						numbersStack = GoToNextNumber(numbersStack, wrongs);
						if (*wrongs == ERROR_MEMORY_STACK) {
							break;
						}
					}
				}
				if ((isspace(postfixNotation[i + 1]) != 0) || (postfixNotation[i + 1] == 0)) {
					numbersStack = GoToNextNumber(numbersStack, wrongs);
					if (*wrongs == ERROR_MEMORY_STACK) {
						break;
					}
				}
			}
			else
				if ((postfixNotation[i] == LETTER_PI_FIRST) && (postfixNotation[i + 1] == LETTER_PI_SECOND)) {
					if (postfixNotation[i - 1] == LETTER_POINT) {
						*wrongs = ERROR_INCORRECT_CONSTANTS;
						break;
					}
					numbersStack = GoToNextNumber(numbersStack, wrongs);
					if (*wrongs == ERROR_MEMORY_STACK) {
						break;
					}
					PutNumberInStackDouble(numbersStack, pi);
					i += 1;
				}
				else
					if (postfixNotation[i] == LETTER_E_SMALL) {
						numbersStack = GoToNextNumber(numbersStack, wrongs);
						if (*wrongs == ERROR_MEMORY_STACK) {
							break;
						}
						PutNumberInStackDouble(numbersStack, e);
					}
					else
						if (postfixNotation[i] == LETTER_SCIENCE_FORM) {
							numbersStack = GoToNextNumber(numbersStack, wrongs);
							if (*wrongs == ERROR_MEMORY_STACK) {
								break;
							}
							i += 1;
							PutScienceFormIntoStack(numbersStack, postfixNotation, &i);
							i -= (postfixNotation[i] == 0);
							pointIndic = 0;
							numberAfterPoint = 0;
						}
						else
							if (postfixNotation[i] == LETTER_VARIABLES) {
									i += 1;
									varNumber = (int)(postfixNotation[i]) - NUMERAL_DOWN;
									if (arrOfValues[varNumber - 1].initialized == CALC_TRUE) {
										numbersStack = GoToNextNumber(numbersStack, wrongs);
										if (*wrongs == ERROR_MEMORY_STACK) {
											break;
										}
										PutNumberInStackDouble(numbersStack, arrOfValues[varNumber - 1].value);
									}
									else {
										lastUninit = varNumber;
									}
								}
							else {
								curOperStr = FindOperOfAllStack(postfixNotation[i]);
								isBinary = IsBinar(curOperStr);
								if (CheckDoubleStack(numbersStack) == 0) {
									if (isBinary == CALC_TRUE) {
										intermed = GetLastNumber(numbersStack);
										PopDouble(numbersStack);
										if (curOperStr != ASSIGNMENT) {
											intermed = CalculateBinary(GetLastNumber(numbersStack), intermed, curOperStr, wrongs);
											if (*wrongs > ERROR_NO) {
												break;
											}
											PutNumberInStackDouble(numbersStack, intermed);
										}
										else {
											arrOfValues[lastUninit - 1].value = intermed;
											arrOfValues[lastUninit - 1].initialized = CALC_TRUE;
										}
									}
									else {
										intermed = CalculateUnar(GetLastNumber(numbersStack), curOperStr, wrongs);
										if (*wrongs > ERROR_NO) {
											break;
										}
										PutNumberInStackDouble(numbersStack, intermed);
									}
								}
								else {
									*wrongs = ERROR_VARIABLES;
									break;
								}
							}
		}
		else {
			pointIndic = 0; // separator is found, the point indicator needs to be reset to zero so that you can find a new one if necessary
			numberAfterPoint = 0; 
		}
		i += 1;
	}
	if (*wrongs > ERROR_NO) {
		FreeStackDouble(numbersStack, arrOfValues);
		return result;
	}
	if (CheckDoubleStack(numbersStack) == CALC_FALSE) {
		result = GetLastNumber(numbersStack);
	}
	FreeStackDouble(numbersStack, arrOfValues);
	return result;
}