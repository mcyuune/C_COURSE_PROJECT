#include <stdlib.h>

#include "assemblerGlobals.h"
#include "AssemblerDictionaries.h"
#include "AssemblerMemory.h"
#include "AssemblerMemoryStructs.h"
#include "AssemblerFileHandler.h"
#include "AssemblerParser.h"
#include "AssemblerErrorHandler.h"

// connected list containing all sign accoured in the file
SignTable firstSign;
SignTable currSign;

// connected list containing all sign referenced and the reference address
referenceTable firstReference;
referenceTable currReference;
referenceTable prevReference;

void link_command_data(int val, int pose);
void insert_new_sign(int address, int type, char* sign);
void insert_reference_address(char* reference, int offset, int numToAdd);
void link_command_data(unsigned int val, int pose);
void duplicate_command(int counter);
void link_operand_data(int operandNum, int* numOfWords);
void save_instruction_data();
void save_command_data();
void init_values();


// this funsion add new sign to the sign table
// input : 1. int address - the sign address value
//         2. int type    - the type of the lable (r,e,a)
//         3. char* sign  - the value of the sign
void insert_new_sign(int address, int type, char* sign)
{
	// if the list is not empty
	if (currSign)
	{
		// add new empty cell
		currSign->next = (SignTable) malloc(sizeof(struct SignValue));
		currSign = currSign->next;
	}
	// if its empty
	else
	{
		// create the first cell
		firstSign =  (SignTable) malloc(sizeof(struct SignValue));
		currSign = firstSign;
	}

	// fill the new cell with the data
	currSign->address = address;
	currSign->lable = (char*) malloc(sizeof(char)* MAXLABLE);
	strcpy(currSign->lable , sign);
	currSign->type = type;
	currSign->next = NULL;
}

// this funsion add new referece to the referece table
// input : 1. char* reference - the reference value
//         2. int offset - the address the reference is being used
//         3. int - numToAdd - times value - how many times the reference is used
void insert_reference_address(char* reference, int offset, int numToAdd)
{
	// if the list is not empty
	if (currReference)
	{
		// add new empty cell
		currReference->next = (referenceTable) malloc( sizeof(struct referencedSign));
		currReference = currReference->next;
	}
	// if its empty
	else
	{
		// create the first cell
		firstReference = (referenceTable) malloc(sizeof(struct referencedSign));
		currReference = firstReference;
	}

	// fill the new cell with the data
	currReference->lable = (char*) malloc(sizeof(char) * MAXLABLE); 
	strcpy(currReference->lable , reference);
	currReference->refereneAddress = IC + numToAdd;
	currReference->offset = offset;
	currReference->next = NULL;
}

// this function add the command data to its pose using bit operation
// input : 1. unsigned int val - the value
//         2. int - pose - the location in the word size
void link_command_data(unsigned int val, int pose)
{
	// move the value to the location using bit operation
	val = val << pose;

	// add the value to already exsist data using or
	MemorySegment[IC].Value = MemorySegment[IC].Value | val;
}

// this function duplicate command (with all added word cells) for the time value
// input : 1. int counter - times to duplicate
void duplicate_command(int counter)
{
	int adder = 0;

	// do it by counter time
	while (counter > 0)
	{
		// duplicate the memory and type arrays
		check_memory_limits(IC + adder);

		MemorySegment[IC + adder].Value = MemorySegment[IC - counter].Value;
		MemoryType[IC + adder] = MemoryType[IC - counter];		
		counter--;
		adder++;
	}
}

// this function add all extra words for a command
// input : 1. int operandNum - num of operands to nadle
//         2. int* numOfWords - counter of the extra words nun
void link_operand_data(int operandNum, int* numOfWords)
{
	int i =0;

	(*numOfWords)++;

	// make sure there isnt memory flow
	check_memory_limits(IC + *numOfWords - 1);

	// if its direct input
	if (currLine.lineCommand.operands[operandNum].Type == IMMEDIATE_INPUT)
	{
		// Svae values
		MemorySegment[IC + *numOfWords - 1].Value  = currLine.lineCommand.operands[operandNum].NumValue;

		MemoryType[IC + *numOfWords - 1] = A;
	}
	// if it has lable reference
	else
	{
		// insert for the reference table for later fill
		insert_reference_address(currLine.lineCommand.operands[operandNum].Value, 
								 currLine.lineCommand.operands[operandNum].offset,
								 *numOfWords - 1);

		// if there is offset
		if (currLine.lineCommand.operands[operandNum].offset >= 0)
		{
			(*numOfWords)++;

			// make sure there isnt memory flow
			check_memory_limits(IC + *numOfWords - 1);

			// save it (for register value - for sign value will be replaced
			MemorySegment[IC + *numOfWords - 1].Value = currLine.lineCommand.operands[operandNum].offset;			
		}
	}
}

