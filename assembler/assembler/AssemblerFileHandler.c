#include <stdio.h>
#include <stdlib.h>

#include "AsssemblerFileHandler.h"
#include "AssemblerErrorHandler.h"
#include "AssemblerGlobals.h"
#include "AssemblerDictionaries.h"

char* CURR_FILE_NAME;

// this function open curr file name
// return: FILE* - returns open file pointer
FILE* open_file()
{
	FILE* ft = fopen(CURR_FILE_NAME, "r");

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
char* generate_file_name(const char* fileName, const int suffix_type)
{
	CURR_FILE_NAME = (char*) malloc( strlen(fileName) + 
									 strlen(Assembler_file_prefix_Code_Table[suffix_type]) + 1);//+1 for the zero-terminator

	strcpy(CURR_FILE_NAME, fileName);
	strcat(CURR_FILE_NAME, Assembler_file_prefix_Code_Table[suffix_type]);
}