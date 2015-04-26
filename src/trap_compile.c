#include "trap_compile.h"
#include <string.h>

static const char* command_strings[] =
{
	"ADD",
	"SUB",
	"SET",
	"AND",
	"OR",
	"NAND",
	"NOR",
	"XOR",
	"XNOR",
	"RSHIFT",
	"LSHIFT",
	"INVERT",
	"LABEL",
	"GOTO",
	"IN",
	"OUT",
	"HALT"
};

static const char* arch_strings[] =
{
	"trap-1"
};

static size_t current_binline = 0;

void trap_compile_current_binline_increment()
{
	current_binline += 1;
}

size_t trap_compile_current_binline_get()
{
	return current_binline;
}

trap_command trap_compile_get_command(char* str)
{
	trap_command cmd = COMMAND_NONE;

	size_t i;
	for (i = 0; i < COMMAND_NONE; ++i)
	{
		if (strcmp(str, command_strings[i]) == 0)
		{
			cmd = i;
			break;
		}
	}

	return cmd;
}

trap_arch trap_compile_get_arch(char* str)
{
	trap_arch arch = TRAP_ARCH_NONE;

	size_t i;
	for (i = 0; i < TRAP_ARCH_NONE; ++i)
	{
		if (strcmp(str, arch_strings[i]) == 0)
		{
			arch = i;
			break;
		}
	}

	return arch;
}

const char* trap_compile_get_arch_str(trap_arch arch)
{
	size_t i = (size_t)arch;
	return arch_strings[i];
}
