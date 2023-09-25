/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:31:58 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/25 16:30:38 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	unlink_semaphores(long philos_nb)
{
	int		i;
	char	*name;
	char	*name_check;

	i = 1;
	sem_unlink(SEMA_FORKS);
	sem_unlink(SEMA_STREAM);
	sem_unlink(SEMA_MEALS);
	sem_unlink(SEMA_LAST_MEAL);
	sem_unlink(SEMA_END_MSG);
	while (i <= philos_nb)
	{
		name = ft_itoa(i);
		name_check = ft_itoa(-i);
		sem_unlink(name);
		sem_unlink(name_check);
		free(name);
		free(name_check);
		i++;
	}
}

bool	set_death_sem_names(t_data *data)
{
	int	i;

	if (allocate_data_memory(data, sem_name) == false)
		return (false);
	i = 0;
	while (i < data->philos_count)
	{
		data->names_create[i] = ft_itoa(-(i + 1));
		data->names_create_check[i] = ft_itoa(i + 1);
		if (!data->names_create[i] || !data->names_create_check[i])
		{
			free_data(data);
			init_close(data);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	set_death_sem(t_data *data)
{
	int	i;

	if (allocate_data_memory(data, sem) == false)
		return (false);
	i = 0;
	while (i < data->philos_count)
	{
		data->sem_create[i] = NULL;
		data->sem_create_check[i] = sem_open(data->names_create_check[i],
				SEMA_FLAGS, SEMA_MODES, 1);
		if (!data->sem_create_check[i])
		{
			sem_unlink(data->names_create_check[i]);
			while (--i)
			{
				sem_close(data->sem_create_check[i]);
				sem_unlink(data->names_create_check[i]);
			}
			return (false);
		}
		i++;
	}
	return (true);
}

bool	init_data(t_data *data, char **args)
{
	unlink_semaphores(ft_atoi(*args));
	data->philos_count = ft_atoi(*args);
	data->time_to_die = ft_atoi(args[1]);
	data->time_to_eat = ft_atoi(args[2]);
	data->time_to_sleep = ft_atoi(args[3]);
	data->number_of_times_each_philosopher_must_eat = -1;
	if (args[4])
		data->number_of_times_each_philosopher_must_eat = ft_atoi(args[4]);
	data->stop_sim = false;
	data->sem_end_msg = NULL;
	data->starting_time = 0;
	data->sem_ate_enough = sem_open(SEMA_MEALS, SEMA_FLAGS, SEMA_MODES, 0);
	data->sem_forks = sem_open(SEMA_FORKS, SEMA_FLAGS, SEMA_MODES,
			data->philos_count);
	data->sem_state_msg = sem_open(SEMA_STREAM, SEMA_FLAGS, SEMA_MODES, 1);
	if (data->sem_ate_enough == SEM_FAILED || data->sem_forks == SEM_FAILED
		|| data->sem_state_msg == SEM_FAILED)
		return (init_close(data), false);
	if (set_death_sem_names(data) == false || set_death_sem(data) == false)
		return (false);
	data->pids = (pid_t *)malloc(sizeof(pid_t) * data->philos_count);
	if (!data->pids)
		return (false);
	return (true);
}

bool	init_philo(t_philo *philo, t_data *data)
{
	sem_unlink(SEMA_LAST_MEAL);
	philo->data = data;
	philo->meals_count = 0;
	philo->last_meal_tstamp = 0;
	philo->sem_last_meal = sem_open(SEMA_LAST_MEAL, SEMA_FLAGS, SEMA_MODES, 1);
	if (philo->sem_last_meal == SEM_FAILED)
		return (false);
	return (true);
}
