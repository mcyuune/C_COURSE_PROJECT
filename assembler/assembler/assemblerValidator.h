#ifndef ASSEMBLER_VALIDATOR_H
#define ASSEMBLER_VALIDATOR_H

// this function checks if curr position in line is proper lable
// input 1. lableStart - char pointer to lable start
//       2. lableEnd - lableEnd - char pointer to lable end
//       3  firstLine - char - first line character - to make sure not empty
// return - int - 0 if false, 1 if ok
int is_proper_lable(const char* lableStart, const char* lableEnd,const char firstLine, const int lableType);

// this function checks if curr position in line is proper instruction
// it move over the line and process all expected instruction parts
// input 1. instructionStart - char pointer to instruction start
//       2. EndInstruction - pointer to char pointer. curr position in line. will move to the instruction end
// return - int - 0 if false, 1 if ok
int is_proper_instruction(const char*, char**);

// this function checks if curr position in line is proper command
// it move over the line and process all expected command parts
// input 1. commandStart - char pointer to instruction start
//       2. commandEnd - pointer to char pointer. curr position in line. will move to the command end
// return - int - 0 if false, 1 if ok
int is_proper_command(const char*, char**);


#endif