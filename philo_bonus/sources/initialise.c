/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:31:58 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/18 21:19:57 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	unlink_semaphores(long philos_nb)
{
	int 	i;

	i = 1;
	sem_unlink(SEMA_FORKS);
	sem_unlink(SEMA_STREAM);
	sem_unlink(SEMA_MEALS);
	sem_unlink(SEMA_LAST_MEAL);
	sem_unlink(SEMA_END);
	sem_unlink(SEMA_END_MSG);
	sem_unlink(SEMA_DEATH_CHECK);
	while (i <= philos_nb)
	{
		sem_unlink(ft_itoa(i));
		sem_unlink(ft_itoa(-i));
		i++;
	}
}

void	set_starting_time(t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	philo->starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

bool	init_data(t_data *data, char *count)
{
	int 	i;

	i = 0;
	unlink_semaphores(ft_atoi(count));
	data->stop_sim = false;
	data->philos_count = ft_atoi(count);
	data->sem_end = NULL;
	data->sem_end_msg = NULL;
	data->sem_ate_enough = sem_open(SEMA_MEALS, SEMA_FLAGS, SEMA_MODES, 0);
	data->sem_forks = sem_open(SEMA_FORKS, SEMA_FLAGS, SEMA_MODES,
			data->philos_count);
	data->sem_state_msg = sem_open(SEMA_STREAM, SEMA_FLAGS, SEMA_MODES, 1);

	data->sem_create_check = ft_calloc(sizeof(sem_t *), data->philos_count);
	if (!data->sem_create_check)
		return (false);
	data->sem_create = ft_calloc(sizeof(sem_t *), data->philos_count);
	if (!data->sem_create)
		return (free(data->sem_create_check), false);
	while (i < data->philos_count)
	{
		data->sem_create_check[i] = sem_open(ft_itoa(i + 1), SEMA_FLAGS, SEMA_MODES, 1);
		if (!data->sem_create_check[i])
		{
			sem_unlink(ft_itoa(i + 1));
			while (--i)
			{
				sem_close(data->sem_create_check[i]);
				sem_unlink(ft_itoa(i + 1));

			}
		}
		i++;
	}
	i = 0;
	while (i < data->philos_count)
	{
		data->sem_create[i] = NULL;
		i++;
	}
	
	if (data->sem_forks == SEM_FAILED || data->sem_ate_enough == SEM_FAILED
		|| data->sem_state_msg == SEM_FAILED)
		return (false);
	return (true);
}

bool	init_philo(t_philo *philo, t_data *data, char **args)
{
	sem_unlink(SEMA_LAST_MEAL);
	philo->time_to_die = ft_atoi(args[0]);
	philo->time_to_eat = ft_atoi(args[1]);
	philo->time_to_sleep = ft_atoi(args[2]);
	philo->meals_count = 0;
	philo->starting_time = 0;
	philo->last_meal_tstamp = 0;
	philo->data = data;
	philo->number_of_times_each_philosopher_must_eat = -1;
	if (args[3])
		philo->number_of_times_each_philosopher_must_eat = ft_atoi(args[3]);
	philo->sem_last_meal = sem_open(SEMA_LAST_MEAL, SEMA_FLAGS, SEMA_MODES, 1);
	if (philo->sem_last_meal == SEM_FAILED)
		return (false);
	return (true);
}