// function save instruction to memory
void save_instruction_data()
{
	int i;

	// check what is the instruction type
	switch(currLine.lineInstruction.instruction)
	{
		case DATA:
			// for data - save every number in the data segment
			for (i=0; i <= currLine.lineInstruction.value_len; i++)
			{
				DataSegment[DC].Value = currLine.lineInstruction.dataValue[i];
				DC++;

				// make sure there isnt memory flow
				check_memory_limits(DC);
			}
			break;

		case STRING:
			// for string - save every letter in the data segment
			for (i=0; i <= currLine.lineInstruction.value_len; i++)
			{
				DataSegment[DC].Value = currLine.lineInstruction.stringValue[i];
				DC++;

				// make sure there isnt memory flow
				check_memory_limits(DC);
			}

			// include line terminator
			DataSegment[DC].Value = STRING_TERMINATOR;
			DC++;

			// make sure there isnt memory flow
			check_memory_limits(DC);

			break;

		case ENTRY:
			// for entry- insert new sign
			insert_new_sign(DC, R ,currLine.lineInstruction.lable);
			break;

		case EXTERN:
			// for extern - insert new sign too
			insert_new_sign(DC, E ,currLine.lineInstruction.lable);
			break;
	}
}

// this function save command data to the memory
void save_command_data()
{
	int i = IC;
	int numOfWords = 1;
	int operandNum = SOURCE_OPERAND;
	int operand_type = 0;

	//create the command basic block
	link_command_data(currLine.lineCommand.type, TYPE_POSE);
	link_command_data(currLine.lineCommand.opcode, OPCODE_POSE);
	link_command_data(currLine.lineCommand.operands[SOURCE_OPERAND].Type, SRC_INPUT_POSE);
	link_command_data(currLine.lineCommand.operands[DEST_OPERAND].Type, DEST_INPUT_POSE);
	link_command_data(currLine.lineCommand.comb_min, COMB_POSE_MIN);
	link_command_data(currLine.lineCommand.comb_max, COMB_POSE_MAX);

	// save the type as A for command
	MemoryType[IC] = A;

	// if there are operands
	if (currLine.lineCommand.opernad_num)
	{
		// if the first is register type
		if (currLine.lineCommand.operands[DEST_OPERAND].Type == REGISTER_INPUT)
		{
			// save it to basic command word
			link_command_data(currLine.lineCommand.operands[DEST_OPERAND].offset, DEST_REGISTER_POSE);
		}
		// if its somthing else
		else
		{
			// link the data
			link_operand_data(DEST_OPERAND, &numOfWords);

			// if its uses reference - save it so it wount be lost
			if (currLine.lineCommand.operands[DEST_OPERAND].Type == DIRECT_INPUT || 
				currLine.lineCommand.operands[DEST_OPERAND].Type == INDEX_INPUT )
			{
				prevReference = currReference;	
			}
		}

		// if there is second oeprand
		if (currLine.lineCommand.opernad_num == 2)
		{   
			// if it is a register type
			if (currLine.lineCommand.operands[SOURCE_OPERAND].Type == REGISTER_INPUT)
			{
				// add it to word block
				link_command_data(currLine.lineCommand.operands[SOURCE_OPERAND].offset, SRC_REGISTER_POSE);
			}
			// diffrent type
			else
			{
				// link the data
				link_operand_data(SOURCE_OPERAND, &numOfWords);

				// if it has reference
				if (currLine.lineCommand.operands[SOURCE_OPERAND].Type == DIRECT_INPUT || 
				currLine.lineCommand.operands[SOURCE_OPERAND].Type == INDEX_INPUT )
				{
					// add the times and num of words
					currReference->times = currLine.lineCommand.time;
					currReference->dupVal = numOfWords;
				}
			}
		}
	}
	
	// if the first operand had reference (its saved because second operand can jump over it it)
	if (prevReference)
	{
		// save times and number of words
		prevReference->times = currLine.lineCommand.time;
		prevReference->dupVal = numOfWords;

		prevReference = NULL;
	}

	IC += numOfWords - 1;
	
	// make sure there isnt memory flow
	check_memory_limits(IC);

	// add it to memory according to time value
	for(i = 1; i < currLine.lineCommand.time; i++)
	{
		IC++;

		// make sure there isnt memory flow
		check_memory_limits(IC);

		// copy command in memory
		duplicate_command(numOfWords);

		IC += numOfWords - 1;

		// make sure there isnt memory flow
		check_memory_limits(IC);
	}

	IC++;

	// make sure there isnt memory flow
	check_memory_limits(IC);
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
		type = LC;
	}
	// else it was instruction line
	else if (currLine.lineInstruction.instruction != EMPTY_COMMAND)
	{
		lableAddress = DC;

		// handle the instruction by its type
		save_instruction_data();

		type = LD;
	}

	// if there was lable
	if (currLine.lineLable)
	{
		// save it to address table
		insert_new_sign(lableAddress, type, currLine.lineLable);
		currLine.lineLable = NULL;
	}

	// init tmp memory values for next line use
	init_values();
}

