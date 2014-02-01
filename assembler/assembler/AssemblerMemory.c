#include <stdlib.h>

#include "assemblerGlobals.h"
#include "AssemblerMemory.h"

struct 
{
	unsigned int Part : 10;
} wordPart;

typedef struct wordPart word[2];

struct addressTable
{
	char* lable;
	word* address;
};

struct lineData
{
	char* lable;
	int command;
	int instruction;
	char* instructionData;
	int*   instructionInt;
	int type;
	int type_val_1;
	int type_val_2;
	int time;
	int offset;
	char* offsetLable;
	int operand1Type;
	char* operand1;
	int operand2Type;
	char* operand2;
};

struct lineData currLine;
int IC;
int DC;

word* MemorySegment;
word* DataSegment;

void init_line_data()
{
	currLine.lable = (char*) malloc(sizeof(char) * MAXLABLE);
	currLine.lable[0] = Assembler_Signs_Code_Table[STRING_TERMINATOR];

	currLine.offsetLable = (char*) malloc(sizeof(char) * MAXLABLE);
	currLine.offsetLable[0] = Assembler_Signs_Code_Table[STRING_TERMINATOR];

	currLine.instructionData = (char*) malloc(sizeof(char) * MAXLINE);
	currLine.instructionData[0] = Assembler_Signs_Code_Table[STRING_TERMINATOR];

	currLine.operand1 = (char*) malloc(sizeof(char) * MAXLINE);
	currLine.operand1[0] = Assembler_Signs_Code_Table[STRING_TERMINATOR];

	currLine.operand2 = (char*) malloc(sizeof(char) * MAXLINE);
	currLine.operand2[0] = Assembler_Signs_Code_Table[STRING_TERMINATOR];

	currLine.instructionInt = (int*) malloc(sizeof(char) * MAXLINE);
	currLine.instructionInt = 0;

	currLine.command = -1;
	currLine.type = -1;
	currLine.type_val_1 = -1;
	currLine.type_val_2 = -1;
	currLine.time = -1;
	currLine.offset = -1;
	

	currLine.instruction = -1;
	
	currLine.operand1Type = -1;

	currLine.operand2Type = -1;
}

void save_line()
{
	int i;

	if (currLine.lable[0] != Assembler_Signs_Code_Table[STRING_TERMINATOR])
	{
		// save it to address table
	}

	// its command line - save it
	if (currLine.command != -1)
	{
		//create the data

		// add it to memory according to time value
		for (i = 0; i < currLine.time; i++)
		{

		}
	}
	// else it was instruction line
	else if (currLine.instruction != -1)
	{
		// handlw the instruction by its type
	}
}

void temp_save_value(int value_type, int int_value, char* string_value, int* int_array_value)
{
	switch (value_type)
	{
		case LABLE:
			currLine.lable = string_value;
			break;
		case OFFSET_DATA:
			currLine.offsetLable = string_value;
			break;
		case INSTRUCTION_DATA:
			currLine.instructionData = string_value;
			break;
		case INSTRUCTION_INT:
			currLine.instructionInt= int_array_value;
			break;
		case OPERAND_1_DATA:
			currLine.operand1 = string_value;
			break;
		case OPERAND_2_DATA:
			currLine.operand1 = string_value;
			break;

		case TYPE:
			currLine.type = int_value;
			break;
		case TIME:
			currLine.time = int_value;
			break;
		case TYPE_VAL_1:
			currLine.type_val_1 = int_value;
			break;
		case TYPE_VAL_2:
			currLine.type_val_2 = int_value;
			break;
		case COMMAND:
			currLine.command = int_value;
			break;
		case INSTRUCTION:
			currLine.instruction = int_value;
			break;
		case OFFSET:
			currLine.offset = int_value;
			break;
		case OPERAND_1_TYPE:
			currLine.operand1Type = int_value;
			break;
		case OPERAND_2_TYPE:
			currLine.operand2Type = int_value;
			break;
	}
}

void init_memory()
{
	DataSegment = malloc (sizeof (word) * 80);
	MemorySegment = malloc (sizeof (word) * 2000);
}