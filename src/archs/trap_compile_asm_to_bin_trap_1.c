#include "../trap_archs.h"
#include "../trap_util.h"
#include "../trap_log.h"
#include <string.h>
#include <stdlib.h>

#define IS_WIDTH_OP_CODE 4
#define IS_WIDTH_ARG_A 4
#define IS_WIDTH_ARG_B 4
#define IS_WIDTH_DIRECT_B 8

static int append_bin_line(
		trap_string* binstr,
		char op[IS_WIDTH_OP_CODE],
		char readra,
		char* arga,
		char* argb)
{
	if (arga[0] != TRAP_LANG_PREFIX_REG)
	{
		trap_log(TRAP_E_ERROR, "Expected register.");
		return 0;
	}

	trap_string* tmp = trap_string_create();

	//create OP code binary
	trap_string_append_chars(binstr, op, IS_WIDTH_OP_CODE);

	//create direct B binary
	if (argb[0] == TRAP_LANG_PREFIX_REG)
	{
		trap_util_int_to_bin(0, IS_WIDTH_DIRECT_B, tmp);
		trap_string_append_string(binstr, tmp);
	}
	else
	{
		trap_util_int_to_bin(atoi(argb), IS_WIDTH_DIRECT_B, tmp);
		trap_string_append_string(binstr, tmp);
	}
	trap_string_clear(tmp);

	//create arg A binary
	trap_util_int_to_bin(atoi(arga + 1), IS_WIDTH_ARG_A, tmp);
	trap_string_append_string(binstr, tmp);
	trap_string_clear(tmp);

	//create readra binary
	trap_string_append_char(binstr, readra);

	//create arg B binary
	if (argb[0] == TRAP_LANG_PREFIX_REG)
	{
		trap_util_int_to_bin(atoi(argb + 1), IS_WIDTH_ARG_B, tmp);
		trap_string_append_string(binstr, tmp);
	}
	else
	{
		trap_util_int_to_bin(0, IS_WIDTH_ARG_B, tmp);
		trap_string_append_string(binstr, tmp);
	}

	trap_string_append_char(binstr, '\n');

	trap_string_free(tmp);

	trap_compile_current_binline_increment();

	return 0;
}

