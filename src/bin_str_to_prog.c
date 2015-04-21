#include "bin_str_to_prog.h"

#include <stdio.h>

trap_string* bin_str_to_prog(trap_string* binstr)
{
	trap_string* progstr = trap_string_create();

	char* progheader = "1=REDSTONE_TORCH_OFF:RIGHT\n0=AIR\n|\n";
	trap_string_append_chars(progstr, progheader, strlen(progheader));

	size_t i;
	for (i = 0; i < binstr->length; ++i)
	{
		char c = binstr->chars[i];

		switch (c)
		{
		case '0':
			trap_string_append_chars(progstr, "0 ", 2);
			break;
		case '1':
			trap_string_append_chars(progstr, "1 ", 2);
			break;
		case '\n':
			trap_string_append_chars(progstr, ",++\n", 4);
			break;
		}
	}

	return progstr;
}
