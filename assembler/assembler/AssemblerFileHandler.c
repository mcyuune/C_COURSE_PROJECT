#include <stdio.h>
#include <stdlib.h>

#include "AsssemblerFileHandler.h"
#include "AssemblerErrorHandler.h"
#include "AssemblerGlobals.h"
#include "AssemblerDictionaries.h"

char* CURR_FILE_NAME;

// this function open curr file name
// return: FILE* - returns open file pointer
FILE* open_file(char* fileMode, char* fileName)
{
	FILE* ft = fopen(fileName, fileMode);

	// TO DO WRITE error
	if (ft == NULL)
	{
		log_error(ASSEMBLER_FILE_HANDLER, OPENING_FILE_ERR, NULL);
	}

	return ft;
}

// this function close the file 
// input:  1. assemblerFile - FILE* - the file pointer
void close_files(FILE* assemblerFile)
{
	if (fclose(assemblerFile) != 0)
	{
		log_error(ASSEMBLER_FILE_HANDLER, CLOSING_FILE_ERR, NULL);
	}
}

// this function generate full file name by adding suffix to the file name
// input:  1. char* fileName - the file name,
//         2. int suffixType - the wanted suffix
// return: char* - full file name
char* generate_file_name(const int suffix_type)
{
	char* fullName = (char*) malloc(strlen(CURR_FILE_NAME) + 
									strlen(Assembler_file_prefix_Code_Table[suffix_type]) + 1);//+1 for the zero-terminator

	strcpy(fullName, CURR_FILE_NAME);
	strcat(fullName, Assembler_file_prefix_Code_Table[suffix_type]);

	return fullName;
}

void write_to_file(int format, FILE* currFile, int address, void* value, 
				   char commandType, int commandCtr, int signCtr)
{
	switch (format)
	{
		case PROGRAM_HEADER_OUTPUT_FORMAT:
		{
			fprintf(currFile, Assembler_output_format_Code_Table[format], commandCtr, signCtr);
			break;
		}
		case PROGRAM_BODY_OUTPUT_FORMAT:
		{
			fprintf(currFile, Assembler_output_format_Code_Table[format], address, 
				(int) *((int*) value), commandType);
			break;
		}
		case PROGRAM_SIGN_OUTPUT_FORMAT:
		{
			fprintf(currFile, Assembler_output_format_Code_Table[format], address, 
				(int) *((int*) value));
			break;
		}
		case ENTRY_FILE_OUTPUT_FORMAT:
		{
			fprintf(currFile, Assembler_output_format_Code_Table[format], 
				 address, ((char*) value)); 
				;
			break;
		}
		case EXTERN_FILE_OUTPUT_FORMAT:
		{
			fprintf(currFile, Assembler_output_format_Code_Table[format], 
				address , ((char*) value)); 
				;
			break;
		}
	}
}
