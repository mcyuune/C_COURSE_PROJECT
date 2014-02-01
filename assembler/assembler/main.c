#include "AssemblerCompiler.h"
#include "assemblerGlobals.h"

#include <stdio.h>

#define MAX_FILE_NAME 100

int main( int argc, char* argv[] )
{	
	int i;	
	char c;

	//sscanf("rrrr-9", "%[r]%d",&c, &i);

	argc = 2;
	//argv[1] = "testLable";
	//argv[2] = "firstPassMainLoppTest";
	//argv[3] = "testInstruction";
	argv[1] = "fullCommandTest";

	// go over all send files
 	for (i=1; i < argc ; i ++)
	{
		if( compile(argv[i]) == -1)
		{
			break;
		};
	}

	getchar();
	
	return 0;
}