trap_string* trap_compile_asm_to_bin_trap_1(
		trap_command cmd,
		char** tokens,
		size_t numtokens,
		trap_map* labels)
{
	trap_string* binstr = trap_string_create();

	size_t required_tokens = 0;

	switch(cmd)
	{
	case COMMAND_ADD:
	case COMMAND_SUB:
	case COMMAND_SET:
	case COMMAND_AND:
	case COMMAND_OR:
	case COMMAND_NAND:
	case COMMAND_NOR:
	case COMMAND_XOR:
	case COMMAND_XNOR:
	case COMMAND_INPUT:
		required_tokens = 3;
		break;

	case COMMAND_RSHIFT:
	case COMMAND_LSHIFT:
	case COMMAND_INVERT:
	case COMMAND_GOTO:
		required_tokens = 2;
		break;

	case COMMAND_HALT:
		required_tokens = 1;
		break;
	default:
		break;
	}

	if (numtokens < required_tokens)
	{
		trap_log(TRAP_E_ERROR, "Not enough tokens.");
		return binstr;
	}

	switch(cmd)
	{
	case COMMAND_ADD:
		append_bin_line(binstr, "0001", '1', tokens[1], tokens[2]);
		break;

	case COMMAND_SUB:
		append_bin_line(binstr, "0010", '1', tokens[1], tokens[2]);
		break;

	case COMMAND_SET:
		append_bin_line(binstr, "0011", '0', tokens[1], tokens[2]);
		break;

	case COMMAND_AND:
		append_bin_line(binstr, "0101", '1', tokens[1], tokens[2]);
		break;

	//A = A NOR B
	//A = A NOR 0
	case COMMAND_OR:
		append_bin_line(binstr, "0100", '1', tokens[1], tokens[2]);
		append_bin_line(binstr, "0100", '1', tokens[1], "0");
		break;

	//A = A AND B
	//A = A NOR 0
	case COMMAND_NAND:
		append_bin_line(binstr, "0101", '1', tokens[1], tokens[2]);
		append_bin_line(binstr, "0100", '1', tokens[1], "0");
		break;

	case COMMAND_NOR:
		append_bin_line(binstr, "0100", '1', tokens[1], tokens[2]);
		break;

	case COMMAND_XOR:
		append_bin_line(binstr, "0110", '1', tokens[1], tokens[2]);
		break;

	//A = A XOR B
	//A = A NOR B
	case COMMAND_XNOR:
		append_bin_line(binstr, "0110", '1', tokens[1], tokens[2]);
		append_bin_line(binstr, "0100", '1', tokens[1], "0");
		break;

	case COMMAND_RSHIFT:
		append_bin_line(binstr, "0111", '0', tokens[1], tokens[1]);
		break;

	//A = A + A
	case COMMAND_LSHIFT:
		append_bin_line(binstr, "0001", '1', tokens[1], tokens[1]);
		break;

	//A = A NOR 0
	case COMMAND_INVERT:
		append_bin_line(binstr, "0100", '1', tokens[1], "0");
		break;

	case COMMAND_HALT:
		append_bin_line(binstr, "1111", '0', "R0", "0");
		break;

	case COMMAND_GOTO:
	{
		size_t* l = trap_map_get(labels, tokens[1]);

		if(l == NULL)
		{
			trap_log(TRAP_E_ERROR, "Unknown label.");
			break;
		}

		size_t line = *l;

		char linestr[11];
		memset(linestr, '\0', sizeof(char) * 11);
		sprintf(linestr, "%.*zu", 10, line);

		if (numtokens == 2)
		{
			append_bin_line(binstr, "1001", '1', "R0", "0");
			append_bin_line(binstr, "1010", '0', "R0", linestr);
		}
		else if (numtokens == 6)
		{
			if (strcmp(tokens[2], "IF") != 0)
			{
				trap_log(TRAP_E_ERROR, "Expected 'IF'.");
				break;
			}

			char* val1 = tokens[3];
			char* val2 = tokens[5];
			char* sign = tokens[4];

			if ((val1[0] != TRAP_LANG_PREFIX_REG
			||   val2[0] != TRAP_LANG_PREFIX_REG)
			&&  strcmp(sign, "==") != 0)
			{
				trap_log(TRAP_E_ERROR, "Both values must be registers.");
			}

			if (strcmp(sign, "==") == 0)
			{
				append_bin_line(binstr, "1001", '1', val1, val2);
				append_bin_line(binstr, "1010", '0', "R0", linestr);
			}
			else if (strcmp(sign, ">") == 0)
			{
				append_bin_line(binstr, "1000", '1', val1, val2);
				append_bin_line(binstr, "1010", '0', "R0", linestr);
			}
			else if (strcmp(sign, "<") == 0)
			{
				append_bin_line(binstr, "1000", '1', val2, val1);
				append_bin_line(binstr, "1010", '0', "R0", linestr);
			}
			else if (strcmp(sign, ">=") == 0)
			{
				append_bin_line(binstr, "1000", '1', val1, val2);
				append_bin_line(binstr, "1010", '0', "R0", linestr);
				append_bin_line(binstr, "1001", '1', val1, val2);
				append_bin_line(binstr, "1010", '0', "R0", linestr);
			}
			else if (strcmp(sign, "<=") == 0)
			{
				append_bin_line(binstr, "1001", '1', val1, val2);
				append_bin_line(binstr, "1010", '0', "R0", linestr);
				append_bin_line(binstr, "1000", '1', val2, val1);
				append_bin_line(binstr, "1010", '0', "R0", linestr);
			}
		}
		else
		{
			trap_log(TRAP_E_ERROR, "Expected 1 or 5 arguments.");
		}
		break;

	//No, this isn't pretty. It should be fixed some time.
	case COMMAND_INPUT:
	{
		size_t len = strlen(tokens[2]);
		char* str = malloc(sizeof(char) * (len + 1));
		memcpy(str + 1, tokens[2], len);
		str[0] = 'R';
		append_bin_line(binstr, "1011", '0', str, "0");
		free(str);

		append_bin_line(binstr, "0011", '0', tokens[1], "0");
		break;
	}

	//No, this isn't pretty. It should be fixed some time.
	case COMMAND_OUTPUT:
	{
		size_t len = strlen(tokens[2]);
		char* str = malloc(sizeof(char) * (len + 1));
		memcpy(str + 1, tokens[2], len);
		str[0] = 'R';
		append_bin_line(binstr, "1100", '0', str, tokens[1]);
		free(str);
		break;
	}

	default:
		break;
	}

	case COMMAND_NONE:
		trap_log(TRAP_E_ERROR, "Unknown command.");
		break;
	}

	return binstr;
}
