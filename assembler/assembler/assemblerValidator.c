#include <string.h>
#include <ctype.h>
#include <stdlib.h>	

#include "assemblerValidator.h"
#include "AssemblerParser.h"
#include "AssemblerErrorHandler.h"
#include "assemblerGlobals.h"
#include "AssemblerDictionaries.h"
#include "AssemblerMemory.h"

// instruction inner function
int is_proper_data(char**);
int is_proper_string(char**);

// command inner function
int is_proper_operand(char** operandEnd, const int* arrOperandTypes, const int CommandID, const int operandNum);
int is_proper_lable_for_command(const char* lable);
int is_proper_times(char** timeValue);
int is_proper_type(char** typeValue, int cntr);

// operand inner functions
int is_proper_register_operand(const char* operandValue, int* offset);
int is_proper_direct_or_index_operand(char*, int*, int* offset);
int is_proper_immediate_operand(char* operandValue, int* numResult);

// check if the lable is proper according to the assembler roles
// input - 1. lableStart char pointer to the lable start,
//	       2. lableEnd char pointer - points to lable end 
//         3. the first char of the line - to terminate its not empty
// return - int value 1 if seccuss, 0 if fails
int is_proper_lable(const char* lableStart, const char* lableEnd, const char firstLine, const int lableType)
{	
	int ret_val = 0;
	int is_parsed = 0;

	char* lableParsed;

	// create the lable, and paresed lable without end :
	char* lable = create_word_by_pointers(lableStart, lableEnd);

	if (!lable)
	{
		log_error(ASSEMBLER_VALIDATOR, EMPTY_INSTRUCTION_LABLE_ERR, "");
	}
	// if lable contain lable sign
	else 
	{
		if (strchr(lable, Assembler_Signs_Code_Table[LABLE_SIGN]) != NULL)
		{
			lableParsed =  create_word_by_pointers(lableStart, lableEnd - 1);
		}
		else
		{
			lableParsed = lable;
			is_parsed = 1;
		}

		// check first character not empty or if there is only :
		if (isspace(firstLine) || lableEnd - lableStart + 1 == 0)
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
		// check if the lable is in use but not for entry and extern
		else if (lableType != INSTRUCTION_LABLE && !is_lable_free(lableParsed))
		{
			log_error(ASSEMBLER_VALIDATOR , LABLE_IN_USE_ERR, lableParsed);
		}
		// check if lable contains saved signs
		else if(is_contain_saved_sign(lableParsed, Assembler_Signs_Code_Table, SIGN_NUMBER) >= 0)
		{
			log_error(ASSEMBLER_VALIDATOR , LABLE_SAVED_SIGN_ERR, lable);
		}
		// check lable len no more then 30
		else if (lableEnd - lableStart >= MAXLABLE)
		{
			log_error(ASSEMBLER_VALIDATOR , LABLE_LEN_ERR, lable);
		}
		else
		{
			ret_val = 1;

			update_temp_line_value(lableType, lableParsed);
		}

		// free memory usage
		free(lable);

		if (lableParsed && !is_parsed)
		{
			free(lableParsed);
		}
	}

	return ret_val;
}

