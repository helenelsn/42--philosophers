/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_death_protection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:16:13 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/25 16:40:54 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

bool	check_create_state(t_data *data, int n)
{
	sem_t	*temp;
	bool	state;

	state = true;
	sem_wait(data->sem_create_check[n]);
	temp = sem_open(data->names_create[n], 0);
	if (temp == SEM_FAILED)
		state = false;
	else
		sem_close(temp);
	sem_post(data->sem_create_check[n]);
	return (state);
}

void	end_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_count)
	{
		sem_wait(data->sem_create_check[i]);
		i++;
	}
	i = 0;
	while (i < data->philos_count)
	{
		data->sem_create[i] = sem_open(data->names_create[i], SEMA_FLAGS,
				SEMA_MODES, 0);
		i++;
	}
	i = 0;
	while (i < data->philos_count)
	{
		sem_post(data->sem_create_check[i]);
		i++;
	}
}

bool	exit_main_process(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_count)
	{
		if (check_create_state(data, i) == false)
			return (false);
		i++;
	}
	return (true);
}
