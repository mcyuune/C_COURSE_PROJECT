#include <stdlib.h>

#include "assemblerGlobals.h"
#include "AssemblerDictionaries.h"
#include "AssemblerMemory.h"

// define struct for word : word part 10 bits
struct 
{
	unsigned int Part : 10;
} wordPart;

// create type word - have 2 wordParts - 20 bits
typedef struct wordPart word[2];

//
struct addressTable
{
	char* lable;
	word* address;
};

// define struct for all line possiblle parts
struct lineData
{
	char* lable;

	int   command;

	int   instruction;
	char* instructionData;
	int*  instructionInt;
	char* instructionLable;

	int   type;
	int   type_val_1;
	int   type_val_2;

	int   time;

	int   offset;
	char* offsetLable;

	int   operand1Type;
	char* operand1;

	int   operand2Type;
	char* operand2;
};

struct lineData currLine;
int IC;
int DC;

word* MemorySegment;
word* DataSegment;

// this function intalize temp line memory for new line
void init_temp_line_space()
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

// this function saves curr tmp line data to the actually memory
void save_line_to_memory()
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

// this function checks if the char* exists in the entered signs dicrionay
// input:   1. value_type - enum for the line type value to save
//          2. value - void* pointer to the value
void update_temp_line_value(const int value_type, const void* value)
{
	switch (value_type)
	{
		case LABLE:
			currLine.lable = (char*) value;
			break;
		case OFFSET_DATA:
			currLine.offsetLable = (char*) value;
			break;
		case INSTRUCTION_DATA:
			currLine.instructionData = (char*) value;
			break;
		case INSTRUCTION_LABLE:
			currLine.instructionLable = (char*) value;
			break;
		case INSTRUCTION_INT:
			currLine.instructionInt= (int*) value;
			break;
		case OPERAND_1_DATA:
			currLine.operand1 = (char*) value;
			break;
		case OPERAND_2_DATA:
			currLine.operand1 = (char*) value;
			break;

		case TYPE:
			currLine.type = *((int*) value);
			break;
		case TIME:
			currLine.time = *((int*) value);
			break;
		case TYPE_VAL_1:
			currLine.type_val_1 = *((int*) value);
			break;
		case TYPE_VAL_2:
			currLine.type_val_2 = *((int*) value);
			break;
		case COMMAND:
			currLine.command = *((int*) value);
			break;
		case INSTRUCTION:
			currLine.instruction = *((int*) value);
			break;
		case OFFSET:
			currLine.offset = *((int*) value);
			break;
		case OPERAND_1_TYPE:
			currLine.operand1Type = *((int*) value);
			break;
		case OPERAND_2_TYPE:
			currLine.operand2Type = *((int*) value);
			break;
	}
}

// this function intalize the assembler memory for use
void init_AssemblerMemory()
{
	DataSegment = malloc (sizeof (word) * 80);
	MemorySegment = malloc (sizeof (word) * 2000);
}