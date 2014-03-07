#include <stdlib.h>

#include "assemblerGlobals.h"
#include "AssemblerDictionaries.h"
#include "AssemblerMemory.h"
#include "AssemblerMemoryStructs.h"
#include "AssemblerFileHandler.h"
#include "AssemblerParser.h"

SignTable firstSign;
SignTable currSign;

referenceTable firstReference;
referenceTable currReference;
referenceTable prevReference;

void link_command_data(int val, int pose);
void insert_new_sign(int address, int type);
void insert_reference_address(char* reference, int offset, int numToAdd);
void link_command_data(unsigned int val, int pose);
void duplicate_command(int counter);
void link_operand_data(int operandNum, int* numOfWords);
void save_instruction_data();
void save_command_data();
void init_values();

void insert_new_sign(int address, int type, char* sign)
{
	if (currSign)
	{
		currSign->next = (SignTable*) malloc(sizeof(SignTable));
		currSign = currSign->next;
	}
	else
	{
		firstSign = malloc(sizeof(SignTable));
		currSign = firstSign;
	}

	currSign->address = address;
	currSign->lable = malloc(sizeof(char)* MAXLABLE);
	strcpy(currSign->lable , sign);
	currSign->type = type;
	currSign->next = NULL;
}

void insert_reference_address(char* reference, int offset, int numToAdd)
{
	if (currReference)
	{
		currReference->next = malloc(sizeof(referenceTable));
		currReference = currReference->next;
	}
	else
	{
		firstReference =  malloc(sizeof(referenceTable));
		currReference = firstReference;
	}

	currReference->lable = malloc(sizeof(char) * MAXLABLE); 
	strcpy(currReference->lable , reference);
	currReference->refereneAddress = IC + numToAdd;
	currReference->offset = offset;
	currReference->next = NULL;
}

void link_command_data(unsigned int val, int pose)
{
	val = val << pose;

	MemorySegment[IC].Value = MemorySegment[IC].Value | val;
}

void duplicate_command(int counter)
{
	int adder = 0;

	while (counter > 0)
	{
		MemorySegment[IC + adder].Value = MemorySegment[IC - counter].Value;
		MemoryType[IC + adder] = MemoryType[IC - counter];		
		counter--;
		adder++;
	}
}

void link_operand_data(int operandNum, int* numOfWords)
{
	int i =0;

	(*numOfWords)++;

	// if its direct input
	if (currLine.lineCommand.operands[operandNum].Type == IMMEDIATE_INPUT)
	{
		MemorySegment[IC + *numOfWords - 1].Value  = currLine.lineCommand.operands[operandNum].NumValue;

		MemoryType[IC + *numOfWords - 1] = A;
	}
	// if it has lable reference
	else
	{
		insert_reference_address(currLine.lineCommand.operands[operandNum].Value, 
								 currLine.lineCommand.operands[operandNum].offset,
								 *numOfWords - 1);

		// if there is offset
		if (currLine.lineCommand.operands[operandNum].offset > 0)
		{
			(*numOfWords)++;

			MemorySegment[IC + *numOfWords - 1].Value = currLine.lineCommand.operands[operandNum].offset;			
		}
	}
}

void save_instruction_data()
{
	int i;
	switch(currLine.lineInstruction.instruction)
	{
		case DATA:
			for (i=0; i <= currLine.lineInstruction.value_len; i++)
			{
				DataSegment[DC].Value = currLine.lineInstruction.dataValue[i];
				DC++;
			}
			break;

		case STRING:
			for (i=0; i <= currLine.lineInstruction.value_len; i++)
			{
				DataSegment[DC].Value = currLine.lineInstruction.stringValue[i];
				DC++;
			}
			break;
		case ENTRY:
			insert_new_sign(DC, R ,currLine.lineInstruction.lable);
			break;

		case EXTERN:
			insert_new_sign(DC, E ,currLine.lineInstruction.lable);
			break;
	}
}

