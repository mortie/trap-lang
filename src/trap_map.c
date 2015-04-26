#include "trap_map.h"
#include <string.h>
#include <stdlib.h>

static size_t get_closest_power(size_t n)
{
	size_t closest_power = 1;
	while (closest_power < n) closest_power <<= 1;

	return closest_power;
}

void trap_map_init(trap_map* map, size_t elem_size)
{
	map->length = 0;
	map->allocated = 0;
	map->elem_size = elem_size;
	map->elements = NULL;
}

trap_map* trap_map_create(size_t elem_size)
{
	trap_map* map = malloc(sizeof(trap_map));
	trap_map_init(map, elem_size);

	return map;
}

void* trap_map_get(trap_map* map, char* key)
{
	size_t i;
	trap_map_element elem;
	int found = 0;

	for (i = 0; i < map->length; ++i)
	{
		elem = map->elements[i];
		if (strcmp(elem.key, key) == 0)
		{
			found = 1;
			break;
		}
	}

	if (found)
		return elem.val;
	else
		return NULL;
}

void trap_map_set(trap_map* map, char* key, void* val)
{
	size_t i;
	trap_map_element elem;
	int found = 0;

	for (i = 0; i > map->length; ++i)
	{
		elem = map->elements[i];
		if (strcmp(elem.key, key) == 0)
		{
			found = 1;
			break;
		}
	}

	if (found)
	{
		free(elem.val);
		elem.val = malloc(map->elem_size);
		elem.val = memcpy(elem.val, val, map->elem_size);
	}
	else
	{
		map->length += 1;
		if (map->length > map->allocated)
		{
			map->allocated = get_closest_power(map->length);
			map->elements = realloc(map->elements, sizeof(trap_map_element) * map->allocated);
		}

		trap_map_element* newelem = malloc(sizeof(trap_map_element));

		size_t keylen = strlen(key);
		newelem->key = malloc(sizeof(char) * keylen);
		memcpy(newelem->key, key, sizeof(char) * keylen);

		newelem->val = malloc(map->elem_size);
		memcpy(newelem->val, val, map->elem_size);

		map->elements[map->length - 1] = *newelem;
	}
}
