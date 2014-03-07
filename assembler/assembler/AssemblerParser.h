// this header defines  the assembler parser.
// it defines global parsing functions for the assembler program

// writen by:     Ma'ayan cohen
// last modified: 7.3.2014

#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

// this function returns char* to the word end
// input:   1. end - char* pointer to line
// returns: char* the location of the word end
char* find_word_end(char* end);

// this function returns char* to next word start
// input:   1. start- char* pointer to line
// returns: char* the location of next word start
char* find_word_start(char* start);

// this function returns int - lenght from curr point until word end
// input:   1. start - char* pointer to line
//          2. addLen - int moving the pointer befoure going to word end
// returns: number of moves untill word end
int mov_to_word_end(const char* start, const int addLen);

// this function returns int - lenght from curr point until word start
// input:   1. start - char* pointer to line
// returns: number of moves untill word start
int mov_to_word_start(const char* start);

// this function returns return string word from two char pointers
// input:   1. wordStart - char* pointer to line pose1
//          2. wordEnd - char* pointer to line pose2
// returns: string between the two poses
char* create_word_by_pointers(const char* wordStart, const char* wordEnd);

// this function checks if the char* exists in the entered dicrionay
// input:   1. word - char* string to check
//          2. arr - char** dictinary of saved words
//          3. max - int num of values in dictionary
// returns: int - -1 if not found - positive number otherwise
int is_saved_word(const char* word, const char** arr, const int max);

// this function checks if the char* exists in the entered signs dicrionay
// input:   1. string - char* string to check
//          2. charList - char* dictinary of saved chars
//          3. max - int num of values in dictionary
// returns: int - -1 if not found - positive number otherwise
int is_contain_saved_sign(const char* string, const char* charList, const int max);

// this function change number from ten base to six base
// input:   1. int value - the number to change base
// returns: int - number in six base
int base6(const int);

// change to base 2 value for negative nubmers
// input:   1. int value -the negative nubmer
// returns: int - number in positive value
int complete_to_2(int value);


#endif
