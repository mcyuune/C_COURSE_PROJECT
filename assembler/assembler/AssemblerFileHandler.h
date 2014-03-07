#ifndef ASSEMBLER_FILE_HANDLER_H
#define ASSEMBLER_FILE_HANDLER_H

#include <stdio.h>

// this function generate full file name by adding suffix to the file name
// input:  1. char* fileName - the file name,
//         2. int suffixType - the wanted suffix
// return: char* - full file name
char* generate_file_name(const int suffixType);

// this function open curr file name
// return: FILE* - returns open file pointer
FILE* open_file(const char* fileMode, const char* fileName );

// this function close the file 
// input:  1. FILE* - the file pointer
void close_files(FILE* file);

void write_to_file(int format, FILE* currFile, int address, void* value, 
				   char commandType, int commandCtr, int signCtr);
#endif