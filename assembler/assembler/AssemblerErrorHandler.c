#include <stdio.h>

#include "AssemblerErrorHandler.h"
#include "assemblerGlobals.h"
#include "AssemblerDictionaries.h"

int errorCntr = 0;

// this function intalize the assemblerErrorHandler and need to be called in the 
// begining of using AssemblerErrorHandler
void init_AssemblerErrorHandler()
{
	Assembler_Error_Code_Table[LABLE_LEN_ERR]				= "lable must be maximum of 30 characters";
	Assembler_Error_Code_Table[LABLE_EMPTY_FIRST_VALUE_ERR] = "Lable first character empty";
	Assembler_Error_Code_Table[LABLE_FIRST_ALPHA_ERR]		= "Lable first character must be alpabatic value";
	Assembler_Error_Code_Table[LABLE_SAVED_SIGN_ERR]		= "Lable contains saved chars ";
	Assembler_Error_Code_Table[LABLE_ALONE_ERR]				= "Lable must be followed by instruction or command ";

	Assembler_Error_Code_Table[LABLE_SAVED_WORD_ERR]		= "value already defined, cant use saved word as a lable";
	Assembler_Error_Code_Table[UNRECOGNIZE_INSTRUCTION_ERR] = "unrecognized instruction found";
	Assembler_Error_Code_Table[STRING_DEFINE_ERR]			= "string values must be surrended by \"";
	Assembler_Error_Code_Table[STRING_EMPTY_ERR]			= "string values not found";
	Assembler_Error_Code_Table[UNVALID_NUMBER_ERR]			= "unvalid valud found. data contains numbers only";
	Assembler_Error_Code_Table[SPACE_IN_NUMBER_ERR]			= "unexpected space was found. unvalid number";
	Assembler_Error_Code_Table[EXTRA_COMMA_IN_NUMBER_ERR]	= "unexpected comma was found. unvalid number";

	Assembler_Error_Code_Table[EMPTY_INSTRUCTION_LABLE_ERR]	= "lable not found. instruction must have lable name";
	Assembler_Error_Code_Table[EMPTY_DATA_ERR]				= "data instruction must have values";
	Assembler_Error_Code_Table[UNRECOGNIZE_SIGN_ERR]		= "unrecognized sign found ";
	Assembler_Error_Code_Table[LINE_LEN_ERR]				= "assambler file line must be maximum of 80 characters";
	Assembler_Error_Code_Table[OPENING_FILE_ERR]			= "error accoured while opening file";
	Assembler_Error_Code_Table[CLOSING_FILE_ERR]			= "error accoured while closing file";

	Assembler_Error_Code_Table[UNVALID_COMMAND_FORMAT_ERR]  = "unvalid command format! command name is followed by /";
	Assembler_Error_Code_Table[UNVALID_OPERAND_NUM_ERR]     = "command operand not found. unmatch operand number for command type";
	Assembler_Error_Code_Table[OPERAND_FORMAT_ERR]          = "command operand not any known foramt of operand";


	Assembler_Error_Code_Table[REGISTER_INPUT_ERR]          = "operand cant be register input for this command";
	Assembler_Error_Code_Table[DIRECT_INPUT_ERR]            = "operand cant be direct input for this command";							  IMMEDIATE_INPUT_ERR,
	Assembler_Error_Code_Table[INDEX_INPUT_ERR]             = "operand cant be index input for this command";
	Assembler_Error_Code_Table[IMMEDIATE_INPUT_ERR]         = "operand cant be immediate input for this command";

	Assembler_Error_Code_Table[TYPE_NOT_FOUND_ERR]          = "command type value not found in command";
	Assembler_Error_Code_Table[TYPE_UNVALID_ERR]            = "command type value unvalid. type can be 0 or 1";
	Assembler_Error_Code_Table[TIME_NOT_FOUND_ERR]          = "command preform times not found in command";
	Assembler_Error_Code_Table[TIME_UNVALID_ERR]            = "command preform times value unvalid. times can be 0 - 3";

	Assembler_Error_Code_Table[SIGN_NOT_FOUND_ERR]          = "symbol not found in file. assuming extern";

	Assembler_Error_Code_Table[MEMORY_LIMITS_ERR]           = "the assembler program reached its memory limits. stoping file compiling";
	Assembler_Error_Code_Table[ENTRY_MISS_LABLE_ERR]        = "no lable found for entry. stoping compiling";
	Assembler_Error_Code_Table[LABLE_IN_USE_ERR]            = "lable already in use";
	
}

// this function write error to output
// input: 1. char* the error source,
//        2. int - error num
//        3. char* - the file error was found
void log_error(const char* source, const int errCode, const char* value)
{
	++errorCntr;

	fprintf(stderr, source);
	fprintf(stderr, ":");
	fprintf(stderr, "%d ", errCode);
	
	if (errCode >= 0 && errCode < ERROR_NUM)
	{
		fprintf(stderr,  "%s", Assembler_Error_Code_Table[errCode]);
	}

	if (*value != STRING_TERMINATOR)
	{
		fprintf(stderr,  " \"%s\"", value);
	}

	fprintf(stderr, " in file \"%s\" - line %d \n", CURR_FILE_NAME, LINE_NUMBER);
}

// this function start file log process (call when starting new file processing)
void log_file_start()
{
	errorCntr = 0;

	fprintf(stderr, "starting compilation for %s...", CURR_FILE_NAME);
	fprintf(stderr, "%c%c" ,LINE_TERMINATOR, LINE_TERMINATOR);
}

// this function close file log process (call when ending curr file processing)
void log_file_end()
{
	// if error were found
	if (errorCntr > 0)
	{
		fprintf(stderr, "%c%c" ,LINE_TERMINATOR, LINE_TERMINATOR);
		fprintf(stderr, "compilation failed for file %s . %d errors found", CURR_FILE_NAME, errorCntr);
	}
	else
	{
		fprintf(stderr, "compilede seccussfully.");
	}

	fprintf(stderr, "%c%c%c%c" ,LINE_TERMINATOR, LINE_TERMINATOR, LINE_TERMINATOR, LINE_TERMINATOR);
}

// this function return the numer of errors for this file
// return int - the num of errors
int check_errors()
{
	return errorCntr;
}