#include "trap_archs.h"
#include "trap_util.h"
#include "trap_log.h"
#include <stdlib.h>

static trap_string* parse_line(trap_string* curline, trap_arch arch, trap_map* labels)
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

	char* cmd_token = tokens[0];

	trap_command cmd = trap_compile_get_command(cmd_token);

	trap_string* binstr;

	if (cmd == COMMAND_LABEL)
	{
		if (numtokens < 2)
		{
			trap_log(TRAP_E_ERROR, "Missing arguments.");
			return trap_string_create();
		}

		size_t linenum = trap_compile_current_binline_get();
		trap_map_set(labels, tokens[1], &linenum);

		binstr = trap_string_create();
	}
	else
	{
		switch (arch)
		{
		case TRAP_ARCH_TRAP_1:
			binstr = trap_compile_asm_to_bin_trap_1(
				cmd,
				tokens,
				numtokens,
				labels
			);
			break;
		}
	}

	free(tokens);

	return binstr;
}

trap_string* trap_compile_asm_to_bin(trap_arch arch, trap_string* tstr)
{
	trap_map* labels = trap_map_create(sizeof(size_t));

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
			trap_string_append_string(binstr, parse_line(curline, arch, labels));
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
