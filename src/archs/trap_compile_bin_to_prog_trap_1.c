#include "../trap_archs.h"
#include <string.h>

trap_string* trap_compile_bin_to_prog_trap_1(trap_string* tstr)
{
	trap_string* progstr = trap_string_create();

	char* progheader = "1=REDSTONE_TORCH_OFF:RIGHT\n0=AIR\n|\n";
	trap_string_append_chars(progstr, progheader, strlen(progheader));

	size_t i;
	for (i = 0; i < tstr->length; ++i)
	{
		char c = tstr->chars[i];

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
}
