// this header defines  the assembler memory.
// the memory save all the data of the line,
// and send it to output compiles files

// writen by:     Ma'ayan cohen
// last modified: 7.3.2014

#ifndef ASSEMBLER_MEMORY_H
#define ASSEMBLER_MEMORY_H

// a dictionary used for line possiblle data types
enum LINE_TYPES
{
	LABLE,
	OPCODE,
	INSTRUCTION,
	STRING_VALUE,
	DATA_VALUE,
	VALUE_LEN,
	INSTRUCTION_LABLE,
	TYPE, 
	COMB_MAX,
	COMB_MIN,
	DEST_IMMEDIATE_VALUE,
	SOURCE_IMMEDIATE_VALUE,
	TIME,
	OP_NUM,
	OFFSET,
	SRC_TYPE,
	SRC_VALUE,
	SRC_OFFSET,
	DEST_TYPE,
	DEST_VALUE,
	DEST_OFFSET
};

// this function saves curr tmp line data to the actually memory
void save_line_to_memory();

// this function intalize the memory for new file
void init_memory_for_new_file();

// this function checks if the char* exists in the entered signs dicrionay
// input:   1. value_type - enum for the line type value to save
//          2. value - void* pointer to the value
void update_temp_line_value(const int value_type, const void* value);

// complete all data by setting the value of sign references
void complete_sign_references();

// this functions create the result of the compiletion
// function creating os file
void create_program_file();

// function creating the params output files
void create_params_files();

// function checked if lable is already in use
// input:    1. char* lable - the new lable
// return    int - 1 if free, and 0 if in use
int is_lable_free(char* lable);

#endif