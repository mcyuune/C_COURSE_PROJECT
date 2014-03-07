// this header contains all variables, structs and types for the AssemblerMemory 
// AssemblerMemory is the only one who uses this header

// writen by:     Ma'ayan cohen
// last modified: 7.3.2014

#ifndef ASSEMBLER_MEMORY_STRUCTS_H
#define ASSEMBLER_MEMORY_STRUCTS_H

// constant values
#define MEMORY_SIZE          2000
#define DATA_SIZE            100
#define EMPTY_COMMAND        99
#define DEST_OPERAND         0
#define SOURCE_OPERAND       1
#define MEMORY_START_ADDRESS 100

// a dictionary used for lable types
enum COMMAND_TYPES
{
	A  = 'A', // command
	LC = 'C', // command lable
	LD = 'L', // data lable
	R  = 'R', // entry
	EN = 'N', // entry
	E  = 'E' // extarn
};

// dictionary setting the position of every command part in the memory command word result
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

///////////////////////////// word type for memory cells //////////////////
// define struct for word : word part 10 bits
struct wordCell
{
	unsigned long Value : 20;
};

// create type word - have 2 wordParts - 20 bits
typedef struct wordCell word;

///////////////////////////// sign table for saving all signs the file //////////////////
// define the struct for the sign table
struct SignValue
{
	char*  lable;
	int    address;
	int    type;	
	struct SignValue* next;
};

//typedef struct SignValue SignCell;

// connected list containing all sign accoured in the file
typedef struct SignValue* SignTable;

///////////////////////////// referenced table for saving all references to other signs the file //////////////////
// connected list containing all sign referenced and the reference address

// define the struct for the reference table
struct referencedSign
{
	char*  lable;
	int    refereneAddress;
	int	   times;
	int    dupVal;
	int    offset;
	int    type;
	struct referencedSign* next;
};

//typedef struct referencedSign referenceCell;

typedef struct referencedSign* referenceTable;

/////////////////////////////  memory structure for every line processing ///////////////
// struct defining the operand command values
struct operand
{
	unsigned int Type;      // the operand type
	char*		 Value;    // operand string value
	unsigned int NumValue;// operand numeric value
	int          offset;  // offset if the operand is index input or register
};

// struct defining the command values
struct commandParts
{
	unsigned int opcode;          // command id
	unsigned int type;           // command type
	unsigned int comb_min;      // less segnificent bit of the word part 
	unsigned int comb_max;     // must segnificent bit of the word part 
	unsigned int opernad_num; // how many operands was in the command

	unsigned int time;       // how many time this command should run

	struct operand operands[2]; // using struct operand to save operands value
};

// creating instruction type for readability
typedef struct commandParts  command;

// struct defining the operand instruction  values
struct instructionParts
{
	int   instruction;  // instruction type
	char* stringValue;  // the string value of the instruction for string
	int*  dataValue;    // the numeric value of the instruction for data
	int   value_len;    // the len of the data
	char* lable;       // the lable value of the instruction for extern and entry
};

// creating instruction type for readability
typedef struct instructionParts instruction;


// define struct for all line possiblle parts of line
struct lineData
{
	char*		lineLable;        // lable for line if exists
	command		lineCommand;      // command data
	instruction lineInstruction;  // instruction value
};

// global variable - currline
struct lineData currLine;

// pointer to memory segment
int IC;

// pointer to data segment
int DC;

// global memory segment of word
word MemorySegment[MEMORY_SIZE];

// global memory for the type of command (a,e or r)
int MemoryType[MEMORY_SIZE];

// global data segment of word
word DataSegment[DATA_SIZE];

#endif