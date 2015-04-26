#include "trap_archs.h"

trap_string* trap_compile_bin_to_prog(trap_arch arch, trap_string* binstr)
{
	switch (arch)
	{
	case TRAP_ARCH_TRAP_1:
		return trap_compile_bin_to_prog_trap_1(binstr);
	}
}
