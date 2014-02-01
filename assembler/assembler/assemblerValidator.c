#include <string.h>
#include <ctype.h>
#include <stdlib.h>	

#include "assemblerValidator.h"
#include "AssemblerParser.h"
#include "AssemblerErrorHandler.h"
#include "assemblerGlobals.h"
#include "AssemblerMemory.h"


int is_proper_data(char**);
int is_proper_string(char**);
int is_proper_operand(char** operandEnd, int* arrOperandTypes, int CommandID, int operandNum);
int is_proper_direct_operand(char* operandValue);
int is_proper_index_operand(char* operandValue);
int is_proper_register_operand(char* operandValue);
int is_proper_immediate_operand(char* operandValue);
int is_proper_lable_for_command(char* lable);



int is_proper_lable(char* lableStart, char** lableEnd, char firstLine)
{ 
	char* lable = create_word_by_pointers(lableStart, (*lableEnd));
	char* lableParsed; 
	
	if (strchr(lable, Assembler_Signs_Code_Table[LABLE_SIGN]) == STRING_TERMINATOR)
	{
		lableParsed = lable;
	}
	else
	{
		lableParsed =  create_word_by_pointers(lableStart, (*lableEnd) - 1);
	}

	// check first character not empty or if there is only :
	if (isspace(firstLine) || *lableEnd - lableStart == 0)
	{
		log_error(ASSEMBLER_VALIDATOR ,LABLE_EMPTY_FIRST_VALUE_ERR, lable);
	}
	// check first lable character alpa
	else if (!isalpha(*lableStart))
	{
		log_error(ASSEMBLER_VALIDATOR , LABLE_FIRST_ALPHA_ERR, lable);
	}
	// chack if saved word
	else if (is_saved_word(lableParsed, Assembler_Command_Code_Table, COMMAND_NUMBER) >= 0 ||
			 is_saved_word(lableParsed, Assembler_instruction_Code_Table, INSTRUCTION_NUMBER) >= 0 || 
			 is_saved_word(lableParsed, Assembler_registers_Code_Table, REGISTERS_NUMBER) >= 0 
			)
	{
		log_error(ASSEMBLER_VALIDATOR , LABLE_SAVED_WORD_ERR, lable);
	}
	else if(is_contain_saved_sign(lableParsed, Assembler_Signs_Code_Table, SIGN_NUMBER))
	{
		log_error(ASSEMBLER_VALIDATOR , LABLE_SAVED_SIGN_ERR, lable);
	}
	// check lable len no more then 30
	else if (*lableEnd - lableStart - 1 > MAXLABLE)
	{
		log_error(ASSEMBLER_VALIDATOR , LABLE_LEN_ERR, lable);
	}
	else
	{
		temp_save_value(LABLE, 0, lableParsed, 0);

		return 1;
	}

	return 0;
}

int is_proper_instruction(char* instructionStart, char** InstructionEnd)
{
	char* lableStart  = (char*) malloc (sizeof(char) * MAXLINE);

	char* instruction = create_word_by_pointers(instructionStart + 1, *InstructionEnd);

	(*InstructionEnd)++;

	// if its data instruction
	if (strcmp(instruction, Assembler_instruction_Code_Table[DATA]) == 0)
	{
		return is_proper_data(InstructionEnd);
	}
	// if it string instruction
	else if (strcmp(instruction, Assembler_instruction_Code_Table[STRING]) == 0)
	{
		return is_proper_string(InstructionEnd);
	}
	// if it extern instruction or entry 
	else if (strcmp(instruction, Assembler_instruction_Code_Table[EXTERN]) == 0 ||
		     strcmp(instruction, Assembler_instruction_Code_Table[ENTRY]) == 0)
	{
		lableStart = find_word_start(*InstructionEnd);
		*InstructionEnd = find_word_end(lableStart);
		return is_proper_lable(lableStart, InstructionEnd, lableStart[0]);
	}
	// else it is just rubbish
	else
	{
		log_error(ASSEMBLER_VALIDATOR ,UNRECOGNIZE_INSTRUCTION_ERR, instruction);
		return 0;
	}
}

