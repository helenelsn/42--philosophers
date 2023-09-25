/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:37:21 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/25 16:44:31 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_error(int error_type)
{
	if (error_type == negative_philos)
		write(STDERR_FILENO, "This program requires a positive amount of \
philosophers to start the simulation!\n", 95);
	else if (error_type == invalid_amount)
		write(STDERR_FILENO, "This program requires between \
4 and 5 arguments!\n", 49);
	else if (error_type == non_numeric)
		write(STDERR_FILENO, "This program requires numeric and \
non-negative arguments!\n", 58);
	write(STDERR_FILENO, "Example : ./philo 4 400 100 50 3\n", 33);
}

bool	check_inputs_amount(int args_nb)
{
	if (args_nb < 4 || args_nb > 5)
		return (print_error(invalid_amount), false);
	return (true);
}

bool	check_philosophers_count(char *philos_count)
{
	if (ft_atoi(philos_count) == 0)
		return (print_error(negative_philos), false);
	return (true);
}

bool	valid_input(int args_nb, char **inputs)
{
	int	i;
	int	j;

	if (check_inputs_amount(args_nb) == false)
		return (false);
	i = 0;
	while (i < args_nb)
	{
		j = 0;
		if (inputs[i][j] == '+')
			j++;
		while (inputs[i][j])
		{
			if (inputs[i][j] < '0' || inputs[i][j] > '9')
				return (print_error(non_numeric), false);
			j++;
		}
		i++;
	}
	if (check_philosophers_count(*inputs) == false)
		return (false);
	return (true);
}