// this function checks if the char* exists in the entered signs dicrionay
// input:   1. value_type - enum for the line type value to save
//          2. value - void* pointer to the value
void update_temp_line_value(const int value_type, const void* value)
{
	// check what is the value we gut and save it
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

// function init the tmp memory values for next line use
void init_values()
{
	currLine.lineCommand.opcode   = EMPTY_COMMAND;
	currLine.lineCommand.comb_max = 0;
	currLine.lineCommand.comb_min = 0;
	currLine.lineCommand.operands[SOURCE_OPERAND].offset = -1;
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

// function creating the output compiled command file of the program
void create_program_file()
{
	int i;
	// open a file 
	FILE* output = open_file(WRITE, generate_file_name(TARGET_SUFFIX));

	// write the header
	write_to_file(PROGRAM_HEADER_OUTPUT_FORMAT, output, 0, NULL, 0, base6(IC), base6(DC));
	
	// write every line in the memory segment
	for (i=0; i < IC; i++)
	{
		int number_in_six =  base6(MemorySegment[i].Value);
		write_to_file(PROGRAM_BODY_OUTPUT_FORMAT, output, base6(i + MEMORY_START_ADDRESS), &number_in_six, MemoryType[i], 0, 0);
	}

	// write evert line in the data segment
	for (; i < IC + DC; i++)
	{
		int number_in_six =  base6(DataSegment[i - IC].Value);
		write_to_file(PROGRAM_SIGN_OUTPUT_FORMAT, output, base6(i + MEMORY_START_ADDRESS), &number_in_six , 0, 0, 0);
	}

	// close the file
	close_files(output);
}

void create_params_files()
{
	int address;
	// open a file for extern and entry
	FILE* entryf;
	FILE* externf = open_file(WRITE, generate_file_name(EXTERNAL_SUFFIX));
	char* entr;
	int it_found = 0;

	SignTable addressSearch;
	
	currReference = firstReference;

	// go over all referemces
	while (currReference)
	{
		// if its external
		if (currReference->type == E)
		{
			// write to file
			address = base6(currReference->refereneAddress + MEMORY_START_ADDRESS);
			write_to_file(EXTERN_FILE_OUTPUT_FORMAT, externf, address, currReference->lable, 0, 0, 0);
		}

		currReference = currReference->next;
	}

	close_files(externf);

	entryf  = open_file(WRITE, generate_file_name(ENTERIES_SUFFIX));

	currSign = firstSign;

	// go over all signs
	while (currSign)
	{
		// if its external
		if (currSign->type == R)
		{
			entr = currSign->lable;

			// keep moving until finding address
			addressSearch = firstSign;

			// while there are signs
			while (addressSearch)
			{
				// if they are match  and its not entry again
				if (addressSearch->type != R && addressSearch->type != E && strcmp(addressSearch->lable, entr) == 0)
				{
					// write to file
					address = addressSearch->address + MEMORY_START_ADDRESS + ((addressSearch->type == LD) ? IC : 0);
					address = base6(address);
					write_to_file(ENTRY_FILE_OUTPUT_FORMAT, entryf, address, addressSearch->lable, 0, 0, 0);

					it_found = 1;
					break;
				}

				addressSearch = addressSearch->next;
			}

			// if there wasnt lable for entry
			if (!it_found)
			{
				// anounce error and close
				log_error(ASSEMBLER_MEMORY, ENTRY_MISS_LABLE_ERR, currSign->lable);
				exit(1);
			}

			
			it_found = 0;
		}

		currSign = currSign->next;
	}

	// close the file
	close_files(entryf);
}

// this function handle the "second pass" of the file
// function goes over every open reference in the reference table, search for a mathcing sign in the sign table
// and updates the refereced data in the memory segment
void complete_sign_references()
{
	int i;
	int index;
	int is_set = 0;
	int base_index;
	currReference = firstReference;

	// go over all reference
	while(currReference)
	{
		// search for match in the sign table 
		currSign = firstSign;
		while (currSign && !is_set)
		{
			// if the sign match
			if (strcmp(currSign->lable, currReference->lable) == 0)
			{
				index = currReference->refereneAddress;

				// if its sign is not external
				if (currSign->type != E)
				{
					// save its value
					MemorySegment[index].Value = currSign->address + MEMORY_START_ADDRESS;

					// for DL (data segment sign) - add IC value for the offset  from base address
					if (currSign->type == LD)
					{
						MemorySegment[index].Value += IC;
					}						

					// save the type. if its DL or DC take R
					MemoryType[index] = (currSign->type == LD | currSign->type == LC) ? R : currSign->type;
				}
				// if it is external
				else
				{
					// the type is E and value is 0
					MemoryType[index] = E;
					MemorySegment[index].Value = 0;

					currReference->type = E;
				}				

				base_index = index;

				// update the address for all duplicate memory for the current place
				for (i = 1; i < currReference->times; i++)
				{
					index = currReference->refereneAddress + (i * currReference->dupVal);
					MemorySegment[index] = MemorySegment[base_index]; 
					MemoryType[index] = MemoryType[base_index];
				}

				// set the flag to found
				is_set = 1;
			}

			currSign = currSign->next;
		}

		// if there is a offset
		if (currReference->offset >= 0)
		{
			base_index = currReference->refereneAddress;

			// set first value and type all next
			MemorySegment[base_index + 1].Value = DataSegment[currReference->offset].Value;
			MemoryType[base_index + 1] = A;

			// update the address for all duplicate memory for the current place
			for (i = 1; i < currReference->times; i++)
			{
				index = currReference->refereneAddress + (i * currReference->dupVal) + 1;
				MemorySegment[index] = MemorySegment[base_index + 1]; 
				MemoryType[index] = MemoryType[base_index + 1];
			}
		}

		// if symbol wasnt found
		if (!is_set)
		{
			// anounce error and keep compiling
			log_error(ASSEMBLER_MEMORY, SIGN_NOT_FOUND_ERR, currReference->lable);

			MemoryType[ currReference->refereneAddress] = E;
		}

		// move to next reference
		currReference = currReference->next;
		is_set = 0;
	}
}

// function inits the memory for next file
void init_memory_for_new_file()
{
	// free sign table
	while (firstSign)
	{
		currSign  = firstSign;
		firstSign = firstSign->next;
		free(currSign);
	}

	// free sign table
	while (firstReference)
	{
		currReference = firstReference;
		firstReference = firstReference->next;
		free(currReference);
	}

	init_values();
}

// function makes sure there is no memory flow
int check_memory_limits(int address)
{
	// check if we reached memory limits
	if( address >= MEMORY_SIZE)
	{
		// if we did log error and exit
		log_error(ASSEMBLER_MEMORY, MEMORY_LIMITS_ERR, 0);

		exit(1);
	}
}

// function checked if lable is already in use
// input:    1. char* lable - the new lable
// return    int - 1 if free, and 0 if in use
int is_lable_free(char* lable)
{
	SignTable searched = firstSign;	

	// go over all saved signs
	while(searched)
	{
		// if there is a match
		// but dont check entry and extern because they are being used again
		if (searched->type != E && searched->type != R && strcmp(searched->lable, lable) == 0)
		{
			// return false;
			return 0;
		}

		searched = searched->next;
	}

	return 1;
}