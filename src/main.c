#include "trap_string.h"
#include "trap_compile.h"
#include "trap_log.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

static enum output_type
{
	OUT_BINARY,
	OUT_PROG
} output_type;

static int compile(trap_string* tstr, enum output_type outtype, trap_arch arch)
{
	fprintf(stderr, "Compiling to %s...\n\n", trap_compile_get_arch_str(arch));

	trap_string* binstr = trap_compile_asm_to_bin(TRAP_ARCH_TRAP_1, tstr);

	if (trap_log_err_exists())
		return 1;

	if (outtype == OUT_BINARY)
	{
		trap_string_println(binstr);
		fprintf(stderr, "Successfully compiled to binary.\n");
	}
	else if (outtype == OUT_PROG)
	{
		trap_string* progstr = trap_compile_bin_to_prog(TRAP_ARCH_TRAP_1, binstr);
		trap_string_println(progstr);
		fprintf(stderr, "Successfully compiled to prog.\n");
	}

	return 0;
}

int main(int argc, char** argv)
{
	int c;
	enum output_type outtype = OUT_BINARY;
	trap_arch arch = TRAP_ARCH_TRAP_1;

	while (1)
	{
		static struct option long_options[] =
		{
			{"help", no_argument, 0, 'h'},
			{"binary", no_argument, 0, 'b'},
			{"prog", no_argument, 0, 'p'},
			{"arch", required_argument, 0, 'a'},
			{0, 0, 0, 0}
		};

		//Getopt stores the index here
		int option_index = 0;

		c = getopt_long(argc, argv, "bpha:", long_options, &option_index);

		switch (c)
		{
		case 'h':
			fprintf(stderr, "%s takes trap-assembly through stdin, and sends output to stdout.\n", argv[0]);
			fprintf(stderr, "Flags:\n");
			fprintf(stderr, "\t-b, --binary: Output result as a string of 1s and 0s.\n");
			fprintf(stderr, "\t-p, --prog: Output result as a .prog-file.\n");
			return 0;

		case 'b':
			outtype = OUT_BINARY;
			break;

		case 'p':
			outtype = OUT_PROG;
			break;

		case 'a':
			arch = trap_compile_get_arch(optarg);
			break;
		}

		//Break when we're at the end of the options
		if (c == -1)
			break;
	}

	if (arch == TRAP_ARCH_NONE)
	{
		fprintf(stderr, "No such architecture.\n");
		return 1;
	}

	trap_string* tstr = trap_string_create();

	while ((c = getchar()) != EOF)
	{
		trap_string_append_char(tstr, c);
	}

	return compile(tstr, outtype, arch);
}
