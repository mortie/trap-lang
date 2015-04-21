#ifndef ASM_TO_BIN_STR_H
#define ASM_TO_BIN_STR_H

#include "trap_string.h"
#include "trap_log.h"

#define LANG_PREFIX_REG 'R'
#define IS_WIDTH_OP_CODE 4
#define IS_WIDTH_ARG_A 4
#define IS_WIDTH_ARG_B 4
#define IS_WIDTH_DIRECT_B 8

trap_string* asm_to_bin_str(trap_string* tstr);

#endif
