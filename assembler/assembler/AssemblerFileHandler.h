// this header defines  the assembler file handler.
// it is responsible for every i/o operations

// writen by:     Ma'ayan cohen
// last modified: 7.3.2014

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

// this function  write a line to specific file.
// input : 1. int - format - what is the format of the lines of the file
//         2. FILE* - currFile a pointer to the file
//         3. int address - a address to print. 
//         4. void* value - a pointer to the value. it is readed according to the format
//         5. char - commandType - value of the command tyoe
//         6. int commandCtr - the command counter - header only
//         6. int SignCtr - the sign counter - header only
void write_to_file(int format, FILE* currFile, int address, void* value, 
				   char commandType, int commandCtr, int signCtr);
#endif