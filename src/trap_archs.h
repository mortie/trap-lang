#ifndef TRAP_ARCHS_H
#define TRAP_ARCHS_H

#include "trap_compile.h"
#include "trap_string.h"

trap_string* trap_compile_bin_to_prog_trap_1(trap_string* tstr);
trap_string* trap_compile_asm_to_bin_trap_1(
	trap_command cmd,
	char** tokens,
	size_t numtokens,
	trap_map* labels);

#endif
