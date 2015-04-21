#include "asm_to_bin_str.h"

static const char* command_strings[] =
{
	"ADD",
	"SUB",
	"SET",
	"AND",
	"OR",
	"NAND",
	"NOR",
	"XOR",
	"XNOR",
	"RSHIFT",
	"LSHIFT",
	"INVERT"
};

typedef enum command
{
	COMMAND_ADD,
	COMMAND_SUB,
	COMMAND_SET,
	COMMAND_AND,
	COMMAND_OR,
	COMMAND_NAND,
	COMMAND_NOR,
	COMMAND_XOR,
	COMMAND_XNOR,
	COMMAND_RSHIFT,
	COMMAND_LSHIFT,
	COMMAND_INVERT,
	COMMAND_NONE
} command;

static void int_to_bin_str(size_t num, size_t nbits, trap_string* tstr)
{
	if (nbits)
	{
		int_to_bin_str(num >> 1, nbits - 1, tstr);
		trap_string_append_char(tstr, ((num & 1) ? '1' : '0'));
	}
}

static int append_bin_line(
		trap_string* binstr,
		char op[IS_WIDTH_OP_CODE],
		char readra,
		char* arga,
		char* argb)
{
	if (arga[0] != LANG_PREFIX_REG)
		return 0;

	trap_string* tmp = trap_string_create();

	//create OP code binary
	trap_string_append_chars(binstr, op, IS_WIDTH_OP_CODE);

	//create direct B binary
	if (argb[0] == LANG_PREFIX_REG)
	{
		int_to_bin_str(0, IS_WIDTH_DIRECT_B, tmp);
		trap_string_append_string(binstr, tmp);
	}
	else
	{
		//do neat things, this is placeholder
		int_to_bin_str(0, IS_WIDTH_DIRECT_B, tmp);
		trap_string_append_string(binstr, tmp);
	}
	trap_string_clear(tmp);

	//create arg A binary
	int_to_bin_str(atoi(arga + 1), IS_WIDTH_ARG_A, tmp);
	trap_string_append_string(binstr, tmp);
	trap_string_clear(tmp);

	//create readra binary
	trap_string_append_char(binstr, readra);

	//create arg B binary
	if (argb[0] == LANG_PREFIX_REG)
	{
		int_to_bin_str(atoi(argb + 1), IS_WIDTH_ARG_B, tmp);
		trap_string_append_string(binstr, tmp);
	}
	else
	{
		int_to_bin_str(0, IS_WIDTH_ARG_B, tmp);
		trap_string_append_string(binstr, tmp);
	}

	trap_string_append_char(binstr, '\n');

	trap_string_free(tmp);
}

static trap_string* create_bin(command cmd, char** tokens, size_t numtokens)
{
	trap_string* binstr = trap_string_create();

	size_t required_tokens = 0;

	switch(cmd)
	{
	case COMMAND_ADD:
	case COMMAND_SUB:
		required_tokens = 3;
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
	case COMMAND_NONE:
		trap_log(TRAP_E_ERROR, "Unknown command.");
		break;
	}

	return binstr;
}

static trap_string* parse_line(trap_string* curline)
{
	char** tokens = NULL;
	size_t numtokens = 0;

	int parsing_token = 0;

	//tokenize
	size_t i;
	for (i = 0; i < curline->length; ++i)
	{
		char c = curline->chars[i];

		if (isspace(c))
		{
			curline->chars[i] = '\0';
			parsing_token = 0;
		}
		else if (!parsing_token)
		{
			numtokens += 1;
			tokens = realloc(tokens, numtokens * sizeof(char*));
			tokens[numtokens - 1] = (curline->chars + i);

			parsing_token = 1;
		}
	}

	if (numtokens < 1)
		return trap_string_create();

	command cmd = COMMAND_NONE;
	char* cmd_token = tokens[0];

	for (i = 0; i < COMMAND_NONE; ++i)
	{
		if (strcmp(cmd_token, command_strings[i]) == 0)
		{
			cmd = i;
			break;
		}
	}

	trap_string* binstr = create_bin(cmd, tokens, numtokens);

	//free everything
	free(tokens);

	return binstr;
}

trap_string* asm_to_bin_str(trap_string* tstr)
{
	trap_string* binstr = trap_string_create();
	trap_string* curline = trap_string_create();

	size_t i;
	for (i = 0; i < tstr->length; ++i)
	{
		char c = tstr->chars[i];

		if (c == '\n')
		{
			trap_log_line_increment();
			trap_string_append_char(curline, '\n');
			trap_string_append_string(binstr, parse_line(curline));
			trap_string_clear(curline);
		}
		else
		{
			trap_string_append_char(curline, c);
		}
	}

	trap_string_free(curline);

	return binstr;
}