int is_proper_data(char** endInstruction)
{	
	int numbers[MAXLINE];
	int last_i = -1;
	int j = 0;
	int i = 0;
	int sign = 1;
	int tmp_number = 0;

	// go until end of string
	while (**endInstruction != STRING_TERMINATOR) 
	{	
		// if space do nothing
		if (isspace(**endInstruction))
		{
			;
		}
		// if its comma 
		else if (**endInstruction == Assembler_Signs_Code_Table[COMMA_SIGN] ||
				 **endInstruction == LINE_TERMINATOR)
		{
			// if there was comma without number before
			if (last_i < i ) 
			{
				log_error(ASSEMBLER_VALIDATOR ,EXTRA_COMMA_IN_NUMBER_ERR,*endInstruction);
				return 0;
			}
			// if everything ok - end curr number
			else
			{
				i++;
			}
				
		}
		// if it plus or minus
		else if (**endInstruction == Assembler_Signs_Code_Table[PLUS_SIGN] ||
				 **endInstruction == Assembler_Signs_Code_Table[MINUS_SIGN])
		{
			// checki digits from next pose
			j = 1;

			if (**endInstruction == Assembler_Signs_Code_Table[MINUS_SIGN]) 
			{
				sign = -1;
			}

		}
		// if its not digit and not all other options
		else if (!isdigit((*endInstruction)[j]))
		{
			// unrecognized sign
			log_error(ASSEMBLER_VALIDATOR ,UNVALID_NUMBER_ERR,*endInstruction);
			return 0;
		}
		// else if its a digit
		if (isdigit((*endInstruction)[j]))
		{
			// find all numbers
			while(isdigit((*endInstruction)[j]))
			{
				tmp_number = tmp_number * 10 + ((*endInstruction)[j] - 48);			
				j++;
			}

			// if there was number seperated by space and not by comma (its save in the array)
			if (last_i == i)
			{
				log_error(ASSEMBLER_VALIDATOR ,UNVALID_NUMBER_ERR,*endInstruction);
				return 0;
			}
			// if everything ok - save the number!
			else
			{
				numbers[i] = tmp_number * sign;
				sign = 1;
				tmp_number = 0;
				last_i  = i;
				// jump over all the digits but do extra for the increase in the end of loop
				(*endInstruction) += (j - 1);

				j = 0;
			}
		}	

		(*endInstruction)++;
	}

	if (i == 0)
	{
		log_error(ASSEMBLER_VALIDATOR ,EMPTY_DATA_ERR ,"");
		return 0;
	}

	temp_save_value(INSTRUCTION, DATA, 0, 0);
	temp_save_value(INSTRUCTION_DATA, 0, 0, numbers);

	return 1;
}

int is_proper_string(char** endInstruction)
{
	char* string;
	char* stringStart = find_word_start((*endInstruction) + 1);
	char* stringEnd;
	
	if (*stringStart != STRING_TERMINATOR && 
		*stringStart != LINE_TERMINATOR)
	{
		stringEnd = find_word_end(stringStart);

		string = create_word_by_pointers(stringStart, stringEnd);

		if (*stringStart != STRING_SIGN ||
			*stringEnd != STRING_SIGN)
		{
			log_error(ASSEMBLER_VALIDATOR ,STRING_DEFINE_ERR, string);
			return 0;
		}

		*endInstruction = stringEnd;
	}
	else
	{
		log_error(ASSEMBLER_VALIDATOR, STRING_EMPTY_ERR, "" );
		return 0;
	}

	return 1;

	temp_save_value(INSTRUCTION, STRING, 0, 0);
	temp_save_value(INSTRUCTION_DATA, 0, string, 0);
}

int is_proper_command(char* commandStart ,char** commandEnd)
{
	char * command;
	struct command_data commandDefination;
	int coammdnID, result = 0, i = 1;


	*commandEnd = get_sign_position(commandStart, *commandEnd, Assembler_Signs_Code_Table[SLASH_SIGN]);

	if (*commandEnd == NULL)
	{
		log_error(ASSEMBLER_PROCESSOR ,UNVALID_COMMAND_FORMAT_ERR, "");
		result = 0;
	}
	else
	{
		command = create_word_by_pointers(commandStart, (*commandEnd) - 1);

		coammdnID = is_saved_word(command, Assembler_Command_Code_Table, COMMAND_NUMBER);

		// if the word is not recognized
		if (coammdnID == -1)
		{
			log_error(ASSEMBLER_PROCESSOR ,UNRECOGNIZE_SIGN_ERR, command);
			result = 0;
		}
		// if is real command - we need to check if it fits the right format of the command
		else
		{
			// check command type and times
			if(is_proper_type(commandEnd, 1))
			{
				 (*commandEnd)++;

				 if  (is_proper_times(commandEnd))
				 {

					commandDefination = Assembler_command_data_Code_Table[coammdnID];

					(*commandEnd)++;

					// if times is not seperated with space
					if (!isspace(**commandEnd))
					{
						// log error
						result = 0;
					}
					else
					{
						// go to data
						while (isspace(**commandEnd))
						{
							(*commandEnd)++;
						}

						switch (commandDefination.command_type)
						{
							case TWO_OPERANDS:

								result = is_proper_operand(commandEnd, commandDefination.sourceOperand, coammdnID, i);

								if (result == 0)
								{
									break;
								}
								else
								{
									i++;
									(*commandEnd)++;
									while (**commandEnd == Assembler_Signs_Code_Table[COMMA_SIGN] ||
										   isspace(**commandEnd))
									{
										(*commandEnd)++;
									}
									
								}

							case ONE_OPERAND:

								result = is_proper_operand(commandEnd, commandDefination.DestOperand, coammdnID, i);
								break;

							case NO_OPERAND:
								result = 1;
						}
					}
				}
			}
		}
	}

	if (result == 1)
	{
		temp_save_value(COMMAND, coammdnID, 0, 0);
	}

	return result;
}

