/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 04:33:22 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/25 16:35:11 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

long	ft_strlen(const char *str)
{
	int	i;

	if (!str)
		return (-1);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *src)
{
	char	*dup;
	size_t	index;

	index = 0;
	dup = malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (!dup)
		return (0);
	while (src[index])
	{
		dup[index] = src[index];
		index++;
	}
	dup[index] = '\0';
	return (dup);
}

static char	*ft_filltab(unsigned int nb, int l, int s)
{
	char	*number;

	number = ft_calloc(sizeof(char), l + 1);
	if (!number)
		return (0);
	if (s < 0)
		number[0] = '-';
	while (nb)
	{
		number[--l] = nb % 10 + 48;
		nb /= 10;
	}
	return (number);
}

char	*ft_itoa(int n)
{
	int long	nb;
	int			s;
	int			l;

	s = 1;
	l = 1;
	nb = n;
	if (nb == 0)
		return (ft_strdup("0"));
	if (nb < 0)
	{
		s = -1;
		nb *= -1;
	}
	while (nb > 9)
	{
		nb /= 10;
		l++;
	}
	return (ft_filltab(n * s, l + (s < 0), s));
}
