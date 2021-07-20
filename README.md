# Calculator-Project

Description
-----------

The programe that calculates string expressions, supports brackets and handles errors


1.Technical description
---------------------

#### 1.1 Programming language:
  ANSI C, 1989 standard, plus Microsoft additions;
#### 1.2 Algorithm:
  Conversion to inverse Polish notation with its calculation;
  
  
2.Form of testing program.
--------------------------

#### 2.1 Program - a console application (Win32 Console Application) launched from the command line

##### 2.1.1 If the program is started without parameters, input data is read from the standard stream, line by line, to the end of the stream.
##### 2.1.2 If the program is started with a single parameter (file name), input data is read from this text file, line by line, to the end (of stream)
##### 2.1.3 If the program is launched without parameters, but with input and/or output redirection, it works similarly to the previous two launch methods
##### 2.1.4 The output is carried out in the standard output stream. One read line corresponds to one printed line, completed with a line feed.
##### 2.1.5 Read empty lines and comment lines are output unchanged. An empty string is a string containing zero or more whitespace characters that is not located at the end of the file A comment line is a line that begins with two slashes “//” and contains any text. The string can contain whitespace characters before the beginning of the comment, but not other text.
##### 2.1.6 The remaining strings are treated as expressions and are immediately evaluated. The original formula is output unchanged and, in the same line, the numerical value of the result (or a brief description of the error). The original formula is separated from the result (or error) by a double equal sign ( = = ), framed by spaces.
##### 2.1.7 The output format corresponds to the specifier "%g " (final zeros are not output, if necessary, the scientific form of the record is used);
##### 2.1.8 If an error occurs during the processing of an expression, a message about it is output instead of the result (after the equal sign). It begins with the phrase " ERROR:", followed by a space-separated explanation of the problem. The program operation is not interrupted at the same time.
##### 2.1.9 There is no explicit or implicit limit on the length of the input (and output) string. The program correctly reads and processes strings of any length.


