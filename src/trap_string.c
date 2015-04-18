#include "trap_string.h"

trap_string* trap_string_create()
{
	trap_string* tstr = malloc(sizeof(trap_string));
	tstr->length = 0;
	tstr->allocated = 1;
	tstr->chars = malloc(sizeof(char) * tstr->allocated);
}

void trap_string_append_char(trap_string* tstr, char c)
{
	tstr->length += 1;
	if (tstr->length > tstr->allocated)
	{
		tstr->allocated *= 2;
		tstr->chars = realloc(tstr->chars, sizeof(char) * tstr->allocated);
	}

	tstr->chars[tstr->length - 1] = c;
}

void trap_string_append_chars(trap_string* tstr, char* str, size_t len)
{
	size_t i;
	for (i = 0; i < len; ++i)
	{
		trap_string_append_char(tstr, str[i]);
	}
}

void trap_string_append_string(trap_string* tstr, trap_string* appendstr)
{
	trap_string_append_chars(tstr, appendstr->chars, appendstr->length);
}

void trap_string_set(trap_string* tstr, char* str, size_t len)
{
	tstr->length = len;
	tstr->allocated = (len > 0 ? len : 1);
	tstr->chars = realloc(tstr->chars, sizeof(char) * tstr->allocated);
	memcpy(tstr->chars, str, sizeof(char) * len);
}

void trap_string_free(trap_string* tstr)
{
	free(tstr->chars);
}
