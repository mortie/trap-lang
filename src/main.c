#include "trap_string.h"
#include "asm_to_bin_str.h"
#include "trap_log.h"
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

	if (trap_log_err_exists())
		return 1;
	else
		trap_string_print(binstr);

	return 0;
}
