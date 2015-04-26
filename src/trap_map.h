#ifndef TRAP_MAP_H
#define TRAP_MAP_H

#include <stddef.h>

typedef struct trap_map_element
{
	void* val;
	char* key;
} trap_map_element;

typedef struct trap_map
{
	size_t length;
	size_t allocated;
	size_t elem_size;
	trap_map_element* elements;
} trap_map;

void trap_map_init(trap_map* map, size_t elem_size);

trap_map* trap_map_create(size_t elem_size);

void* trap_map_get(trap_map* map, char* key);

void trap_map_set(trap_map* map, char* key, void* val);

#endif
