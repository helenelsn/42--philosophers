/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:05:56 by Helene            #+#    #+#             */
/*   Updated: 2023/09/14 23:48:47 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	is_numeral(char c)
{
	return (c >= '0' && c <= '9');
}

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f'
		|| c == '\r');
}

long	ft_atoi(const char *nb_str)
{
	long		i;
	long long	number;

	i = 0;
	number = 0;
	if (*nb_str && *nb_str == '+')
		i++;
	else if (*nb_str && !is_numeral(*nb_str))
		return (-1);
	while (nb_str[i] && is_whitespace(nb_str[i]))
		i++;
	while (nb_str[i] && is_numeral(nb_str[i]) && number <= __LONG_MAX__)
	{
		number = number * 10 + (nb_str[i] - 48);
		i++;
	}
	return (number);
}
