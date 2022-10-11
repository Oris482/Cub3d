#include "../cub3d.h"

#define ERROR -1
#define SUCCESS 1
#define FALSE 0

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

char	*ft_strchr(const char *s, const char c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == c)
			break ;
		s++;
	}
	if (*s == '\0')
		return (NULL);
	return ((char *)s);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (FALSE);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (FALSE);
		s1++;
		s2++;
	}
	if (*s1 || *s2)
		return (FALSE);
	return (SUCCESS);
}

char	*ft_substr(const char *start, const char *end)
{
	size_t	len;
	char	*ret;
	int		idx;

	len = end - start;
	if (len == 0)
		return (NULL);
	ret = (char *)malloc(len + 1);
	ret[len] = '\0';
	idx = 0;
	while (idx < len)
	{
		ret[idx++] = *start;
		start++;
	}
	return (ret);
}

char	*ft_strcpy(const char *s)
{
	size_t	len;
	int		idx;
	char	*ret;

	len = ft_strlen(s);
	ret = (char *)malloc(len + 1);
	ret[len] = '\0';
	idx = 0;
	while (idx < len)
	{
		ret[idx] = s[idx];
		idx++;
	}
	return (ret);
}