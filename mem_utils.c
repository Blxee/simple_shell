#include "main.h"
#include <stdlib.h>

/**
 * get_mem_array - returns the address a static variable
 *
 * Return: the variable to keep track of memory in the program
 */
void *get_mem_array(void)
{
	static void *mem_array[MEM_ARRAY_SIZE];

	return (mem_array);
}

/**
 * alloc_mem - improved version of malloc(), that keeps track of memory
 *
 * @size: size of bytes to allocate
 *
 * Return: the address of the new memory
 */
void *alloc_mem(unsigned long size)
{
	void **mem_array = get_mem_array();
	void *ptr = malloc(size);

	if (ptr == NULL)
	{
		perror(*get_program_name());
		exit(127);
	}

	while (*mem_array != NULL)
		mem_array++;

	*mem_array = ptr;

	return (ptr);
}

/**
 * is_allocated - determines whether an address is heap allocated
 *
 * @ptr: the address to check
 *
 * Return: 1 id @ptr was found in mem_array and 0 if not
 */
int is_allocated(void *ptr)
{
	void **mem_array = get_mem_array();
	unsigned int i = 0;

	if (ptr)
	{
		while (i < MEM_ARRAY_SIZE)
		{
			if (mem_array[i] == ptr)
				return (1);
			i++;
		}
	}
	return (0);
}

/**
 * free_mem - improved free(), only frees memory once
 *
 * @ptr: the address of the memory to be freed
 */
void free_mem(void *ptr)
{
	void **mem_array = get_mem_array();
	unsigned int i = 0;

	if (ptr != NULL)
		while (i < MEM_ARRAY_SIZE)
		{
			if (mem_array[i] == ptr)
			{ /* only free if it is not freed yet */
				free(ptr);
				mem_array[i] = NULL;
				break;
			}
			i++;
		}
}

/**
 * free_all - frees all memory allocated by the program
 */
void free_all(void)
{
	void **mem_array = get_mem_array();
	unsigned int i = 0;

	while (i < MEM_ARRAY_SIZE)
	{
		if (mem_array[i] != NULL)
		{
			free(mem_array[i]);
			mem_array[i] = NULL;
		}
		i++;
	}
}
