#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "AssemblerProcessor.h"
#include "assemblerGlobals.h"
#include "AssemblerErrorHandler.h"
#include "assemblerValidator.h"
#include "AssemblerParser.h"
#include "AssemblerMemory.h"

int process_command_line(char*, char**);
int process_instruction(char*, char**);
int process_lable(char*, char**, char);
int commit_lable();
void process_line(char* line);


void process_first_pass(FILE* file)
{
	char* line  = (char*) malloc(sizeof (char) * MAXLINE);

	LINE_NUMBER = 0;

	// read line after line from the file
	while (fgets(line, MAXLINE, file))
	{
		LINE_NUMBER++;
		process_line(line);
	}
}

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

void process_line(char* line)
{
	int lableResult = 1;
	int operationResult = 0;

	char* wordStart  = (char*) malloc(sizeof (char) * MAXLINE);
	char* wordEnd    = (char*) malloc(sizeof (char) * MAXLINE);

	char* wordNextStart  = (char*) malloc(sizeof (char) * MAXLINE);

	init_line_data();

	wordStart = find_word_start(line);

	// if its not a comment line
	// and not empty line
	if (*line != Assembler_Signs_Code_Table[COMMENT_SIGN] &&
		*wordStart != STRING_TERMINATOR
		)
	{
		wordEnd = find_word_end(wordStart);

		// if it lable
		if (*wordEnd == Assembler_Signs_Code_Table[LABLE_SIGN])
		{
			lableResult = process_lable(wordStart, &wordEnd, line[0]);

			wordStart = find_word_start(wordEnd + 1);
			wordEnd =   find_word_end(wordStart);
		}

		// if its instruction
		if (*wordStart == Assembler_Signs_Code_Table[INSTRUCTION_SIGN])
		{
			operationResult = process_instruction(wordStart, &wordEnd);
		}
		// else check command if not end of line
		else if (*wordStart != Assembler_Signs_Code_Table[STRING_TERMINATOR])
		{
			operationResult = process_command_line(wordStart,  &wordEnd);
		}

		// if the operation succeded make
		if (operationResult == 1 && lableResult == 1)
		{
			// if the word isnt pointing on string end
			if (*wordEnd !=  STRING_TERMINATOR)
			{
				// check to see if there are extra lines
				wordNextStart = find_word_start(wordEnd + 1);

				// report extra word
				if (*wordNextStart !=  STRING_TERMINATOR)
				{
					log_error(ASSEMBLER_PROCESSOR ,UNRECOGNIZE_SIGN_ERR,create_word_by_pointers(wordStart, wordEnd));
				}
				// else line is great - save it
				else
				{
					save_line();
				}
			}
		}		
	}
}

int process_lable(char* lableStart, char** lableEnd, char first)
{
	return is_proper_lable(lableStart, lableEnd, first);													
}

int process_instruction(char* instruction, char** instructionEnd)
{
	// if the instruction is valid
	if (is_proper_instruction(instruction,instructionEnd) == 1)
	{
		// save it to memory
	}
}

int process_command_line(char* command, char** commandEnd)
{
	is_proper_command(command, commandEnd);
}