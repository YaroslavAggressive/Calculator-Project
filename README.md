# Calculator-Project

Description
-----------

The programe that calculates string expressions, supports brackets and handles errors


1.Technical description
---------------------

#### **1.1** Programming language:
  ANSI C, 1989 standard, plus Microsoft additions;
#### **1.2** Algorithm:
  Conversion to inverse Polish notation with its calculation;
  
  
2.Form of testing program.
--------------------------

#### 2.1 Program - a console application (Win32 Console Application) launched from the command line

##### **2.1.1** If the program is started without parameters, input data is read from the standard stream, line by line, to the end of the stream.
##### **2.1.2** If the program is started with a single parameter (file name), input data is read from this text file, line by line, to the end (of stream).
##### **2.1.3** If the program is launched without parameters, but with input and/or output redirection, it works similarly to the previous two launch methods.
##### **2.1.4** The output is carried out in the standard output stream. One read line corresponds to one printed line, completed with a line feed.
##### **2.1.5** Read empty lines and comment lines are output unchanged. An empty string is a string containing zero or more whitespace characters that is not located at the end of the file A comment line is a line that begins with two slashes “//” and contains any text. The string can contain whitespace characters before the beginning of the comment, but not other text.
##### **2.1.6** The remaining strings are treated as expressions and are immediately evaluated. The original formula is output unchanged and, in the same line, the numerical value of the result (or a brief description of the error). The original formula is separated from the result (or error) by a double equal sign ( = = ), framed by spaces.
##### **2.1.7** The output format corresponds to the specifier "%g " (final zeros are not output, if necessary, the scientific form of the record is used).
##### **2.1.8** If an error occurs during the processing of an expression, a message about it is output instead of the result (after the equal sign). It begins with the phrase " ERROR:", followed by a space-separated explanation of the problem. The program operation is not interrupted at the same time.
##### **2.1.9** There is no explicit or implicit limit on the length of the input (and output) string. The program correctly reads and processes strings of any length.

#### **2.2** Structure of the program

##### **2.2.1** The principle of modularity is observed: the algorithm is logically and physically separated from the user interface.
##### **2.2.2** The computational algorithm is designed as a separate function that accepts a string as input, in which the expression is written ("double CountResult(const char* postfixNotation, error_t* wrongs, stack_variables_t* arrOfValues)").
##### **2.2.3** This main function is engaged in one task — the calculation of the expression, does not read anything from the keyboard and does not display it on the screen. It is divided into auxiliary sub-functions that perform separate subtasks.
##### **2.2.4** The main and all auxiliary functions are placed in separate modules with their own header file.

#### **2.3** Run-time errors

##### **2.3.1** The program is maximally protected from incorrect input data ("foolproof"), input-output errors, as well as from errors that occur during calculations (such as division by zero).
##### **2.3.2** The algorithmic module correctly informs the interface module about the occurrence of an error.
##### **2.3.3** The program is comprehensively tested on various input data.
##### **2.3.4** Cases of memory shortage are correctly handled like other runtime errors. If there is not enough memory during calculations, an error message is displayed instead of the result. If there is not enough memory while reading the input string, an error message (“ERROR: not enough memory”) is displayed instead of the entire expression, and the input data is correctly skipped until the beginning of the next line (containing the next expression). The program does not crash, the lack of memory for one expression does not mean that it will not be enough for the rest.

#### **2.4** The program is checked by a special tool-linter (PVS-Studio), which helps to detect a large number of logical errors and inaccuracies. Most of the identified errors and instabilities (potential errors) have been eliminated, primarily at the High and Medium levels.

3. Functionality of the calculation module
------------------------------------------
