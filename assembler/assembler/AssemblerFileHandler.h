#ifndef ASSEMBLER_FILE_HANDLER_H
#define ASSEMBLER_FILE_HANDLER_H

#include <stdio.h>

// this function generate full file name by adding suffix to the file name
// input:  1. char* fileName - the file name,
//         2. int suffixType - the wanted suffix
// return: char* - full file name
char* generate_file_name(const char* fileName, const int suffixType);

// this function open curr file name
// return: FILE* - returns open file pointer
FILE* open_file();

// this function close the file 
// input:  1. FILE* - the file pointer
void close_files(FILE* file);

#endif