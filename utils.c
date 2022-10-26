#include <stdlib.h>
#include "ft_string.h"

void	multi_free(void *ptr1, void *ptr2, void *ptr3, void *ptr4)
{
	free(ptr1);
	free(ptr2);
	free(ptr3);
	free(ptr4);
}

void	*my_malloc(size_t len)
{
	void	*ptr;

	ptr = malloc(len);
	if (ptr == NULL)
		exit_with_err("Malloc error", E_NOMEM);
	return (ptr);
}

void	ft_memset(void *ptr, unsigned char value, size_t size)
{
	size_t			idx;
	unsigned char	*target;

	target = (unsigned char *)ptr;
	idx = 0;
	while (idx < size)
		target[idx++] = value;
}

void	remove_newline(char *line)
{
	while (*line)
	{
		if (*line == '\n')
			*line = '\0';
		line++;
	}
}

void	set_range_double(double *to_set, double floor, double ceil)
{
	if (*to_set > ceil)
		*to_set = ceil;
	else if (*to_set < floor)
		*to_set = floor;
}

void	set_range_int(int *to_set, int floor, int ceil)
{
	if (*to_set > ceil)
		*to_set = ceil;
	else if (*to_set < floor)
		*to_set = floor;
}