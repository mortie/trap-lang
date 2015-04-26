#ifndef TRAP_COMPILE_H
#define TRAP_COMPILE_H

#include "trap_string.h"
#include "trap_map.h"
#include <stddef.h>

#define TRAP_LANG_PREFIX_REG 'R'

typedef enum trap_command
{
	COMMAND_ADD,
	COMMAND_SUB,
	COMMAND_SET,
	COMMAND_AND,
	COMMAND_OR,
	COMMAND_NAND,
	COMMAND_NOR,
	COMMAND_XOR,
	COMMAND_XNOR,
	COMMAND_RSHIFT,
	COMMAND_LSHIFT,
	COMMAND_INVERT,
	COMMAND_LABEL,
	COMMAND_GOTO,
	COMMAND_INPUT,
	COMMAND_OUTPUT,
	COMMAND_HALT,
	COMMAND_NONE
} trap_command;

typedef enum trap_arch
{
	TRAP_ARCH_TRAP_1,
	TRAP_ARCH_NONE
} trap_arch;

trap_arch trap_compile_get_arch(char* str);
const char* trap_compile_get_arch_str(trap_arch arch);

trap_command trap_compile_get_command(char* str);

trap_string* trap_compile_asm_to_bin(trap_arch arch, trap_string* tstr);

trap_string* trap_compile_bin_to_prog(trap_arch arch, trap_string* tstr);
trap_string* trap_compile_bin_to_prog_trap_1(trap_string* tstr);

void trap_compile_current_binline_increment();
size_t trap_compile_current_binline_get();

#endif
