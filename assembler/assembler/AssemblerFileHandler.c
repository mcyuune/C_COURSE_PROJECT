#include <stdio.h>
#include <stdlib.h>

#include "AsssemblerFileHandler.h"
#include "AssemblerErrorHandler.h"
#include "AssemblerGlobals.h"

char* CURR_FILE_NAME;

FILE* open_file()
{
	FILE* ft = fopen(CURR_FILE_NAME, "r");

	// TO DO WRITE error
	if (ft == NULL)
	{
		log_error(ASSEMBLER_FILE_HANDLER, OPENING_FILE_ERR, NULL, 1);
	}

	return ft;
}

int close_file(FILE* assemblerFile)
{
	if (fclose(assemblerFile) != 0)
	{
		log_error(ASSEMBLER_FILE_HANDLER, CLOSING_FILE_ERR, NULL, 1);
	}
}

char* generate_file_name(char* fileName, int suffix_type)
{
	CURR_FILE_NAME = (char*) malloc( strlen(fileName) + 
									 strlen(Assembler_file_prefix_Code_Table[suffix_type]) + 1);//+1 for the zero-terminator

	strcpy(CURR_FILE_NAME, fileName);
	strcat(CURR_FILE_NAME, Assembler_file_prefix_Code_Table[suffix_type]);
}