// check if the instruction is proper according to the assembler roles
// input - 1. char pointer to the instruction start,
//	       2. pointer to char pointer - that is a pointer to the current position in the current line processed 
// return - int value 1 if seccuss, 0 if fails
int is_proper_instruction(const char* instructionStart, char** InstructionEnd)
{
	int ret_val = 0;

	int lableType;
	int   instructionType;
	char* lableStart;
	char* instruction = create_word_by_pointers(instructionStart + 1, *InstructionEnd);

	if (!instruction)
	{
		log_error(ASSEMBLER_VALIDATOR, UNRECOGNIZE_SIGN_ERR, "");
	}
	else
	{
		(*InstructionEnd)++;

		// if its data instruction
		if (strcmp(instruction, Assembler_instruction_Code_Table[DATA]) == 0)
		{
			// check if orioer data
			ret_val =  is_proper_data(InstructionEnd);
		}
		// if it string instruction
		else if (strcmp(instruction, Assembler_instruction_Code_Table[STRING]) == 0)
		{
			// check if proper string
			ret_val =  is_proper_string(InstructionEnd);
		}
		// if it extern instruction or entry 
		else if (strcmp(instruction, Assembler_instruction_Code_Table[EXTERN]) == 0 ||
				 strcmp(instruction, Assembler_instruction_Code_Table[ENTRY]) == 0)
		{
			// find the lable start
			lableStart = find_word_start(*InstructionEnd);

			// increase the line pointer to the lable end
			*InstructionEnd += mov_to_word_end(lableStart, lableStart - *InstructionEnd);

			ret_val = is_proper_lable(lableStart, *InstructionEnd, lableStart[0], INSTRUCTION_LABLE);

			if (ret_val)
			{
				instructionType = (strcmp(instruction, Assembler_instruction_Code_Table[EXTERN]) == 0) ?
									EXTERN: ENTRY;	
				update_temp_line_value(INSTRUCTION, &instructionType);
			}

			//free memory 
			free(instruction);
		}
		// else it is just rubbish
		else
		{
			log_error(ASSEMBLER_VALIDATOR ,UNRECOGNIZE_INSTRUCTION_ERR, instruction);
		}
	}

	
	return ret_val;
}

// check if the instruction data value is proper according to the assembler roles
// input - pointer to char pointer - that is a pointer to the current position in the current line processed 
// return - int value 1 if seccuss, 0 if fails
int is_proper_data(char** endInstruction)
{	
	// array for saving the data number values
	unsigned int numbers[MAXLINE];
	int last_i = -1;
	int j = 0, i = 0;
	int value[1];
	int is_empty = 1;

	int sign = 1;
	int tmp_number = 0;

	int calc_num;

	int ret_val = 1;

	// go until end of string
	while (**endInstruction != STRING_TERMINATOR && **endInstruction != LINE_TERMINATOR) 
	{	
		// if its comma or new line - curr number is over
		if (**endInstruction == Assembler_Signs_Code_Table[COMMA_SIGN])
		{
			// if there was comma before, but no number between
			if (last_i < i) 
			{
				log_error(ASSEMBLER_VALIDATOR ,EXTRA_COMMA_IN_NUMBER_ERR,*endInstruction);
				i = -1;
				break;
			}
			else
			{
				// if everything ok - move to next number in array
				i++;	
			}
		}
		// if it plus or minus sign
		else if (**endInstruction == Assembler_Signs_Code_Table[PLUS_SIGN] ||
				 **endInstruction == Assembler_Signs_Code_Table[MINUS_SIGN])
		{
			// checki digits from next pose
			j = 1;

			// if minus - update sign
			if (**endInstruction == Assembler_Signs_Code_Table[MINUS_SIGN]) 
			{
				sign = -1;
			}

		}
		else if (isspace(**endInstruction))
		{
			;
		}
		// if its not digit and not all other options
		else if (!isdigit((*endInstruction)[j]))
		{
			// unrecognized sign
			log_error(ASSEMBLER_VALIDATOR ,UNVALID_NUMBER_ERR, *endInstruction);
			ret_val = 0;
			i = -1;

			break;
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
				log_error(ASSEMBLER_VALIDATOR ,UNVALID_NUMBER_ERR, *endInstruction);
				return 0;
			}
			// if everything ok - save the number!
			else
			{
				calc_num =  tmp_number * sign;

				// make the number value positive if negative 
				numbers[i] = (calc_num < 0)? complete_to_2(calc_num) : calc_num;

				is_empty = 0;
				// jump over all the digits but do extra for the increase in the end of loop
				(*endInstruction) += (j - 1);

				sign = 1;
				tmp_number = 0;
				last_i  = i;
				j = 0;
			}
		}	

		(*endInstruction)++;
	}

	// if i == 0 - no number douns
	if (is_empty)
	{
		log_error(ASSEMBLER_VALIDATOR ,EMPTY_DATA_ERR ,"");
		ret_val = 0;
	}

	// if foramt is ok
	if (ret_val == 1)
	{
		// save the instruction values
		value[0] = DATA;
		update_temp_line_value(INSTRUCTION, value);
		update_temp_line_value(DATA_VALUE, numbers);
		update_temp_line_value(VALUE_LEN, &last_i);
	}

	return ret_val;
}

