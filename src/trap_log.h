#ifndef TRAP_LOG_H
#define TRAP_LOG_H

#include <stdio.h>

typedef enum trap_log_type
{
	TRAP_E_INFO,
	TRAP_E_WARNING,
	TRAP_E_ERROR
} trap_log_type;

void trap_log(trap_log_type type, char* msg);

void trap_log_line_set(size_t i);

void trap_log_line_increment();

int trap_log_err_exists();

#endif
