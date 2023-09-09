/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:05:56 by Helene            #+#    #+#             */
/*   Updated: 2023/09/05 19:09:44 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	is_numeral(char c)
{
	return (c >= '0' && c <= '9');
}

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\n'
		|| c == '\t' || c == '\v'
		|| c == '\f' || c == '\r');
}

int	ft_atoi(const char *nb_str)
{
	int	i;
	int	number;

	i = 0;
	number = 0;
    if (*nb_str && *nb_str == '+')
        i++;
    else if (*nb_str && !is_numeral(*nb_str))
        return (-1);
        
	while (nb_str[i] && is_whitespace(nb_str[i]))
		i++;
	while (nb_str[i] && is_numeral(nb_str[i]))
	{
		number = number * 10 + (nb_str[i] - 48);
		i++;
	}
	return (number);
}