// check if the instruction string value is proper according to the assembler roles
// input - pointer to char pointer - that is a pointer to the current position in the current line processed 
// return - int value 1 if seccuss, 0 if fails
int is_proper_string(char** endInstruction)
{
	int ret_val = 0;
	int value[1];
	int len;
	char* string;

	// find next word start
	char* stringStart = *endInstruction;
	char* stringEnd = stringStart;
	
	stringStart += mov_to_word_start(stringStart);

	// if next word exists
	if (*stringStart != STRING_TERMINATOR && 
		*stringStart != LINE_TERMINATOR)
	{
		// find word end
		stringEnd += mov_to_word_end(stringStart, stringStart - stringEnd);

		// if string value not in " "
		if (*stringStart != Assembler_Signs_Code_Table[STRING_SIGN] ||
			*stringEnd != Assembler_Signs_Code_Table[STRING_SIGN])
		{
			log_error(ASSEMBLER_VALIDATOR ,STRING_DEFINE_ERR, string);
		}
		else
		{
			// create the string
			string = create_word_by_pointers(stringStart + 1, stringEnd - 1);

			ret_val = 1;
			value[0] = STRING;
			len = (stringEnd - 1) - (stringStart + 1);
			update_temp_line_value(INSTRUCTION, value);
			update_temp_line_value(STRING_VALUE, string);
			update_temp_line_value(VALUE_LEN, &len);
		}

		free(string);
		*endInstruction += stringEnd - stringStart + 1;
	}
	else
	{
		log_error(ASSEMBLER_VALIDATOR, STRING_EMPTY_ERR, "" );
	}

	return ret_val;
}

// check if the command foramt is proper according to the assembler roles
// input - 1. commandStart - 
//         2. commandEnd a pointer to char pointer -  current position in the line processed 
// return - int value 1 if seccuss, 0 if fails
int is_proper_command(const char* commandStart ,char** commandEnd)
{
	char * command, *slashPose;
	struct command_data commandDefination;

	int    coammdnID; 
	int    i = 1;
	int    commandLen = 0;	
	int    value[1];

	int    ret_val = 0;

	// this saves full command including slash
	command		= create_word_by_pointers(commandStart, (*commandEnd));

	// find slash loc

	if (command)
	{
		slashPose = strchr(command, Assembler_Signs_Code_Table[SLASH_SIGN]);

		// if not exists - save inidcation to that - but still check the commend
		// maybe its unknown sign
		if (!slashPose)
		{
			commandLen =  strlen(command);
		}
		else
		{
			commandLen = slashPose - command;
		}	

		// free curr command 
		free(command);		

		// create it again for command only no slash
		command   = create_word_by_pointers(commandStart, commandStart + commandLen - 1);
		coammdnID = is_saved_word(command, Assembler_Command_Code_Table, COMMAND_NUMBER);

		// if the word is not recognized
		if (coammdnID == -1)
		{
			log_error(ASSEMBLER_PROCESSOR ,UNRECOGNIZE_SIGN_ERR, command);
		}
		// if the command exists - but missing slash
		else if (!slashPose)
		{
			log_error(ASSEMBLER_PROCESSOR ,UNVALID_COMMAND_FORMAT_ERR, "");
		}
		// if is real command - we need to check if it fits the right format of the command
		else
		{
			//move command end back to the end of the command name
			*commandEnd -= *commandEnd - commandStart;
			*commandEnd += commandLen;

			// check command type and times
			if(is_proper_type(commandEnd, 1))
			{
				(*commandEnd)++;

				// check if it proper time
				if  (is_proper_times(commandEnd))
				{
					// find curr command id definition
					commandDefination = Assembler_command_data_Code_Table[coammdnID];

					(*commandEnd)++;

					// if times is not seperated with space
					if (!isspace(**commandEnd))
					{
						// log error
						log_error(ASSEMBLER_VALIDATOR, UNRECOGNIZE_SIGN_ERR, *commandEnd);
					}
					else
					{
						// move on to data
						while (isspace(**commandEnd))
						{
							(*commandEnd)++;
						}

						// check according to command definition
						switch (commandDefination.command_type)
						{
							// if twp
							case TWO_OPERANDS:

								// check one - if error - return error
								ret_val = is_proper_operand(commandEnd, commandDefination.sourceOperand, coammdnID, 1);

								if (ret_val == 0)
								{
									break;
								}
								else
								{
									// if ok - move the curesor to next value
									// droping to one operand and doing that code too
									(*commandEnd)++;

									while (**commandEnd == Assembler_Signs_Code_Table[COMMA_SIGN] ||
											isspace(**commandEnd))
									{
										(*commandEnd)++;
									}
									
								}
							// for one operand (and second of two operands)
							case ONE_OPERAND:

								// check it
								ret_val = is_proper_operand(commandEnd, commandDefination.DestOperand, coammdnID, 2);
								break;

							case NO_OPERAND:
								ret_val = 1;
						}
					}
				}
			}
		}
	}

	// free memory
	free(command);

	// if operands ok - save them
	if (ret_val == 1)
	{
		value[0] = coammdnID;
		i = commandDefination.command_type;
		update_temp_line_value(OPCODE, value);
		update_temp_line_value(OP_NUM, &i);
	}

	return ret_val;
}

