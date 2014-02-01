#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#define ERROR_NUM 40

void log_error(char*, int,  char*);
void init_errors();

char* Assembler_Error_Code_Table[ERROR_NUM];

#endif