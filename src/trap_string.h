#ifndef TRAP_STRING_H
#define TRAP_STRING_H

#include <stddef.h>

typedef struct trap_string
{
	char* chars;
	size_t length;
	size_t allocated;
} trap_string;

trap_string* trap_string_create();

void trap_string_append_char(trap_string* tstr, char c);

void trap_string_append_chars(trap_string* tstr, char* str, size_t len);

void trap_string_append_string(trap_string* tstr, trap_string* appendstr);

void trap_string_set(trap_string* tstr, char* str, size_t len);

void trap_string_print(trap_string* tstr);

void trap_string_println(trap_string* tstr);

void trap_string_free(trap_string* tstr);

void trap_string_clear(trap_string* tstr);

#endif