// check if the command type value is proper according to the assembler roles
// input - 1. typeStart - a pointer to char pointer -  current position in the line processed 
//         2. cntr - int indicating position in type (3 possible values)
// return - int value 1 if seccuss, 0 if fails
int is_proper_type(char** typeStart, int cntr)
{
	int ret_val = 1;
	int value;

	// if first sign not / or not digit value
	if (**typeStart != Assembler_Signs_Code_Table[SLASH_SIGN] ||
		!isdigit((*typeStart)[1])
	   )
	{
		log_error(ASSEMBLER_VALIDATOR ,TYPE_NOT_FOUND_ERR ,"" );
		ret_val = 0;
	}
	// type found - check its validity
	else 
	{
		(*typeStart)++;

		// if type value is not in the possible range
		if (**typeStart > TEN_BITS_CHAR || **typeStart < ALL_BITS_CHAR)
		{
			log_error(ASSEMBLER_VALIDATOR ,TYPE_UNVALID_ERR ,create_word_by_pointers(*typeStart, *typeStart + 1));
			ret_val = 0;
		}
		else
		{
			value = **typeStart - '0';
			
			// if everything ok save the value , add cntre to save position in type
			update_temp_line_value(TYPE + cntr - 1, &value);

			// if its ten bits take the rest type definition
			if ((**typeStart == TEN_BITS_CHAR || cntr > 1) && cntr < TYPE_NUMBER)
			{
				// recursivly check for next type (3 is max)
				(*typeStart)++;
				ret_val = is_proper_type(typeStart, ++cntr);
			}
		}
	}

	return ret_val;
}

// check if the command timee value is proper according to the assembler roles
// input - 1. timeStart - a pointer to char pointer -  current position in the line processed 
// return - int value 1 if seccuss, 0 if fails
int is_proper_times(char** time_start)
{
	int ret_val = 0;
	int value;

	// if format doesnt fit to time value
	if (**time_start != Assembler_Signs_Code_Table[COMMA_SIGN] ||
		!isdigit((*time_start)[1])
	   )
	{
		log_error(ASSEMBLER_VALIDATOR ,TIME_NOT_FOUND_ERR ,"");
	}
	// times found - check its validity
	else 
	{
		(*time_start)++;

		// if time value is not in range
		if (**time_start < ONE_TIMES_CHAR || **time_start > THREE_TIMES_CHAR)
		{
			log_error(ASSEMBLER_VALIDATOR ,TIME_UNVALID_ERR ,create_word_by_pointers(*time_start, *time_start + 1));
		}
		else
		{
			// if valid - save the value

			value = **time_start - '0';
			update_temp_line_value(TIME,&value);
			ret_val = 1;
		}
	}
		
	return ret_val;
}

