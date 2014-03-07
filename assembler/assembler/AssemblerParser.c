#include <ctype.h>
#include <stdlib.h>

#include "AssemblerParser.h"
#include "assemblerGlobals.h"
#include "AssemblerDictionaries.h"

// this function returns char* to the word end
// input:   1. end - char* pointer to line
// returns: char* the location of the word end
char* find_word_end(char* end)
{
	// make sure not null
	if (*end != Assembler_Signs_Code_Table[STRING_TERMINATOR])
	{
		// keewp going until space
		while (end[1] != STRING_TERMINATOR && !isspace(end[1]))
		{
			end++;
		}
	}

	return end;
}

// this function returns char* to next word start
// input:   1. start- char* pointer to line
// returns: char* the location of next word start
char* find_word_start(char* start)
{
	// make sure not null
	if (*start != STRING_TERMINATOR)
	{
		// keep going until value
		while (isspace(start[0]))
		{
			start++;
		}
	}

	return start;
}

// this function returns int - lenght from curr point until word start
// input:   1. start - char* pointer to line
// returns: number of moves untill word start
int mov_to_word_start(const char* start)
{
	int i = 0;

	//make sure not null
	if(*start != STRING_TERMINATOR)
	{
		// keep going until space
		while (isspace(start[i]))
		{
			i++;
		}
	}

	return i;
}

// this function returns int - lenght from curr point until word end
// input:   1. start - char* pointer to line
//          2. addLen - int moving the pointer befoure going to word end
// returns: number of moves untill word end
int mov_to_word_end(const char* start, const int addLen)
{
	int i = 0;

	// move over the word until space or end
	while (!isspace(start[i]) && start[i] !=  STRING_TERMINATOR)
	{
		i++;
	}

	// return number until word end
	return i + addLen - 1;
}

// this function checks if the char* exists in the entered signs dicrionay
// input:   1. string - char* string to check
//          2. charList - char* dictinary of saved chars
//          3. max - int num of values in dictionary
// returns: int - -1 if not found - positive number otherwise
int is_contain_saved_sign(const char* string, const char* charList, const int max)
{
	int i;

	for (i = 0; i <  max; i++)
	{
		// if there is saved sign in the string
		if (strchr(string, charList[i]) != NULL)
		{
			return i;
		}
	}

	return -1;
}

// this function returns return string word from two char pointers
// input:   1. wordStart - char* pointer to line pose1
//          2. wordEnd - char* pointer to line pose2
// returns: string between the two poses
char* create_word_by_pointers(const char* wordStart, const char* wordEnd)
{
	char* word;
	
	// if pointer doesnt make sense return null
	if (wordEnd < wordStart ||
		wordEnd - wordStart > MAXLINE)
	{
		word = NULL;
	}
	else
	{
		// create the word
		word = (char*) malloc (sizeof(char) * (wordEnd - wordStart) + 2);
		strncpy(word, wordStart ,wordEnd - wordStart + 1);

		word[wordEnd - wordStart + 1] = STRING_TERMINATOR;
	}

	return word;
}

// this function checks if the char* exists in the entered dicrionay
// input:   1. word - char* string to check
//          2. arr - char** dictinary of saved words
//          3. max - int num of values in dictionary
// returns: int - -1 if not found - positive number otherwise
int is_saved_word(const char* word, const char** arr, const int max)
{
	int i;

	// go over all dictionary
	for (i=0; i< max; i++)
	{
		// if valu match
		if (strcmp(arr[i], word) == 0)
		{
			// return the index
			return i;
		}
	}

	return -1;
}

// this function change number from ten base to six base
// input:   1. int value - the number to change base
// returns: int - number in six base
int base6(int value)
{
	int i = 1;
	int result = 0;

	// while value is more tthen zero
	while (value > 0)
	{
		// add the base 6 value
		result += (value % 6) * i;
		value = value / 6;
		i=i*10;
	}

	return result;
}

// change to base 2 value for negative nubmers
// input:   1. int value -the negative nubmer
// returns: int - number in positive value
int complete_to_2(int value)
{
	unsigned int one = 1;
	int mask = 0x00FFFFFFFF;
	int i;	

	// make positive
	i = value * -1;

	// switch his bits
	i ^= mask;

	// add one
	i += one;

	return i;
}