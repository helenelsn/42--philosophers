/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 15:50:43 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/25 16:30:57 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

bool	allocate_data_memory(t_data *data, int data_type)
{
	if (data_type == sem_name)
	{
		data->names_create_check = ft_calloc(sizeof(char *),
				data->philos_count);
		if (!data->names_create_check)
			return (false);
		data->names_create = ft_calloc(sizeof(char *), data->philos_count);
		if (!data->names_create)
			return (free(data->sem_create_check), false);
	}
	else
	{
		data->sem_create_check = ft_calloc(sizeof(sem_t *), data->philos_count);
		if (!data->sem_create_check)
			return (false);
		data->sem_create = ft_calloc(sizeof(sem_t *), data->philos_count);
		if (!data->sem_create)
			return (free(data->sem_create_check), false);
	}
	return (true);
}

void	init_close(t_data *data)
{
	if (data->sem_ate_enough != SEM_FAILED)
		sem_close(data->sem_ate_enough);
	if (data->sem_forks != SEM_FAILED)
		sem_close(data->sem_forks);
	if (data->sem_state_msg != SEM_FAILED)
		sem_close(data->sem_state_msg);
}
