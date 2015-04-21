#include "trap_string.h"
#include "asm_to_bin_str.h"
#include "bin_str_to_prog.h"
#include "trap_log.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

static enum output_type
{
	OUT_BINARY,
	OUT_PROG
} output_type;

static int compile(enum output_type outtype)
{
	trap_string* tstr = trap_string_create();

	char c;
	while ((c = getchar()) != EOF)
	{
		trap_string_append_char(tstr, c);
	}

	trap_string* binstr = asm_to_bin_str(tstr);

	if (trap_log_err_exists())
		return 1;

	if (outtype == OUT_BINARY)
	{
		trap_string_println(binstr);
		fprintf(stderr, "Successfully compiled to binary.\n");
	}
	else if (outtype == OUT_PROG)
	{
		trap_string* progstr = bin_str_to_prog(binstr);
		trap_string_println(progstr);
		fprintf(stderr, "Successfully compiled to prog.\n");
	}

	return 0;
}

int main(int argc, char** argv)
{
	int c;
	enum output_type outtype = OUT_BINARY;

	while (1)
	{
		static struct option long_options[] =
		{
			{"help", no_argument, 0, 'h'},
			{"binary", no_argument, 0, 'b'},
			{"prog", no_argument, 0, 'p'},
			{0, 0, 0, 0}
		};

		//Getopt stores the index here
		int option_index = 0;

		c = getopt_long(argc, argv, "bph", long_options, &option_index);

		switch (c)
		{
		case 0:
			//If this option is a flag, do nothing else
			if (long_options[option_index].flag != 0)
				break;

			printf("Option %s", long_options[option_index].name);
			if (optarg)
				printf(" with argument %s", optarg);
			printf("\n");
			break;

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
		}

		//Break when we're at the end of the options
		if (c == -1)
			break;
	}
	return compile(outtype);
}
