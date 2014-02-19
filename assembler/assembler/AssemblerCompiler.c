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
	generate_file_name(fileName, SOURCE_SUFFIX);
	log_file_start();

	// try to open curr file
	file = open_file();

	// if file is good
	if (file != NULL)
	{
		// process the file
		process_first_pass(file);
		process_second_pass(file);

		// try to close the file
		close_files(file);

		// save all result file
	}

	log_file_end();	
}