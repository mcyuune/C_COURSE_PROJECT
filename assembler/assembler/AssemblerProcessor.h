#ifndef ASSEMBLER_PROCESSOR_H
#define ASSEMBLER_PROCESSOR_H

#include <stdio.h>

// process the first pass of the file
// input 1. file - FILE pointer to curr file
void process_first_pass(FILE* file);

// process the first pass of the file
// input 1. file - FILE pointer to curr file
void process_second_pass(FILE* file);

#endif