int is_proper_type(char** typeStart, int cntr)
{
	int result = 1;

	if (**typeStart != Assembler_Signs_Code_Table[SLASH_SIGN] ||
		!isdigit((*typeStart)[1])
	   )
	{
		log_error(ASSEMBLER_VALIDATOR ,TYPE_NOT_FOUND_ERR ,"" );
		result = 0;
	}
	// type found - check its validity
	else 
	{
		(*typeStart)++;

		if (**typeStart > TEN_BITS_CHAR || **typeStart < ALL_BITS_CHAR)
		{
			log_error(ASSEMBLER_VALIDATOR ,TYPE_UNVALID_ERR ,create_word_by_pointers(*typeStart, *typeStart + 1));
			result = 0;
		}
		else
		{
			temp_save_value(TYPE + cntr - 1, (int) *typeStart, 0 , 0);

			// if its ten bits take the rest type definition
			if ((**typeStart == TEN_BITS_CHAR || cntr > 1) && cntr < TYPE_NUMBER)
			{
				(*typeStart)++;
				result = is_proper_type(typeStart, ++cntr);
			}
		}
	}

	return result;
}

int is_proper_times(char** time_start)
{
	if (**time_start != Assembler_Signs_Code_Table[COMMA_SIGN] ||
		!isdigit((*time_start)[1])
	   )
	{
		log_error(ASSEMBLER_VALIDATOR ,TIME_NOT_FOUND_ERR ,"");
		return 0;
	}
	// times found - check its validity
	else 
	{
		(*time_start)++;

		if (**time_start < ONE_TIMES_CHAR || **time_start > THREE_TIMES_CHAR)
		{
			log_error(ASSEMBLER_VALIDATOR ,TIME_UNVALID_ERR ,create_word_by_pointers(*time_start, *time_start + 1));
			return 0;
		}	
	}
		
	temp_save_value(TIME, (int) *time_start, 0, 0);
	return 1;
}

int is_proper_operand(char** operandEnd, int* arrOperandTypes, int commandID, int operandNum)
{
	int valueType = -1;

	char* operandValue = (char*) malloc (sizeof(char) * MAXLINE);
	char* operandStart =  find_word_start(*operandEnd);

	if (*operandStart == STRING_TERMINATOR)
	{
		log_error(ASSEMBLER_VALIDATOR ,UNVALID_OPERAND_NUM_ERR , "" );
		return 0;
	}

	*operandEnd = operandStart;

	// read until operand breaks with , or space
	while (*((*operandEnd) + 1) != Assembler_Signs_Code_Table[COMMA_SIGN] &&
		   !isspace(*(*operandEnd) + 1))
	{
		(*operandEnd)++;
	}

	operandValue = create_word_by_pointers(operandStart, *operandEnd);

	// now check the operand by the operand definition array
	if (is_proper_register_operand(operandValue))
	{
		if (arrOperandTypes[REGISTER_INPUT])
		{
			valueType = REGISTER_INPUT;
		}
		else
		{
			// log error
			log_error(ASSEMBLER_VALIDATOR, REGISTER_INPUT_ERR, operandValue);
			return 0;
		}
		
	}
	else if (is_proper_immediate_operand(operandValue))
	{
		if (arrOperandTypes[IMMEDIATE_INPUT])
		{
			valueType = IMMEDIATE_INPUT;
		}
		else
		{
			// log error
			log_error(ASSEMBLER_VALIDATOR, INDEX_INPUT_ERR, operandValue);
			return 0;
		}
	}
	else if (is_proper_index_operand(operandValue))
	{
		if (arrOperandTypes[INDEX_INPUT])
		{
			valueType = INDEX_INPUT;
		}
		else
		{
			// log error
			log_error(ASSEMBLER_VALIDATOR, INDEX_INPUT_ERR, operandValue);
			return 0;
		}
	}
	else if (is_proper_direct_operand(operandValue))
	{	
		if (arrOperandTypes[DIRECT_INPUT])
		{
			valueType = DIRECT_INPUT;
		}
		else
		{
			// log error
			log_error(ASSEMBLER_VALIDATOR, DIRECT_INPUT_ERR, operandValue);
			return 0;
		}
	}
	// nothing is good - error
	else
	{
		// log error
		log_error(ASSEMBLER_VALIDATOR, OPERAND_FORMAT_ERR, operandValue);
		return 0;
	}

	// save data acoording to operand number
	if (operandNum == 1)
	{
		temp_save_value(OPERAND_1_TYPE, valueType, 0, 0);
		temp_save_value(OPERAND_1_DATA, 0, operandValue, 0);
	}
	else
	{
		temp_save_value(OPERAND_2_TYPE, valueType, 0, 0);
		temp_save_value(OPERAND_2_DATA, 0, operandValue, 0);
	}

	return 1;
}

