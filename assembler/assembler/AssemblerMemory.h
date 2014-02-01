#ifndef ASSEMBLER_MEMORY_H
#define ASSEMBLER_MEMORY

enum DATA_TYPES
{
	LABLE,
	COMMAND,
	INSTRUCTION,
	INSTRUCTION_DATA,
	INSTRUCTION_INT,
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

void save_line();
void init_line_data();
void temp_save_value(int value_type, int int_value, char* string_value, int* int_array_value);
void init_memory();

#endif