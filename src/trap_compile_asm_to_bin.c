#include "trap_compile.h"
#include "trap_util.h"
#include <stdlib.h>

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
	"INVERT",
	"HALT"
};

static const char* arch_strings[] =
{
	"trap-1"
};

static trap_string* parse_line(trap_string* curline, trap_arch arch)
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

	trap_command cmd = COMMAND_NONE;
	char* cmd_token = tokens[0];

	for (i = 0; i < COMMAND_NONE; ++i)
	{
		if (strcmp(cmd_token, command_strings[i]) == 0)
		{
			cmd = i;
			break;
		}
	}

	trap_string* binstr;

	switch (arch)
	{
	case TRAP_ARCH_TRAP_1:
		binstr = trap_compile_asm_to_bin_trap_1(
			cmd,
			tokens,
			numtokens
		);
		break;
	}

	//free everything
	free(tokens);

	return binstr;
}

trap_string* trap_compile_asm_to_bin(trap_arch arch, trap_string* tstr)
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
			trap_string_append_string(binstr, parse_line(curline, arch));
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

trap_arch trap_compile_get_arch(char* str)
{
	trap_arch arch = TRAP_ARCH_NONE;

	size_t i;
	for (i = 0; i < TRAP_ARCH_NONE; ++i)
	{
		if (strcmp(str, arch_strings[i]) == 0)
		{
			arch = i;
			break;
		}
	}

	return arch;
}

const char* trap_compile_get_arch_str(trap_arch arch)
{
	size_t i = (size_t)arch;
	return arch_strings[i];
}
