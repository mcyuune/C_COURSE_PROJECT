// this header defines  the assembler error handler. 
// it handles all login of errors

// writen by:     Ma'ayan cohen
// last modified: 7.3.2014

#ifndef ASSEMBLER_ERROR_HANDLER_H
#define ASSEMBLER_ERROR_HANDLER_H

#define ERROR_NUM 40

// this function write error to output
// input: 1. source - char* the error source,
//        2. errCode - int - error num
//        3. value - char* - the file error was found
void log_error(const char* source, const int errCode, const char* value);

// this function close file log process (call when ending curr file processing)
void log_file_end();

// this function start file log process (call when starting new file processing)
void log_file_start();

// this function intalize the assemblerErrorHandler and need to be called in the 
// begining of using AssemblerErrorHandler
void init_AssemblerErrorHandler();

// table conatining all assembler errors
char* Assembler_Error_Code_Table[ERROR_NUM];

int check_errors();
#endif