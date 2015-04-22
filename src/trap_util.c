#include "trap_util.h"

void trap_util_int_to_bin(size_t num, size_t nbits, trap_string* tstr)
{
	if (nbits)
	{
		trap_util_int_to_bin(num >> 1, nbits - 1, tstr);
		trap_string_append_char(tstr, ((num & 1) ? '1' : '0'));
	}
}
