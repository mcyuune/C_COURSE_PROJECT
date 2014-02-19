#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "AssemblerProcessor.h"
#include "assemblerGlobals.h"
#include "AssemblerDictionaries.h"
#include "AssemblerErrorHandler.h"
#include "assemblerValidator.h"
#include "AssemblerParser.h"
#include "AssemblerMemory.h"

void process_line(char* line);

// process the first pass of the file
// input 1. file - FILE pointer to curr file
void process_first_pass(FILE* file)
{
	char* line  = (char*) malloc(sizeof (char) * MAXLINE);
	LINE_NUMBER = 0;

	// read line after line from the file
	while (fgets(line, MAXLINE, file))
	{
		LINE_NUMBER++;

		// process the line
		process_line(line);
	}

	// free memory
	free(line);
}


// process the first pass of the file
// input 1. file - FILE pointer to curr file
void process_second_pass(FILE* file)
{
	/*
	char* newline;

	while (fgets(newline, MAXLINE, file))
	{
		// process the file first pass
		// find : its lable.
		// find . something check data
		// other its command
		// for / it means the type bulshit
		// , seperate operands

	}
	*/
}

// Handle the processing of first pass lines.
// input : 1. line : char pointer to the curr line
void process_line(char* line)
{
	int lableResult = 1;
	int operationResult = 0;
	int lableExists = 0;
	int operationExists = 0;

	char* word;
	char* wordStart  = line;
	char* wordEnd    = line;	

	// init values for the line data in the memory
	init_temp_line_space();

	// find the start of data in the line (not white spaces)
	wordStart += mov_to_word_start(wordStart);

	// if the line is not a comment line
	// and not empty line
	if (*wordStart != STRING_TERMINATOR && *wordStart != Assembler_Signs_Code_Table[COMMENT_SIGN])
	{
		// find curr word end
		wordEnd   += mov_to_word_end(wordStart, wordStart - wordEnd);

		// if it lable (lable sign in end of word)
		if (*wordEnd == Assembler_Signs_Code_Table[LABLE_SIGN])
		{
			// check validity
			lableResult = is_proper_lable(wordStart, wordEnd, line[0], LABLE);

			// move to next word
			wordEnd += 1;
			wordStart += wordEnd - wordStart;

			lableExists = 1;
		}

		// if lable result were ok - or no lable found
		if (lableResult == 1)
		{
			// move forward
			wordStart += mov_to_word_start(wordEnd);
			wordEnd   += mov_to_word_end(wordStart, wordStart - wordEnd);

			// if next word is instruction (dot sign in begining)
			if (*wordStart == Assembler_Signs_Code_Table[INSTRUCTION_SIGN])
			{
				// check instruction
				operationResult = is_proper_instruction(wordStart, &wordEnd);
				operationExists = 1;
			}
			// else if there are more values in line
			else if (*wordStart != STRING_TERMINATOR)
			{
				// check command
				operationResult = is_proper_command(wordStart, &wordEnd);
				operationExists = 1;
			}

			// if there was lable without any more values
			if (operationExists == 0 && lableExists == 1)
			{
				log_error(ASSEMBLER_PROCESSOR, LABLE_ALONE_ERR, "");
			}
			// if the operation and the lable succeded 
			else if (operationResult == 1)
			{
				// if the word end isnt pointing on string end
				if (*wordEnd !=  STRING_TERMINATOR && *wordEnd !=  LINE_TERMINATOR)
				{
					// check to see if there are extra words
					wordStart += (wordEnd - wordStart) + mov_to_word_start(wordEnd + 1);

					// report extra word
					if (*wordStart !=  STRING_TERMINATOR && 
						*wordStart !=  LINE_TERMINATOR)
					{
						word = create_word_by_pointers(wordEnd, wordStart);
						log_error(ASSEMBLER_PROCESSOR ,UNRECOGNIZE_SIGN_ERR,word);

						free(word);
					}
					// else line is great - save it
					else
					{
						save_line_to_memory();
					}
				}
			}
		}		
	}
}