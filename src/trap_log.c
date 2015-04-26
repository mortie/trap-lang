#include "trap_log.h"

static size_t current_line = 0;
static int eexists = 0;

void trap_log(trap_log_type type, char* msg)
{
	char* prefix;

	switch (type)
	{
	case TRAP_E_INFO:
		prefix = "INFO:"; break;
	case TRAP_E_WARNING:
		prefix = "WARNING:"; break;
	case TRAP_E_ERROR:
		prefix = "ERROR:"; break;
	}

	if (type == TRAP_E_ERROR)
		eexists = 1;

	fprintf(stderr, "%s Line %i: %s\n", prefix, (int)current_line, msg);
}

void trap_log_line_set(size_t i)
{
	current_line = i;
}

size_t trap_log_line_get()
{
	return current_line;
}

void trap_log_line_increment()
{
	current_line += 1;
}

int trap_log_err_exists()
{
	return eexists;
}
