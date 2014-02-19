#ifndef ASSEMBLER_MEMORY_H
#define ASSEMBLER_MEMORY_H

// a dictionary used for line possiblle data types
enum DATA_TYPES
{
	LABLE,
	COMMAND,
	INSTRUCTION,
	INSTRUCTION_DATA,
	INSTRUCTION_INT,
	INSTRUCTION_LABLE,
	TYPE, 
	TYPE_VAL_1,
	TYPE_VAL_2,
	TIME,
	OFFSET,
	OFFSET_DATA,
	OPERAND_1_TYPE,
	OPERAND_1_DATA,
	OPERAND_2_TYPE,
	OPERAND_2_DATA
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

#endif