#include "AssemblerDictionaries.h"
#include "AssemblerCompiler.h"
#include "assemblerGlobals.h"
#include "AssemblerErrorHandler.h"

#include <stdio.h>

#define MAX_FILE_NAME 100

int main( int argc, char* argv[] )
{	
	int i;	

	argc = 2;
	//argv[1] = "testLable";
	//argv[1] = "firstPassMainLoppTest";
	//argv[0] = "testInstruction";
	//argv[0] = "commandTest";
	argv[0] = "testFull";

	// init all gloabals and directories 
	init_AssemblerGlobals();
	init_AssemblerErrorHandler();

	// go over all send files
 	for (i=0; i < argc -1 ; i++ )
	{
		compile(argv[i]);
	}

	getchar();
	
	return 0;
}
