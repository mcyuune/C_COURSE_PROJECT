#ifndef ASSEMBLER_FILE_HANDLER_H
#define ASSEMBLER_FILE_HANDLER_H

#include <stdio.h>

char* generate_file_name(char* fileName, int suffix_type);
int   close_file(FILE* assemblerFile);
FILE* open_file();

#endif