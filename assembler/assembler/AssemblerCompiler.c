#include <stdlib.h>
#include <string.h>

#include "AssemblerCompiler.h"
#include "assemblerGlobals.h"
#include "AssemblerErrorHandler.h"
#include "AssemblerFileHandler.h"
#include "AssemblerProcessor.h"
#include "AssemblerMemory.h"


int compile(char* fileName)
{
	FILE* file;

	init_globals();
	init_errors();
	init_memory();

	generate_file_name(fileName, SOURCE_SUFFIX);

	// try to open curr file
	file = open_file();

	// if error accoured while opening the file
	if (file == NULL)
	{
		return -1;
	}
	// if the file is open
	else
	{
		process_first_pass(file);
		process_second_pass(file);

		// try to close the file
		close_file(file);
	}

	return 1;
}