// check if the command type value is proper according to the assembler roles
// input - 1. operandEnd - a pointer to char pointer -  current position in the line processed 
//         2. arrOperandTypes - array of int idicating type od inputs operand can have for this command
//		   3. commandID - int inidcating curr command
//         4. operandNUm - int inidcating which operand is testd (first or second)
// return - int value 1 if seccuss, 0 if fails
int is_proper_operand(char** operandEnd, const int* arrOperandTypes, const int commandID, const int operandNum)
{
	int ret_val   = 0;
	int valueType[1] = {-1};
	int offset;
	int numResul = 0;
	
	char* operandValue;
	char* operandStart =  find_word_start(*operandEnd);

	// if no value available
	if (*operandStart == STRING_TERMINATOR)
	{
		log_error(ASSEMBLER_VALIDATOR ,UNVALID_OPERAND_NUM_ERR , "" );
	}
	else
	{
		// inc line pointer
		*operandEnd +=  operandStart - *operandEnd;

		// read until operand breaks with ',' or space
		while (*((*operandEnd) + 1) != Assembler_Signs_Code_Table[COMMA_SIGN] &&
			   !isspace(*((*operandEnd) + 1)))
		{
			(*operandEnd)++;
		}

		operandValue = create_word_by_pointers(operandStart, *operandEnd);

		// now check the operand value match register operand
		if (is_proper_register_operand(operandValue, &offset))
		{
			// if this input is possible for command
			if (arrOperandTypes[REGISTER_INPUT])
			{
				valueType[0] = REGISTER_INPUT;
				ret_val = 1;
			}
			else
			{
				// log error
				log_error(ASSEMBLER_VALIDATOR, REGISTER_INPUT_ERR, operandValue);
			}
		
		}
		// now check the operand value match immediate operand
		else if (is_proper_immediate_operand(operandValue, &numResul))
		{
			// if this input is possible for command
			if (arrOperandTypes[IMMEDIATE_INPUT])
			{
				valueType[0] = IMMEDIATE_INPUT;
				ret_val = 1;
			}
			else
			{
				// log error
				log_error(ASSEMBLER_VALIDATOR, INDEX_INPUT_ERR, operandValue);
			}
		}
		// now check the operand value match direct or index operand
		else if (is_proper_direct_or_index_operand(operandValue, valueType, &offset))
		{	
			// if this input is possible for command
			if (valueType[0] == DIRECT_INPUT)
			{
				if (arrOperandTypes[DIRECT_INPUT])
				{
					ret_val = 1;
				}
				else
				{
					// log error
					log_error(ASSEMBLER_VALIDATOR, DIRECT_INPUT_ERR, operandValue);
				}
			} 
			else
			{
				if (arrOperandTypes[INDEX_INPUT])
				{
					ret_val = 1;
				}
				else
				{
					// log error
					log_error(ASSEMBLER_VALIDATOR, INDEX_INPUT_ERR, operandValue);
				}
			}
		}
		// nothing is good - error
		else
		{
			// log error
			log_error(ASSEMBLER_VALIDATOR, OPERAND_FORMAT_ERR, operandValue);
		}

		if (ret_val == 1)
		{
			// save data acoording to operand number
			if (operandNum == 1)
			{
				update_temp_line_value(SRC_TYPE, valueType);
				update_temp_line_value(SRC_OFFSET, &offset);
				update_temp_line_value(SRC_VALUE, operandValue);
				update_temp_line_value(SOURCE_IMMEDIATE_VALUE, &numResul);
			}
			else if (operandNum == 2)
			{
				update_temp_line_value(DEST_TYPE, valueType);
				update_temp_line_value(DEST_OFFSET, &offset);
				update_temp_line_value(DEST_VALUE, operandValue);
				update_temp_line_value(DEST_IMMEDIATE_VALUE, &numResul);
			}
		}

		// free memory;
		free(operandValue);
	}

	return ret_val;
}