void save_command_data()
{
	int i = IC;
	int numOfWords = 1;
	int operandNum = SOURCE_OPERAND;
	int operand_type = 0;

	//create the data
	link_command_data(currLine.lineCommand.type, TYPE_POSE);
	link_command_data(currLine.lineCommand.opcode, OPCODE_POSE);
	link_command_data(currLine.lineCommand.operands[SOURCE_OPERAND].Type, SRC_INPUT_POSE);
	link_command_data(currLine.lineCommand.operands[DEST_OPERAND].Type, DEST_INPUT_POSE);
	link_command_data(currLine.lineCommand.comb_min, COMB_POSE_MIN);
	link_command_data(currLine.lineCommand.comb_max, COMB_POSE_MAX);

	MemoryType[IC] = A;

	if (currLine.lineCommand.opernad_num)
	{
		if (currLine.lineCommand.operands[DEST_OPERAND].Type == REGISTER_INPUT)
		{
			link_command_data(currLine.lineCommand.operands[DEST_OPERAND].offset, DEST_REGISTER_POSE);
		}
		else
		{
			link_operand_data(DEST_OPERAND, &numOfWords);

			if (currLine.lineCommand.operands[DEST_OPERAND].Type == DIRECT_INPUT || 
				currLine.lineCommand.operands[DEST_OPERAND].Type == INDEX_INPUT )
			{
				prevReference = currReference;	
			}
		}


		if (currLine.lineCommand.opernad_num == 2)
		{   
			if (currLine.lineCommand.operands[SOURCE_OPERAND].Type == REGISTER_INPUT)
			{
				link_command_data(currLine.lineCommand.operands[SOURCE_OPERAND].offset, SRC_REGISTER_POSE);
			}
			else
			{
				link_operand_data(SOURCE_OPERAND, &numOfWords);

				if (currLine.lineCommand.operands[SOURCE_OPERAND].Type == DIRECT_INPUT || 
				currLine.lineCommand.operands[SOURCE_OPERAND].Type == INDEX_INPUT )
				{
					currReference->times = currLine.lineCommand.time;
					currReference->dupVal = numOfWords;
				}
			}
		}
	}
	
	if (prevReference)
	{
		prevReference->times = currLine.lineCommand.time;
		prevReference->dupVal = numOfWords;

		prevReference = NULL;
	}

	IC += numOfWords - 1;

	// add it to memory according to time value
	for(i = 1; i < currLine.lineCommand.time; i++)
	{
		IC++;

		// copy command in memory
		duplicate_command(numOfWords);

		IC += numOfWords - 1;
	}

	IC++;
}

// this function saves curr tmp line data to the actually memory
void save_line_to_memory()
{	
	int lableAddress;
	int type;

	// its command line - save it
	if (currLine.lineCommand.opcode != EMPTY_COMMAND)
	{
		lableAddress = IC;
		save_command_data();
		type = A;
	}
	// else it was instruction line
	else if (currLine.lineInstruction.instruction != EMPTY_COMMAND)
	{
		lableAddress = DC;

		// handle the instruction by its type
		save_instruction_data();

		type = R;
	}

	// if there was lable
	if (currLine.lineLable)
	{
		// save it to address table
		insert_new_sign(lableAddress, type, currLine.lineLable);
		currLine.lineLable = NULL;
	}

	init_values();
}

// this function checks if the char* exists in the entered signs dicrionay
// input:   1. value_type - enum for the line type value to save
//          2. value - void* pointer to the value
void update_temp_line_value(const int value_type, const void* value)
{
	switch (value_type)
	{
		case LABLE:
			currLine.lineLable = (char*) malloc(sizeof(char) * MAXLABLE);
			strcpy(currLine.lineLable,  (char*)value);
			break;
		case STRING_VALUE:
			currLine.lineInstruction.stringValue = (char*) malloc(sizeof(char) * MAXLABLE);
			strcpy(currLine.lineInstruction.stringValue,(char*) value);
			break;
		case DATA_VALUE:
			currLine.lineInstruction.dataValue = (int*) malloc(sizeof(int) * MAXLINE);
			currLine.lineInstruction.dataValue = (int*) value;
			break;
		case SRC_VALUE:
			currLine.lineCommand.operands[SOURCE_OPERAND].Value = (char*) malloc(sizeof(char) * MAXLABLE);
			strcpy(currLine.lineCommand.operands[SOURCE_OPERAND].Value, (char*) value);
			break;
		case DEST_VALUE:
			currLine.lineCommand.operands[DEST_OPERAND].Value = (char*) malloc(sizeof(char) * MAXLABLE);
			strcpy(currLine.lineCommand.operands[DEST_OPERAND].Value, (char*) value);
			break;
		case INSTRUCTION_LABLE:
			currLine.lineInstruction.lable = (char*) malloc(sizeof(char) * MAXLABLE);
			strcpy(currLine.lineInstruction.lable, (char*) value);
			break;
		case TYPE:
			currLine.lineCommand.type = *((int*) value);
			break;
		case SOURCE_IMMEDIATE_VALUE:
			currLine.lineCommand.operands[SOURCE_OPERAND].NumValue = *((int*) value);
			break;
		case DEST_IMMEDIATE_VALUE:
			currLine.lineCommand.operands[DEST_OPERAND].NumValue = *((int*) value);
			break;
		case TIME:
			currLine.lineCommand.time = *((int*) value);
			break;
		case COMB_MIN:
			currLine.lineCommand.comb_min = *((int*) value);
			break;
		case COMB_MAX:
			currLine.lineCommand.comb_max = *((int*) value);
			break;
		case OPCODE:
			currLine.lineInstruction.instruction = EMPTY_COMMAND;
			currLine.lineCommand.opcode = *((unsigned*) value);
			break;
		case OP_NUM:
			currLine.lineCommand.opernad_num = *((int*) value);
			break;
		case INSTRUCTION:
			currLine.lineCommand.opcode = EMPTY_COMMAND;
			currLine.lineInstruction.instruction = *((int*) value);
			break;
		case VALUE_LEN:
			currLine.lineInstruction.value_len = *((int*) value);
		case SRC_OFFSET:
			currLine.lineCommand.operands[SOURCE_OPERAND].offset = *((int*) value);
			break;
		case DEST_OFFSET:
			currLine.lineCommand.operands[DEST_OPERAND].offset = *((int*) value);
			break;
		case SRC_TYPE:
			currLine.lineCommand.operands[SOURCE_OPERAND].Type = *((int*) value);
			break;
		case DEST_TYPE:
			currLine.lineCommand.operands[DEST_OPERAND].Type = *((int*) value);
			break;
	}
}


