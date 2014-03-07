#ifndef ASSEMBLER_MEMORY_STRUCTS_H
#define ASSEMBLER_MEMORY_STRUCTS_H

#define MEMORY_SIZE 2000
#define DATA_SIZE   100
#define EMPTY_COMMAND 99
#define DEST_OPERAND 0
#define SOURCE_OPERAND   1
#define MEMORY_START_ADDRESS 100

// a dictionary used for lable types
enum COMMAND_TYPES
{
	A = 'A',
	R = 'R',
	E = 'E'
};

enum COMMAND_PARTS
{
	COMB_POSE_MIN     = 0,
	COMB_POSE_MAX     = 1,
	DEST_REGISTER_POSE= 2,
	DEST_INPUT_POSE	  = 5,
	SRC_REGISTER_POSE = 7,
	SRC_INPUT_POSE	  = 10,
	OPCODE_POSE		  = 12,
	TYPE_POSE		  = 16
};

// define struct for word : word part 10 bits
struct wordCell
{
	unsigned long Value : 20;
};

// create type word - have 2 wordParts - 20 bits
typedef struct wordCell word;

// address table for the lables
typedef struct SignValue* SignTable;

struct SignValue
{
	char*  lable;
	int    address;
	int    type;	
	struct SignValue* next;
};

typedef struct referencedSign* referenceTable;

struct referencedSign
{
	char*  lable;
	int    refereneAddress;
	int	   times;
	int    dupVal;
	int    offset;
	struct referenedSign* next;
};

struct operand
{
	unsigned int Type;
	char*		 Value;
	unsigned int NumValue;
	int          offset;
};

struct commandParts
{
	unsigned int opcode;
	unsigned int type;
	unsigned int comb_min;
	unsigned int comb_max;
	unsigned int opernad_num;

	unsigned int time;

	struct operand operands[2];
};

struct instructionParts
{
	int   instruction;
	char* stringValue;
	int*  dataValue;
	int   value_len;
	char* lable;
};

struct instructionParts typedef instruction;

struct commandParts typedef command;

// define struct for all line possiblle parts
struct lineData
{
	char*		lineLable;
	command		lineCommand;
	instruction lineInstruction;
};

struct lineData currLine;
int IC;
int DC;

word MemorySegment[MEMORY_SIZE];
word DataSegment[DATA_SIZE];
int MemoryType[MEMORY_SIZE];

#endif