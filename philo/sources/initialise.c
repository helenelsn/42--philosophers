/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:01:17 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/14 21:31:54 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	init_data_mutexes(t_data *data)
{
	int	i;
	int	err;

	i = 0;
	err = 0;
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->philos_count);
	if (!data->forks)
		return ;
	while (i < data->philos_count)
	{
		err = pthread_mutex_init(&data->forks[i], NULL);
		if (err)
			write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
		i++;
	}
	err = pthread_mutex_init(&data->msg_display, NULL);
	if (err)
		write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
	err = pthread_mutex_init(&data->end_simulation_m, NULL);
	if (err)
		write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
}

void	init_data(char **program_args, t_data *data)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	data->starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	data->number_of_times_each_philosopher_must_eat = -1;
	data->philos_count = ft_atoi(program_args[0]);
	data->time_to_die = ft_atoi(program_args[1]);
	data->time_to_eat = ft_atoi(program_args[2]);
	data->time_to_sleep = ft_atoi(program_args[3]);
	data->end_simulation = false;
	if (program_args[4])
		data->number_of_times_each_philosopher_must_eat
			= ft_atoi(program_args[4]);
	init_data_mutexes(data);
}

void	init_philo(t_philo *philo, t_data *data, int i)
{
	philo->meals_count = 0;
	philo->data = data;
	philo->philo_id = i;
	philo->tid = 0;
	philo->last_meal_tstamp = 0;
}

void	create_threads(t_philo *philos, t_data data)
{
	int	i;
	int	err;

	i = 0;
	err = 0;
	while (i < data.philos_count)
	{
		init_philo(&philos[i], &data, i);
		if (pthread_mutex_init(&philos[i].meals_count_m, NULL))
			write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
		if (pthread_mutex_init(&philos[i].last_meal_m, NULL))
			write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
		if (data.philos_count == 1)
			err = pthread_create(&philos[i].tid, NULL, philo_routine_one,
					(void *)&philos[i]);
		else
			err = pthread_create(&philos[i].tid, NULL, philo_routine,
					(void *)&philos[i]);
		if (err)
			write(STDERR_FILENO, "pthread_create() failed\n", 24);
		i++;
	}
}
