#include "../trap_compile.h"
#include "../trap_util.h"
#include "../trap_log.h"

static int append_bin_line(
		trap_string* binstr,
		char op[IS_WIDTH_OP_CODE],
		char readra,
		char* arga,
		char* argb)
{
	if (arga[0] != LANG_PREFIX_REG)
	{
		trap_log(TRAP_E_ERROR, "Expected register.");
		return 0;
	}

	trap_string* tmp = trap_string_create();

	//create OP code binary
	trap_string_append_chars(binstr, op, IS_WIDTH_OP_CODE);

	//create direct B binary
	if (argb[0] == LANG_PREFIX_REG)
	{
		trap_util_int_to_bin(0, IS_WIDTH_DIRECT_B, tmp);
		trap_string_append_string(binstr, tmp);
	}
	else
	{
		trap_util_int_to_bin(atoi(argb), IS_WIDTH_DIRECT_B, tmp);
		trap_string_append_string(binstr, tmp);
	}
	trap_string_clear(tmp);

	//create arg A binary
	trap_util_int_to_bin(atoi(arga + 1), IS_WIDTH_ARG_A, tmp);
	trap_string_append_string(binstr, tmp);
	trap_string_clear(tmp);

	//create readra binary
	trap_string_append_char(binstr, readra);

	//create arg B binary
	if (argb[0] == LANG_PREFIX_REG)
	{
		trap_util_int_to_bin(atoi(argb + 1), IS_WIDTH_ARG_B, tmp);
		trap_string_append_string(binstr, tmp);
	}
	else
	{
		trap_util_int_to_bin(0, IS_WIDTH_ARG_B, tmp);
		trap_string_append_string(binstr, tmp);
	}

	trap_string_append_char(binstr, '\n');

	trap_string_free(tmp);
}

trap_string* trap_compile_asm_to_bin_trap_1(
		trap_command cmd,
		char** tokens,
		size_t numtokens)
{
	trap_string* binstr = trap_string_create();

	size_t required_tokens = 0;

	switch(cmd)
	{
	case COMMAND_ADD:
	case COMMAND_SUB:
	case COMMAND_SET:
	case COMMAND_AND:
	case COMMAND_OR:
	case COMMAND_NAND:
	case COMMAND_NOR:
	case COMMAND_XOR:
	case COMMAND_XNOR:
		required_tokens = 3;
		break;

	case COMMAND_RSHIFT:
	case COMMAND_LSHIFT:
	case COMMAND_INVERT:
		required_tokens = 2;
		break;

	case COMMAND_HALT:
		required_tokens = 1;
	}

	if (numtokens < required_tokens)
	{
		trap_log(TRAP_E_ERROR, "Not enough tokens.");
		return binstr;
	}

	switch(cmd)
	{
	case COMMAND_ADD:
		append_bin_line(binstr, "0001", '1', tokens[1], tokens[2]);
		break;

	case COMMAND_SUB:
		append_bin_line(binstr, "0010", '1', tokens[1], tokens[2]);
		break;

	case COMMAND_SET:
		append_bin_line(binstr, "0011", '0', tokens[1], tokens[2]);
		break;

	case COMMAND_AND:
		append_bin_line(binstr, "0101", '1', tokens[1], tokens[2]);
		break;

	//A = A NOR B
	//A = A NOR 0
	case COMMAND_OR:
		append_bin_line(binstr, "0100", '1', tokens[1], tokens[2]);
		append_bin_line(binstr, "0100", '1', tokens[1], "0");
		break;

	//A = A AND B
	//A = A NOR 0
	case COMMAND_NAND:
		append_bin_line(binstr, "0101", '1', tokens[1], tokens[2]);
		append_bin_line(binstr, "0100", '1', tokens[1], "0");
		break;

	case COMMAND_NOR:
		append_bin_line(binstr, "0100", '1', tokens[1], tokens[2]);
		break;

	case COMMAND_XOR:
		append_bin_line(binstr, "0110", '1', tokens[1], tokens[2]);
		break;

	//A = A XOR B
	//A = A NOR B
	case COMMAND_XNOR:
		append_bin_line(binstr, "0110", '1', tokens[1], tokens[2]);
		append_bin_line(binstr, "0100", '1', tokens[1], "0");
		break;

	case COMMAND_RSHIFT:
		append_bin_line(binstr, "0111", '0', tokens[1], tokens[1]);
		break;

	//A = A + A
	case COMMAND_LSHIFT:
		append_bin_line(binstr, "0001", '1', tokens[1], tokens[1]);
		break;

	//A = A NOR 0
	case COMMAND_INVERT:
		append_bin_line(binstr, "0100", '1', tokens[1], "0");
		break;

	case COMMAND_HALT:
		append_bin_line(binstr, "1111", '0', "R0", "0");
		break;

	case COMMAND_NONE:
		trap_log(TRAP_E_ERROR, "Unknown command.");
		break;
	}

	return binstr;
}
