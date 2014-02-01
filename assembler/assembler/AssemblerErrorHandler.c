#include <stdio.h>

#include "AssemblerErrorHandler.h"
#include "assemblerGlobals.h"

int errorCntr = 0;

void init_errors()
{
	Assembler_Error_Code_Table[LABLE_LEN_ERR]				= "lable must be maximum of 30 characters";
	Assembler_Error_Code_Table[LABLE_EMPTY_FIRST_VALUE_ERR] = "Lable first character empty";
	Assembler_Error_Code_Table[LABLE_FIRST_ALPHA_ERR]		= "Lable first character must be alpabatic value";
	Assembler_Error_Code_Table[LABLE_SAVED_SIGN_ERR]		= "Lable contains saved chars ";
	Assembler_Error_Code_Table[LABLE_SAVED_WORD_ERR]		= "- value already defined, cant use saved word as a lable";
	Assembler_Error_Code_Table[UNRECOGNIZE_INSTRUCTION_ERR] = "unrecognized instruction found";
	Assembler_Error_Code_Table[STRING_DEFINE_ERR]			= "string values must be surrended by \"";
	Assembler_Error_Code_Table[STRING_EMPTY_ERR]			= "string values not found";
	Assembler_Error_Code_Table[UNVALID_NUMBER_ERR]			= "unvalid valud found. data contains numbers only";
	Assembler_Error_Code_Table[SPACE_IN_NUMBER_ERR]			= "unexpected space was found. unvalid number";
	Assembler_Error_Code_Table[EXTRA_COMMA_IN_NUMBER_ERR]	= "unexpected comma was found. unvalid number";
	
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

	Assembler_Error_Code_Table[TYPE_NOT_FOUND_ERR]          = "command type not found in command";
	Assembler_Error_Code_Table[TYPE_UNVALID_ERR]            = "command type value unvalid. type can be 0 or 1";
	Assembler_Error_Code_Table[TIME_NOT_FOUND_ERR]          = "command preform times not found in command";
	Assembler_Error_Code_Table[TIME_UNVALID_ERR]            = "command preform times value unvalid. times can be 0 - 3";
}

void log_error(char* source, int errCode, char* value)
{
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

	errorCntr++;
}