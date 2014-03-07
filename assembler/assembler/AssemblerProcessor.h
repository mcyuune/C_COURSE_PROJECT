// this header defines  the assembler processor.
// it handles the processing of the file. first and second part. and outputing the compiles file result

// writen by:     Ma'ayan cohen
// last modified: 7.3.2014

#ifndef ASSEMBLER_PROCESSOR_H
#define ASSEMBLER_PROCESSOR_H

#include <stdio.h>

// process the first pass of the file
// input 1. file - FILE pointer to curr file
void process_first_pass(FILE* file);

// process the first pass of the file
// input 1. file - FILE pointer to curr file
void process_second_pass(FILE* file);

// final function for processing - create the compiled file output
void create_compiled_output();

#endif