#include <stdlib.h>

void	multi_free(void *ptr1, void *ptr2, void *ptr3, void *ptr4)
{
	free(ptr1);
	free(ptr2);
	free(ptr3);
	free(ptr4);
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
