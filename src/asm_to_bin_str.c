#include "asm_to_bin_str.h"
#include <stdio.h>

static short strings_equal(char* str1, char* str2)
{
	return (strcmp(str1, str2) == 0);
}

static size_t str_to_int(char* str)
{
	return atoi(str);
}

static void int_to_bin_str(size_t num, size_t nbits, trap_string* tstr)
{
	if (nbits)
	{
		int_to_bin_str(num >> 1, nbits - 1, tstr);
		trap_string_append_char(tstr, ((num & 1) ? '1' : '0'));
	}
}

static short is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static short append_bin_line(
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
	int_to_bin_str(str_to_int(arga + 1), IS_WIDTH_ARG_A, tmp);
	trap_string_append_string(binstr, tmp);
	trap_string_clear(tmp);

	//create readra binary
	trap_string_append_char(binstr, readra);

	//create arg B binary
	if (argb[0] == LANG_PREFIX_REG)
	{
		int_to_bin_str(str_to_int(argb + 1), IS_WIDTH_ARG_B, tmp);
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

static trap_string* create_bin(command cmd, char** tokens)
{
	trap_string* binstr = trap_string_create();

	switch(cmd)
	{
	case COMMAND_ADD:
		append_bin_line(binstr, "0001", '1', tokens[1], tokens[2]);
	case COMMAND_SUB:
		append_bin_line(binstr, "0010", '1', tokens[1], tokens[2]);
	}

	return binstr;
}

static trap_string* parse_line(trap_string* curline)
{
	char** tokens = NULL;
	size_t numtokens = 0;

	short parsing_token = 0;

	//tokenize
	size_t i;
	for (i = 0; i < curline->length; ++i)
	{
		char c = curline->chars[i];

		if (is_whitespace(c))
		{
			curline->chars[i] = '\0';
		}

		if (!parsing_token && !is_whitespace(c))
		{
			numtokens += 1;
			tokens = realloc(tokens, numtokens * sizeof(char*));
			tokens[numtokens - 1] = (curline->chars + i);

			parsing_token = 1;
		}
		else if (parsing_token && is_whitespace(c))
		{
			parsing_token = 0;
		}
	}

	for (i = 0; i < numtokens; ++i)
	{
		printf("Token: '%s'\n", tokens[i]);
	}

	command cmd;
	char* cmd_token = tokens[0];

	printf("%s\n", cmd_token);

	if (strings_equal(cmd_token, LANG_TOKEN_ADD))
		cmd = COMMAND_ADD;
	else if (strings_equal(cmd_token, LANG_TOKEN_SUB))
		cmd = COMMAND_SUB;
	else if (strings_equal(cmd_token, LANG_TOKEN_SET))
		cmd = COMMAND_SET;
	else if (strings_equal(cmd_token, LANG_TOKEN_AND))
		cmd = COMMAND_AND;
	else if (strings_equal(cmd_token, LANG_TOKEN_OR))
		cmd = COMMAND_OR;
	else if (strings_equal(cmd_token, LANG_TOKEN_NAND))
		cmd = COMMAND_NAND;
	else if (strings_equal(cmd_token, LANG_TOKEN_NOR))
		cmd = COMMAND_NOR;
	else if (strings_equal(cmd_token, LANG_TOKEN_XOR))
		cmd = COMMAND_XOR;
	else if (strings_equal(cmd_token, LANG_TOKEN_XNOR))
		cmd = COMMAND_XNOR;
	else if (strings_equal(cmd_token, LANG_TOKEN_RSHIFT))
		cmd = COMMAND_RSHIFT;
	else if (strings_equal(cmd_token, LANG_TOKEN_LSHIFT))
		cmd = COMMAND_LSHIFT;
	else if (strings_equal(cmd_token, LANG_TOKEN_INVERT))
		cmd = COMMAND_INVERT;
	else
		cmd = COMMAND_NONE;

	trap_string* binstr = create_bin(cmd, tokens);

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
