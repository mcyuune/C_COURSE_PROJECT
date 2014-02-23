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
	COMB_MIN,
	COMB_MAX,
	TIME,
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

// this function intalize temp line memory for new line
void init_temp_line_space();

// this function checks if the char* exists in the entered signs dicrionay
// input:   1. value_type - enum for the line type value to save
//          2. value - void* pointer to the value
void update_temp_line_value(const int value_type, const void* value);

// this function intalize the assembler memory for use
void init_AssemblerMemory();

void create_program_file();
void create_params_files();

#endif