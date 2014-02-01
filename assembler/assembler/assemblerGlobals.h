#ifndef ASSEMBLER_DEFINES_H

#define ASSEMBLER_DEFINES_H

#include <string.h>

#define COMMAND_NUMBER 16
#define INSTRUCTION_NUMBER 4
#define SIGN_NUMBER 14
#define PREFIX_NUMBER 5
#define REGISTERS_NUMBER 8
#define INPUTS_NUMBER 4
#define TYPE_NUMBER 3

#define MAXLINE  80
#define MAXLABLE 30

#define LINE_TERMINATOR '\n'
#define STRING_TERMINATOR '\0'

#define ASSEMBLER_PARSER		"ASSEMBLER_PARSER"
#define ASSEMBLER_VALIDATOR		"ASSEMBLER_VALIDATOR"
#define ASSEMBLER_PROCESSOR		"ASSEMBLER_PROCESSOR"
#define ASSEMBLER_FILE_HANDLER "ASSEMBLER_FILE_HANDLER"


struct operand_data
{
	int arrInputsType[4];
};

struct command_data
{
	int command_type;
	int* sourceOperand;
	int* DestOperand;
};

enum ASSEMBLER_COMMAND_TYPE_DICTIONARY {ALL_BITS,
										TEN_BITS,
										ALL_BITS_CHAR = 48,
										TEN_BITS_CHAR = 49
									   };

enum ASSEMBLER_COMMAND_TIMES_COUNT {ONE_TIME = 1,
									TWO_TIMES = 2,
									THREE_TIMES = 3,
									ONE_TIMES_CHAR = 49,
									TWO_TIMES_CHAR = 50,
									THREE_TIMES_CHAR = 51,
								   };

enum ASSEMBLER_FILE_PREFIX_DICTIONRAY {SOURCE_SUFFIX,
									   TARGET_SUFFIX,
									   EXTERNAL_SUFFIX,
									   ENTERIES_SUFFIX,};

enum ASSEMBLER_SIGN_DICTIONRAY  {LABLE_SIGN, 
							    INSTRUCTION_SIGN, 
								TYPE_SIGN, 
								COMMA_SIGN, 
								COMMENT_SIGN, 
								MINUS_SIGN, 
								PLUS_SIGN, 
								STRING_SIGN,
								SLASH_SIGN,
								REGISTER_SIGN, 
								IMMEDIATE_INPUT_SIGN,
								OFFSET_INPUT_SIGN,
								OPEN_OFFSET_SIGN,
								CLOSE_OFFSET_SIGN
							   };

enum ASSEMBLER_INSTRUCTION_DICTIONARY {DATA, 
									   STRING, 
									   EXTERN, 
									   ENTRY
									  };

enum ASSEMBLER_REGISTERS_DICTIONARY {R0,
									 R1,
									 R2,
									 R3,
									 R4,
									 R5,
									 R6,
									 R7,
									 R0_CHR = 48,
									 R1_CHR, 
									 R2_CHR, 
									 R3_CHR, 
									 R4_CHR, 
									 R5_CHR, 
									 R6_CHR,
									 R7_CHR
									};

enum ASSEMBLER_INPUT_TYPES_DICTIONARY {IMMEDIATE_INPUT,
									   DIRECT_INPUT,
									   INDEX_INPUT,
									   REGISTER_INPUT
									   };

enum ASSEMBLER_COMMAND_TYPES_DICTIONARY {NO_OPERAND,
									     ONE_OPERAND,
										 TWO_OPERANDS
										};
													
enum ASSEMBLER_COMMAND_DICTIONARY {MOV,
								   CMP,
								   ADD,
								   SUB,
								   NOT,
								   CLR,
								   LEA,
								   INC,
								   DEC,
								   JMP,
								   BNE,
								   RED,
								   PRN,
								   JSR,
								   RTS,
								   STP
								  };

enum ASSEMBLER_ERROR_DICTIONARY  {LABLE_LEN_ERR, 
								  LABLE_EMPTY_FIRST_VALUE_ERR, 
								  LABLE_FIRST_ALPHA_ERR, 
								  LABLE_SAVED_WORD_ERR,
								  LABLE_SAVED_SIGN_ERR,

								  UNRECOGNIZE_INSTRUCTION_ERR, 

								  STRING_DEFINE_ERR, 
								  STRING_EMPTY_ERR,

								  UNVALID_NUMBER_ERR, 
								  SPACE_IN_NUMBER_ERR,
								  EXTRA_COMMA_IN_NUMBER_ERR,
								  EMPTY_DATA_ERR,

								  UNRECOGNIZE_SIGN_ERR, 
								  LINE_LEN_ERR,

								  UNVALID_COMMAND_FORMAT_ERR,
								  UNVALID_OPERAND_NUM_ERR,

								  TYPE_NOT_FOUND_ERR,
								  TYPE_UNVALID_ERR,
								  TIME_NOT_FOUND_ERR,
								  TIME_UNVALID_ERR,

								  OPERAND_FORMAT_ERR,
								  REGISTER_INPUT_ERR,
								  IMMEDIATE_INPUT_ERR,
								  INDEX_INPUT_ERR,
								  DIRECT_INPUT_ERR,

								  OPENING_FILE_ERR,
								  CLOSING_FILE_ERR
							     };


char* Assembler_instruction_Code_Table[INSTRUCTION_NUMBER];

char  Assembler_Signs_Code_Table[SIGN_NUMBER];

char* Assembler_file_prefix_Code_Table[PREFIX_NUMBER]; 

char* Assembler_registers_Code_Table[REGISTERS_NUMBER]; 

char* Assembler_Command_Code_Table[COMMAND_NUMBER]; 
struct command_data Assembler_command_data_Code_Table[COMMAND_NUMBER]; 
char* Assembler_inputs_format_Code_Table[INPUTS_NUMBER]; 
									
void init_globals();

char* CURR_FILE_NAME;
int   LINE_NUMBER;

#endif