#ifndef ASSEMBLER_GLOBALS_H

#define ASSEMBLER_GLOBALS_H

#include <string.h>

// array index numbers
#define COMMAND_NUMBER     16
#define SIGN_NUMBER		   13
#define REGISTERS_NUMBER   8
#define PREFIX_NUMBER	   5
#define INSTRUCTION_NUMBER 4
#define INPUTS_NUMBER      4
#define TYPE_NUMBER        3

// Values max length
#define MAXLINE  80
#define MAXLABLE 30

// common use string terminators
#define LINE_TERMINATOR '\n'
#define STRING_TERMINATOR '\0'
#define REGISTER_SIGN 'r'

#define READ          "r"
#define WRITE         "w"

// define the assembler models names
#define ASSEMBLER_PARSER		"ASSEMBLER_PARSER"
#define ASSEMBLER_VALIDATOR		"ASSEMBLER_VALIDATOR"
#define ASSEMBLER_PROCESSOR		"ASSEMBLER_PROCESSOR"
#define ASSEMBLER_FILE_HANDLER "ASSEMBLER_FILE_HANDLER"

// variables for curr file compiling
char* CURR_FILE_NAME;
int   LINE_NUMBER;

// this function must be called in the beggining of assembler compileing
void init_AssemblerGlobals();

#endif