void init_values()
{
	currLine.lineCommand.opcode   = EMPTY_COMMAND;
	currLine.lineCommand.comb_max = 0;
	currLine.lineCommand.comb_min = 0;
	currLine.lineCommand.operands[SOURCE_OPERAND].offset = 0;
	currLine.lineCommand.operands[SOURCE_OPERAND].Type   = 0;
	currLine.lineCommand.operands[SOURCE_OPERAND].Value  = 0;
	currLine.lineCommand.operands[DEST_OPERAND].offset = 0;
	currLine.lineCommand.operands[DEST_OPERAND].Type   = 0;
	currLine.lineCommand.operands[DEST_OPERAND].Value  = 0;
	currLine.lineCommand.time = 0;
	currLine.lineCommand.type = 0;
	currLine.lineCommand.opernad_num = 0;

	currLine.lineInstruction.instruction = EMPTY_COMMAND;
	currLine.lineLable = NULL;
	currLine.lineInstruction.stringValue = NULL;
	currLine.lineInstruction.lable = NULL;
	currLine.lineInstruction.dataValue = NULL;
	currLine.lineInstruction.value_len = 0;	
}

void create_program_file()
{
	int i;
	FILE* output = open_file(WRITE, generate_file_name(TARGET_SUFFIX));

	write_to_file(PROGRAM_HEADER_OUTPUT_FORMAT, output, 0, NULL, 0, base6(IC), base6(DC));
	
	for (i=0; i < IC; i++)
	{
		int number_in_six =  base6(MemorySegment[i].Value);
		write_to_file(PROGRAM_BODY_OUTPUT_FORMAT, output, base6(i + MEMORY_START_ADDRESS), &number_in_six, MemoryType[i], 0, 0);
	}

	for (; i < IC + DC; i++)
	{
		int number_in_six =  base6(DataSegment[i - IC].Value);
		write_to_file(PROGRAM_SIGN_OUTPUT_FORMAT, output, base6(i + MEMORY_START_ADDRESS), &number_in_six , 0, 0, 0);
	}

	close_files(output);
}

void create_params_files()
{
}

void complete_sign_references()
{
	int i;
	int index;
	int is_set = 0;

	currReference = firstReference;

	while(currReference)
	{
		currSign = firstSign;
		while (currSign && !is_set)
		{
			// if the address match
			if (strcmp(currSign->lable, currReference->lable) == 0)
			{
				// update the address for all duplicate memory for the current place
				for (i = 0; i < currReference->times; i++)
				{
					index = currReference->refereneAddress + (i * currReference->dupVal);

					// if its external
					if (currSign->type != E)
					{
						MemorySegment[index].Value = currSign->address + MEMORY_START_ADDRESS;

						if (currSign->type == R)
						{
							MemorySegment[index].Value += IC;
						}						

						MemoryType[index] = currSign->type;

						if (currReference->offset > 0)
						{
							if (currSign->type == R)
							{
								MemorySegment[index + 1].Value = DataSegment[currSign->address + currReference->offset].Value;

							}
							else if (currSign->type == A)
							{
								MemorySegment[index + 1].Value = MemorySegment[currSign->address + currReference->offset].Value;
							}

							MemoryType[index + 1] = A;
						}
					}
					else
					{
						if (currReference->offset > 0)
						{
							MemoryType[index + 1] = A;
						}

						MemoryType[index] = E;
					}
				}

				is_set = 1;
			}

			currSign = currSign->next;
		}

		currReference = currReference->next;
		is_set = 0;
	}
}