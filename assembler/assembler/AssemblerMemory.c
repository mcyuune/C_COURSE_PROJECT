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

void link_command_data(int val, int pose);
void insert_new_sign(int address, int type);
void insert_reference_address(char* reference);
void link_command_data(unsigned int val, int pose);
void duplicate_command(int counter);
void link_operand_data(int operandNum, int* numOfWords);
void save_instruction_data();
void save_command_data();

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
}

void insert_reference_address(char* reference)
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
	currReference->refereneAddress = IC;
}

void link_command_data(unsigned int val, int pose)
{
	val = val << pose;

	MemorySegment[IC].Value = MemorySegment[IC].Value | val;
}

void duplicate_command(int counter)
{
	while (counter > 0)
	{
		MemorySegment[IC].Value = MemorySegment[IC - counter].Value;
		IC++;
		counter--;
	}
}

void link_operand_data(int operandNum, int* numOfWords)
{
	(*numOfWords)++;
	IC++;

	// if its direct input
	if (currLine.lineCommand.operands[operandNum].Type == IMMEDIATE_INPUT)
	{
		MemorySegment[IC].Value = (int) *currLine.lineCommand.operands[operandNum].Value;
	}
	// if it has lable reference
	else
	{
		insert_reference_address(currLine.lineCommand.operands[operandNum].Value);		

		// if there is offset
		if (currLine.lineCommand.operands[operandNum].offset)
		{
			(*numOfWords)++;
			IC++;

			MemorySegment[IC].Value = currLine.lineCommand.operands[operandNum].offset;		
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

	//create the data
	link_command_data(currLine.lineCommand.type, TYPE_POSE);
	link_command_data(currLine.lineCommand.opcode, OPCODE_POSE);
	link_command_data(currLine.lineCommand.operands[SOURCE_OPERAND].Type, SRC_INPUT_POSE);
	link_command_data(currLine.lineCommand.operands[DEST_OPERAND].Type, DEST_INPUT_POSE);
	link_command_data(currLine.lineCommand.comb_min, COMB_POSE_MIN);
	link_command_data(currLine.lineCommand.comb_max, COMB_POSE_MAX);

	MemoryType[IC] = A;

	if (currLine.lineCommand.operands[SOURCE_OPERAND].Type == REGISTER_INPUT)
	{
		link_command_data(currLine.lineCommand.operands[SOURCE_OPERAND].offset, SRC_REGISTER_POSE);
	}
	else
	{
		link_operand_data(operandNum, &numOfWords);
	}

	operandNum = DEST_OPERAND;

	if (currLine.lineCommand.operands[DEST_OPERAND].Type == REGISTER_INPUT)
	{
		link_command_data(currLine.lineCommand.operands[DEST_OPERAND].offset, DEST_REGISTER_POSE);
	}
	else
	{
		link_operand_data(operandNum, &numOfWords);
	}

	// add it to memory according to time value
	while (IC - i < currLine.lineCommand.time)
	{
		// copy command in memory
		duplicate_command(numOfWords);
		IC++;
	}
}

// this function saves curr tmp line data to the actually memory
void save_line_to_memory()
{	
	int lableAddress;

	// its command line - save it
	if (currLine.lineCommand.opcode != EMPTY_COMMAND)
	{
		lableAddress = IC;
		save_command_data();
	}
	// else it was instruction line
	else if (currLine.lineInstruction.instruction != EMPTY_COMMAND)
	{
		lableAddress = DC;

		// handle the instruction by its type
		save_instruction_data();
	}

	// if there was lable
	if (currLine.lineLable)
	{
		// save it to address table
		insert_new_sign(lableAddress, A, currLine.lineLable);
	}
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

void create_program_file()
{
	int i;

	FILE* output = open_file(WRITE, generate_file_name(TARGET_SUFFIX));

	write_to_file(PROGRAM_HEADER_OUTPUT_FORMAT, output, base6(IC), base6(DC), 0);
	
	for (i=0; i < IC; i++)
	{
		write_to_file(PROGRAM_BODY_OUTPUT_FORMAT, output, base6(i + MEMORY_START_ADDRESS), base6(MemorySegment[i].Value), base6(MemoryType[i]));
	}

	for (; i < IC + DC; i++)
	{
		write_to_file(PROGRAM_BODY_OUTPUT_FORMAT, output, base6(i + MEMORY_START_ADDRESS), base6(DataSegment[i].Value), 0);
	}

	close_files(output);
}

void create_params_files()
{
}
