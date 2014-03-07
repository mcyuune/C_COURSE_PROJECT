#include <stdlib.h>
#include <string.h>

#include "AssemblerCompiler.h"
#include "AssemblerErrorHandler.h"

#include "assemblerGlobals.h"
#include "AssemblerDictionaries.h"

#include "AssemblerFileHandler.h"
#include "AssemblerProcessor.h"

// the main assembler procedure.
// input : fileName - char* containing the file name to compiple
void compile(char* fileName)
{
	FILE* file;

	// create file name and log its start
	CURR_FILE_NAME = fileName;
	log_file_start();

	// try to open curr file
	file = open_file(READ, generate_file_name(SOURCE_SUFFIX));

	// if file is good
	if (file != NULL)
	{
		// process the file
		process_first_pass(file);
		process_second_pass(file);		

		// try to close the file
		close_files(file);

		// if validation was correct
		if (!check_errors())
		{
			// create compiled assembler output 
			create_compiled_output();
		}
	}

	log_file_end();	
}