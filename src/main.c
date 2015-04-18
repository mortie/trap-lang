#include "trap_string.h"
#include "asm_to_bin_str.h"
#include <stdio.h>

int main()
{
	trap_string* tstr = trap_string_create();

	char c;
	while ((c = getchar()) != EOF)
	{
		trap_string_append_char(tstr, c);
	}

	trap_string* binstr = asm_to_bin_str(tstr);

	return 0;
}