// check if the operand value is proper immediate operand according to the assembler roles
// input - 1. operandValue - char* to the operand value
// return - int value 1 if seccuss, 0 if fails
int is_proper_immediate_operand(char* operandValue, int* numResult)
{
	int i = 0;	
	int j;
	int sign;
	char* numberValue;

	if (operandValue[i] == Assembler_Signs_Code_Table[IMMEDIATE_INPUT_SIGN])
	{
		i++;

		if (operandValue[i] == Assembler_Signs_Code_Table[MINUS_SIGN] ||
			operandValue[i] == Assembler_Signs_Code_Table[PLUS_SIGN])
		{
			sign = (operandValue[i] == Assembler_Signs_Code_Table[MINUS_SIGN]) ? -1 : 1;
			i++;
		}

		while(!isspace(operandValue[i]) &&
			  operandValue[i] != STRING_TERMINATOR)
		{
			j = i;

			if (!isdigit(operandValue[i]))
			{
				i = 0;
				break;
			}

			*numResult = (*numResult * 10) + (operandValue[i] - '0');

			i++;
		}

		if (i > 0 )
		{
			numberValue = create_word_by_pointers(operandValue + j, operandValue + i -1);

			strcpy(operandValue, numberValue);
			free(numberValue);

			*numResult *= sign;
		}
	}

	return i > 0;
}


// check if the operand value is proper register operand according to the assembler roles
// input - 1. operandValue - char* to the operand value
// return - int value 1 if seccuss, 0 if fails
int is_proper_register_operand(const char* operandValue, int* offset)
{	
	int ret_val = 0;

	// if the first sign is r and second is number between 0 to 7 and there is space afterward
	if (*operandValue == REGISTER_SIGN &&
		operandValue[1] >= R0_CHR && operandValue[1] <= R7_CHR )
	{
		// move the cursor forder
		ret_val = 1;
		*offset = operandValue[1] - '0';
	}

	return ret_val;
}

// check if the operand value is proper direct or index operand according to the assembler roles
// input - 1. lable - char* to the operand value
//         2. operandType = int* out paramter indicating the operand type found
// return - int value 1 if seccuss, 0 if fails
int is_proper_direct_or_index_operand(char* operandValue, int* operandType, int* offset)
{
	int i	    = 0;
	int ret_val = 0;

	char* lableValue;
	char* VarNameEnd = strchr(operandValue, Assembler_Signs_Code_Table[OPEN_OFFSET_SIGN]);

	*operandType = DIRECT_INPUT;

	// if it contain offset sign at all
	if (VarNameEnd)
	{
		// the type should be index input
		*operandType = INDEX_INPUT;

		// get lable value without index input sign
		lableValue = create_word_by_pointers(operandValue, VarNameEnd - 1);
	}
	// else just put all operand value
	else
	{
		lableValue = operandValue;
	}

	//check the lable
	ret_val = is_proper_lable_for_command(lableValue);
	
	// if lable is ok and its index input
	if (ret_val && (*operandType == (int)INDEX_INPUT))
	{
		ret_val = 0;

		// next sign should be @
		if (VarNameEnd[1] == Assembler_Signs_Code_Table[OFFSET_INPUT_SIGN])
		{
			VarNameEnd+= 2;
			*offset = 0;

			// count the num of digits in the offset
			while(isdigit(VarNameEnd[i]))
			{
				*offset = *offset * 10 + (VarNameEnd[i] - '0');
				i++;
			}

			// return 1 if there was digits and there is close ) 
			if (i > 0 && VarNameEnd[i] == Assembler_Signs_Code_Table[CLOSE_OFFSET_SIGN])
			{
				ret_val = 1;

				strcpy(operandValue, lableValue);
			}
		}
		
		free(lableValue);
	}

	return ret_val;
}

// check if the operand value is proper lable as part of command operand according to the assembler roles
// input - 1. lable - char* to the operand value
// return - int value 1 if seccuss, 0 if fails
int is_proper_lable_for_command(const char* lable)
{ 
	int ret_val = 0;

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
		// make sure doesnt contain saved signs
		else if(is_contain_saved_sign(lable, Assembler_Signs_Code_Table, SIGN_NUMBER) >= 0)
		{
			log_error(ASSEMBLER_VALIDATOR , LABLE_SAVED_SIGN_ERR, lable);
		}
		// check lable len no more then 30
		else if (strlen(lable)  > MAXLABLE)
		{
			log_error(ASSEMBLER_VALIDATOR , LABLE_LEN_ERR, lable);
		}
		else
		{
			//update_temp_line_value(INSTRUCTION_LABLE, lable);

			ret_val = 1;
		}
	}

	return ret_val;
}

