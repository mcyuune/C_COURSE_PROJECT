#include "assemblerGlobals.h"
#include "AssemblerDictionaries.h"

// this function must be called in the beggining of assembler compileing
void init_AssemblerGlobals()
{
	// intalizing all data code tables
	struct command_data commandDefinition;

	// command input variaty options
	int arrAllInputTypes[INPUTS_NUMBER]		 = {1, 1, 1, 1};
	int arrThreeLastnputTypes[INPUTS_NUMBER] = {0, 1, 1, 1};
	int arrFirstInputTypes[INPUTS_NUMBER]	 = {1, 0, 0, 0};

	// intalize instruction
	Assembler_instruction_Code_Table[DATA]   = "data";
	Assembler_instruction_Code_Table[STRING] = "string";
	Assembler_instruction_Code_Table[EXTERN] = "extern";
	Assembler_instruction_Code_Table[ENTRY]  = "entry";

	// intalize sign
	Assembler_Signs_Code_Table[LABLE_SIGN]		     = ':';
	Assembler_Signs_Code_Table[INSTRUCTION_SIGN]     = '.';
	Assembler_Signs_Code_Table[TYPE_SIGN]		     = '/';
	Assembler_Signs_Code_Table[COMMA_SIGN]		     = ',';
	Assembler_Signs_Code_Table[COMMENT_SIGN]	     = ';';
	Assembler_Signs_Code_Table[MINUS_SIGN]		     = '-';
	Assembler_Signs_Code_Table[SLASH_SIGN]		     = '/';
	Assembler_Signs_Code_Table[PLUS_SIGN]		     = '+';
	Assembler_Signs_Code_Table[STRING_SIGN]		     = '"';
	Assembler_Signs_Code_Table[IMMEDIATE_INPUT_SIGN] = '#';
	Assembler_Signs_Code_Table[OFFSET_INPUT_SIGN]    = '@';
	Assembler_Signs_Code_Table[OPEN_OFFSET_SIGN]     = '(';
	Assembler_Signs_Code_Table[CLOSE_OFFSET_SIGN]    = ')';

	// intalize command
	Assembler_Command_Code_Table[MOV] = "mov";
	Assembler_Command_Code_Table[CMP] = "cmp";
	Assembler_Command_Code_Table[ADD] = "add";
	Assembler_Command_Code_Table[SUB] = "sub";
	Assembler_Command_Code_Table[NOT] = "not";
	Assembler_Command_Code_Table[CLR] = "clr";
	Assembler_Command_Code_Table[LEA] = "lea";
	Assembler_Command_Code_Table[INC] = "inc";
	Assembler_Command_Code_Table[DEC] = "dec";
	Assembler_Command_Code_Table[JMP] = "jmp";
	Assembler_Command_Code_Table[BNE] = "bne";
	Assembler_Command_Code_Table[RED] = "red";
	Assembler_Command_Code_Table[PRN] = "prn";
	Assembler_Command_Code_Table[JSR] = "jsr";
	Assembler_Command_Code_Table[RTS] = "rts";
	Assembler_Command_Code_Table[STP] = "stop";

	// intalize registers
	Assembler_registers_Code_Table[R0] = "r0";
	Assembler_registers_Code_Table[R1] = "r1";
	Assembler_registers_Code_Table[R2] = "r2";
	Assembler_registers_Code_Table[R3] = "r3";
	Assembler_registers_Code_Table[R4] = "r4";
	Assembler_registers_Code_Table[R5] = "r5";
	Assembler_registers_Code_Table[R6] = "r6";
	Assembler_registers_Code_Table[R7] = "r7";

	// intalize file prefix
	Assembler_file_prefix_Code_Table[SOURCE_SUFFIX]   = ".as";
	Assembler_file_prefix_Code_Table[TARGET_SUFFIX]   = ".ob";
	Assembler_file_prefix_Code_Table[EXTERNAL_SUFFIX] = ".ext";
	Assembler_file_prefix_Code_Table[ENTERIES_SUFFIX] = ".ent";

	// create type 1 command type
	commandDefinition.command_type = TWO_OPERANDS;
	commandDefinition.sourceOperand = arrAllInputTypes;
	commandDefinition.DestOperand = arrThreeLastnputTypes;

	// intalize matching command for this command type
	Assembler_command_data_Code_Table[MOV] = commandDefinition;
	Assembler_command_data_Code_Table[ADD] = commandDefinition;
	Assembler_command_data_Code_Table[SUB] = commandDefinition;
	 
	// create type 2 command type
	commandDefinition.command_type = TWO_OPERANDS;
	commandDefinition.sourceOperand = arrAllInputTypes;
	commandDefinition.DestOperand = arrAllInputTypes; 
	
	// intalize matching command for this command type
	Assembler_command_data_Code_Table[CMP] = commandDefinition;

	// create type 3 command type
	commandDefinition.command_type  = TWO_OPERANDS;
	commandDefinition.sourceOperand  = arrThreeLastnputTypes;
	commandDefinition.DestOperand = arrThreeLastnputTypes;  
	 
	// intalize matching command for this command type
	Assembler_command_data_Code_Table[LEA] = commandDefinition;

	// create type 4 command type
	commandDefinition.command_type  = ONE_OPERAND;
	commandDefinition.DestOperand = arrThreeLastnputTypes; 
	
	// intalize matching command for this command type
	Assembler_command_data_Code_Table[NOT] = commandDefinition;
	Assembler_command_data_Code_Table[CLR] = commandDefinition;
	Assembler_command_data_Code_Table[INC] = commandDefinition;
	Assembler_command_data_Code_Table[DEC] = commandDefinition;
	Assembler_command_data_Code_Table[JMP] = commandDefinition;
	Assembler_command_data_Code_Table[BNE] = commandDefinition;
	Assembler_command_data_Code_Table[RED] = commandDefinition;
	 
	// create type 5 command type
	commandDefinition.command_type = ONE_OPERAND;
	commandDefinition.DestOperand  = arrAllInputTypes;
	 
	// intalize matching command for this command type
	Assembler_command_data_Code_Table[PRN] = commandDefinition;

	// create type 6 command type
	commandDefinition.command_type = ONE_OPERAND;
	commandDefinition.DestOperand  = arrFirstInputTypes;
	 
	// intalize matching command for this command type
	Assembler_command_data_Code_Table[JSR] = commandDefinition;

	// create type 7 command type
	commandDefinition.command_type = NO_OPERAND;
	 
	// intalize matching command for this command type
	Assembler_command_data_Code_Table[RTS] = commandDefinition;
	Assembler_command_data_Code_Table[STP] = commandDefinition;
}