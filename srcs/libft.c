#include "ft_traceroute.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strncmp(const char *s1, const char *s2, unsigned int n)
{
	while (*s1 != '\0' && *s2 != '\0' && n > 0)
	{
		if (*s1 != *s2)
			break ;
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*ptr_b;

	i = 0;
	ptr_b = (unsigned char *)b;
	while (i < len)
		ptr_b[i++] = (unsigned char)c;
	return (b);
}

int	ft_atoi(const char *str)
{
	int					sign;
	int					i;
	unsigned long long	n;

	i = 0;
	sign = 1;
	n = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign *= -1;
	while (str[i] >= 48 && str[i] <= 57)
		n = (n * 10) + (str[i++] - 48);
	if (n >= 9223372036854775808ULL)
	{
		if (sign == 1)
			return (-1);
		else
			return (0);
	}
	return ((int)n * sign);
}

