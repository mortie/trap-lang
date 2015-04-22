#include "trap_string.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static size_t get_closest_power(size_t n)
{
	size_t closest_power = 1;
	while (closest_power < n) closest_power <<= 1;

	return closest_power;
}

trap_string* trap_string_create()
{
	trap_string* tstr = malloc(sizeof(trap_string));
	tstr->length = 0;
	tstr->allocated = 0;
	tstr->chars = NULL;
}

void trap_string_init(trap_string* tstr)
{
	tstr->length = 0;
	tstr->allocated = 0;
	tstr->chars = NULL;
}

void trap_string_append_char(trap_string* tstr, char c)
{
	tstr->length += 1;
	if (tstr->length > tstr->allocated)
	{
		tstr->allocated = get_closest_power(tstr->length);
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
	tstr->allocated = get_closest_power(len);
	tstr->chars = realloc(tstr->chars, sizeof(char) * tstr->allocated);
	memcpy(tstr->chars, str, sizeof(char) * len);
}

void trap_string_print(trap_string* tstr)
{
	size_t i;
	for (i = 0; i < tstr->length; ++i)
	{
		putchar(tstr->chars[i]);
	}
}

void trap_string_println(trap_string* tstr)
{
	trap_string_print(tstr);
	putchar('\n');
}

void trap_string_free(trap_string* tstr)
{
	free(tstr->chars);
	free(tstr);
}

void trap_string_clear(trap_string* tstr)
{
	free(tstr->chars);
	tstr->length = 0;
	tstr->allocated = 0;
	tstr->chars = NULL;
}
