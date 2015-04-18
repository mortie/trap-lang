#include "asm_to_bin_str.h"
#include <stdio.h>

static trap_string* parse_line(trap_string* curline)
{
	trap_string* binstr = trap_string_create();

	printf("'%s' is one line.\n", curline->chars);

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
			trap_string_append_string(binstr, parse_line(curline));
			trap_string_set(curline, "", 0);
		}
		else
		{
			trap_string_append_char(curline, c);
		}
	}

	trap_string_free(curline);

	return binstr;
}
