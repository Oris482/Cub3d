#include "../cub3d.h"

#define ERROR -1
#define SUCCESS 1

size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (*s++)
		len++;
	return (len);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (ERROR);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (ERROR);
		s1++;
		s2++;
	}
	if (*s1 || *s2)
		return (ERROR);
	return (SUCCESS);
}
