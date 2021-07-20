// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ErrorProcessing.h"
#include "OperationsWithVariables.h"
#include "SupportingFunctions.h"
#include "BuildingNotation.h"
#include "CalculatingModule.h"
#include <stdio.h>
#include <malloc.h>

int main(int argc, char* argv[]) {
	FILE* flow = NULL;
	error_t mistake = ERROR_NO; // variables with error code in case of unexpected exception
	char* input = NULL;
	char* preOperation = NULL;
	double result;
	stack_variables_t* arrOfValues = NULL;
	bool_t flag = CALC_FALSE, file = CALC_FALSE;// for empty and space strings and comments
	context_t flowEnd = SCENARIO_CONTINUE;// need for separating file stream and command window stream 
	if (argc > 2) { // too many arguments for our program
		mistake = ERROR_FILE;
		PrintMistake(input, &mistake); // we print the corresponding error
		return 0;
	}
	else
	if (argc == 2) { // everything is ok, but we will read the expressions from the file
		freopen_s(&flow, argv[1], "r", stdin);
		if (flow == NULL) { // cannot find entered file
			mistake = ERROR_FILE;
			PrintMistake(input, &mistake);
			return 0;
		}
		file = CALC_TRUE; // there was a text file at the input, so it will have to be closed at the output
	}
	else {
		flow = stdin; // no inpuet project arguments were found, so значит, continue reading from stdin
	}
	input = ReadLine(flow, &flowEnd, &mistake); // read first line, manually processing first step of cycle
	while (flowEnd != SCENARIO_STREAM_END) { // condition for exiting the loop is the appearance of end of stream in new line
		if (mistake == ERROR_NO) {
			flag = IfSpaceOrComment(input); // empty string or comment checking
			if (flag == CALC_TRUE) { // a comment line or a space line - we immediately output it to the user
				printf("%s\n", input);
			}
			else {
				preOperation = BuildNotation(input, &mistake, &arrOfValues); // we get a string in Polish notation
				if (mistake == ERROR_NO) { 
					result = CountResult(preOperation, &mistake, arrOfValues); // calculating result
					if (mistake == ERROR_NO) { 
						printf("%s == %g\n", input, result); // output the result, if there is no error
					}
					else {
						PrintMistake(input, &mistake);
					}
				}
				else {
					PrintMistake(input, &mistake);
				}
				free(preOperation);
				preOperation = NULL;
			}
		}
		else {
			PrintMistake(input, &mistake); // printing error to user
		}
		if (input != NULL) { // checking, if line hasn't been cleared before
			free(input);
			input = NULL;
		}
		mistake = ERROR_NO; // we reset the error variable, since we still have to look for them in new expressions
		if (flowEnd != SCENARIO_LAST_EXPRESSION) { // if it's not the last line in stream
			input = ReadLine(flow, &flowEnd, &mistake); 
		}
		else {
			break;
		}
	}
	if (mistake != ERROR_NO) {
		PrintMistake(input, &mistake);
	}
	if (input != NULL) {
		free(input);
	}
	if (file == CALC_TRUE) {
		fclose(flow);
	}
	return 0;
}