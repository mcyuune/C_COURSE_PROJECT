#include "assemblerGlobals.h"

void init_globals()
{
	struct command_data command;

	int arrAllInputTypes[INPUTS_NUMBER]		 = {1, 1, 1, 1};
	int arrThreeLastnputTypes[INPUTS_NUMBER] = {0, 1, 1, 1};
	int arrFirstInputTypes[INPUTS_NUMBER]	 = {1, 0, 0, 0};

	Assembler_instruction_Code_Table[DATA]   = "data";
	Assembler_instruction_Code_Table[STRING] = "string";
	Assembler_instruction_Code_Table[EXTERN] = "extern";
	Assembler_instruction_Code_Table[ENTRY]  = "entry";

	Assembler_Signs_Code_Table[LABLE_SIGN]		  = ':';
	Assembler_Signs_Code_Table[INSTRUCTION_SIGN]  = '.';
	Assembler_Signs_Code_Table[TYPE_SIGN]		  = '/';
	Assembler_Signs_Code_Table[COMMA_SIGN]		  = ',';
	Assembler_Signs_Code_Table[COMMENT_SIGN]	  = ';';
	Assembler_Signs_Code_Table[MINUS_SIGN]		  = '-';
	Assembler_Signs_Code_Table[SLASH_SIGN]		  = '/';
	Assembler_Signs_Code_Table[PLUS_SIGN]		  = '+';
	Assembler_Signs_Code_Table[STRING_SIGN]		  = '"';
	Assembler_Signs_Code_Table[IMMEDIATE_INPUT_SIGN]    = '#';
	Assembler_Signs_Code_Table[REGISTER_SIGN] = 'r';
	Assembler_Signs_Code_Table[OFFSET_INPUT_SIGN] = '@';
	Assembler_Signs_Code_Table[OPEN_OFFSET_SIGN] = '(';
	Assembler_Signs_Code_Table[CLOSE_OFFSET_SIGN] = ')';

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

	 Assembler_registers_Code_Table[R0] = "r0";
	 Assembler_registers_Code_Table[R1] = "r1";
	 Assembler_registers_Code_Table[R2] = "r2";
	 Assembler_registers_Code_Table[R3] = "r3";
	 Assembler_registers_Code_Table[R4] = "r4";
	 Assembler_registers_Code_Table[R5] = "r5";
	 Assembler_registers_Code_Table[R6] = "r6";
	 Assembler_registers_Code_Table[R7] = "r7";


	 Assembler_file_prefix_Code_Table[SOURCE_SUFFIX] = ".as";
	 Assembler_file_prefix_Code_Table[TARGET_SUFFIX] = ".ob";
	 Assembler_file_prefix_Code_Table[EXTERNAL_SUFFIX] = ".ext";
	 Assembler_file_prefix_Code_Table[ENTERIES_SUFFIX] = ".ent";

	 Assembler_inputs_format_Code_Table[IMMEDIATE_INPUT] = "%[#]c%d";
	 Assembler_inputs_format_Code_Table[DIRECT_INPUT]	 = "%30s";
	 Assembler_inputs_format_Code_Table[INDEX_INPUT]	 = "%30s[(@]%d[)]";
	 Assembler_inputs_format_Code_Table[REGISTER_INPUT]	 = "%[r]%d";

	 command.command_type = TWO_OPERANDS;
	 command.sourceOperand = arrAllInputTypes;
	 command.DestOperand = arrThreeLastnputTypes;

	 Assembler_command_data_Code_Table[MOV] = command;
	 Assembler_command_data_Code_Table[ADD] = command;
	 Assembler_command_data_Code_Table[SUB] = command;
	 
	 command.command_type = TWO_OPERANDS;
	 command.sourceOperand = arrAllInputTypes;
	 command.DestOperand = arrAllInputTypes; 
	 
	 Assembler_command_data_Code_Table[CMP] = command;

	 command.command_type  = TWO_OPERANDS;
	 command.sourceOperand  = arrThreeLastnputTypes;
	 command.DestOperand = arrThreeLastnputTypes; 
	 
	 Assembler_command_data_Code_Table[LEA] = command;


	 command.command_type  = ONE_OPERAND;
	 command.DestOperand = arrThreeLastnputTypes; 
	 
	 Assembler_command_data_Code_Table[NOT] = command;
	 Assembler_command_data_Code_Table[CLR] = command;
	 Assembler_command_data_Code_Table[INC] = command;
	 Assembler_command_data_Code_Table[DEC] = command;
	 Assembler_command_data_Code_Table[JMP] = command;
	 Assembler_command_data_Code_Table[BNE] = command;
	 Assembler_command_data_Code_Table[RED] = command;
	 


	 command.command_type = ONE_OPERAND;
	 command.DestOperand  = arrAllInputTypes;
	 
	 Assembler_command_data_Code_Table[PRN] = command;


	 command.command_type = ONE_OPERAND;
	 command.DestOperand  = arrFirstInputTypes;
	 
	 Assembler_command_data_Code_Table[JSR] = command;


	 command.command_type = NO_OPERAND;
	 
	 Assembler_command_data_Code_Table[RTS] = command;
	 Assembler_command_data_Code_Table[STP] = command;
}