#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

char* find_word_end(char*);
char* find_word_start(char*);
char* create_word_by_pointers(char*, char*);
int is_saved_word(char*, char**, int);
int is_contain_saved_sign(char* string, char* charList, int max);
char* make_6_base(int);

char* get_sign_position(char*, char*, char);

#endif