int is_proper_direct_operand(char* operandValue)
{	
	// replace this with the data check

	if (is_proper_lable_for_command(operandValue))
	{
		if(is_contain_saved_sign(operandValue, Assembler_Signs_Code_Table, SIGN_NUMBER) == -1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

int is_proper_index_operand(char* operandValue)
{
	int i = 0;

	char* VarNameEnd = strchr(operandValue, Assembler_Signs_Code_Table[OPEN_OFFSET_SIGN]);

	// if it doesn't contain offset sign at all
	if (VarNameEnd == NULL)
	{
		return 0;
	}
	// if it contains - check the lable
	else if (!is_proper_lable_for_command(create_word_by_pointers(operandValue, VarNameEnd - 1)))
	{
		return 0;
	}
	// next sign should be @
	else if (VarNameEnd[1] != Assembler_Signs_Code_Table[OFFSET_INPUT_SIGN])
	{
		return 0;
	}
	else
	{
		VarNameEnd+= 2;

		// count the num of digits in the offset
		while(isdigit(VarNameEnd[i]))
		{
			i++;
		}

		// return 1 if there was digits and there is close ) and there is space after
		return (i > 0 && VarNameEnd[i] == Assembler_Signs_Code_Table[CLOSE_OFFSET_SIGN] &&
				VarNameEnd[i + 1] == STRING_TERMINATOR);
	}
}

int is_proper_register_operand(char* operandValue)
{	
	// if the first sign is r and second is number between 0 to 7 and there is space afterward
	if (*operandValue == Assembler_Signs_Code_Table[REGISTER_SIGN] &&
		operandValue[1] >= R0_CHR && operandValue[1] <= R7_CHR )
	{
		// move the cursor forder
		return 1;
	}

	return 0;
}

int is_proper_immediate_operand(char* operandValue)
{
	int i = 0;	

	if (operandValue[i] == Assembler_Signs_Code_Table[IMMEDIATE_INPUT_SIGN])
	{
		i++;

		if (operandValue[i] == Assembler_Signs_Code_Table[MINUS_SIGN] ||
			operandValue[i] == Assembler_Signs_Code_Table[PLUS_SIGN])
		{
			i++;
		}

		while(!isspace(operandValue[i]) &&
			  operandValue[i] != Assembler_Signs_Code_Table[COMMA_SIGN])
		{
			if (!isdigit(operandValue[i]))
			{
				i = 0;
				break;
			}

			i++;
		}
	}

	return i > 0;
}

int is_proper_lable_for_command(char* lable)
{ 
	// check first lable character alpa
	if (isalpha(*lable))
	{
		// chack if saved word
		if (is_saved_word(lable, Assembler_Command_Code_Table, COMMAND_NUMBER) >= 0 ||
			is_saved_word(lable, Assembler_instruction_Code_Table, INSTRUCTION_NUMBER) >= 0 || 
			is_saved_word(lable, Assembler_registers_Code_Table, REGISTERS_NUMBER) >= 0 
		   )
		{
			log_error(ASSEMBLER_VALIDATOR , LABLE_SAVED_WORD_ERR, lable);
		}
		// check lable len no more then 30
		else if (strlen(lable)  <= MAXLABLE)
		{
			temp_save_value(LABLE, 0, lable, 0);

			return 1;
		}
	}

	return 0;
}

