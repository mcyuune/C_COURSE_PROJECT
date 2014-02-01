#include <ctype.h>
#include <stdlib.h>

#include "AssemblerParser.h"
#include "assemblerGlobals.h"

char* find_word_end(char* end)
{
	if (*end != Assembler_Signs_Code_Table[STRING_TERMINATOR])
	{
		while (end[1] != Assembler_Signs_Code_Table[STRING_TERMINATOR] && !isspace(end[1]))
		{
			end++;
		}
	}

	return end;
}

char* find_word_start(char* start)
{
	while (isspace(*start) && *start !=  STRING_TERMINATOR)
	{
		start++;
	}

	return start;
}

int is_contain_saved_sign(char* string, char* charList, int max)
{
	int i;

	for (i = 0; i <  max; i++)
	{
		// if there is saved sign in the string
		if (strchr(string, charList[i]) != NULL)
		{
			i = -1;
			break;
		}
	}

	return i;
}

char* create_word_by_pointers(char* wordStart, char* wordEnd)
{
	char* word = (char*) malloc (sizeof(char) * (wordEnd - wordStart) + 2);
	strncpy(word, wordStart ,wordEnd - wordStart + 1);

	word[wordEnd - wordStart + 1] = STRING_TERMINATOR;

	return word;
}

int is_saved_word(char* word, char** arr, int max)
{
	int i;

	for (i=0; i< max; i++)
	{
		if (strcmp(arr[i], word) == 0)
		{
			return i;
		}
	}

	return -1;
}

char* get_sign_position(char* start, char* end, char sign)
{
	while (start < end && *start != sign)
	{
		start++;
	}

	return (*start == sign)? start : NULL ;
}