#include "assemblerHelpFunctions.h"
#include <ctype.h>

int is_empty_char(char ch)
{
	return (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\x0b');
}

int is_not_letter(char ch)
{
	return (ch < 'a' && ch > 'z' && ch < 'A' && ch > 'Z');
}

int count_string_len(char* string)
{
	int i = 0;

	return 0;
}

int contain_spaces(char* string, int max)
{
	int i;

	for (i=0;  i< max; i++)
	{
		if (isspace(string[i]))
		{
			return 1;
		}
	}

	return 0;
}