/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:31:58 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/25 11:57:28 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	unlink_semaphores(long philos_nb)
{
	int 	i;
	char 	*name;
	char	*name_check;

	i = 1;
	sem_unlink(SEMA_FORKS);
	sem_unlink(SEMA_STREAM);
	sem_unlink(SEMA_MEALS);
	sem_unlink(SEMA_LAST_MEAL);
	sem_unlink(SEMA_END); //todel 
	sem_unlink(SEMA_END_MSG);
	sem_unlink(SEMA_DEATH_CHECK); //todel
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

void	set_starting_time(t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	philo->data->starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000 + 10 * philo->data->philos_count; //modif
}

bool	init_data(t_data *data, char *count)
{
	int 	i;

	i = 0;
	unlink_semaphores(ft_atoi(count));
	data->stop_sim = false;
	data->philos_count = ft_atoi(count);
	data->sem_end = NULL; //todel 
	data->sem_end_msg = NULL;
	data->starting_time = 0;
	data->sem_ate_enough = sem_open(SEMA_MEALS, SEMA_FLAGS, SEMA_MODES, 0);
	data->sem_forks = sem_open(SEMA_FORKS, SEMA_FLAGS, SEMA_MODES,
			data->philos_count);
	data->sem_state_msg = sem_open(SEMA_STREAM, SEMA_FLAGS, SEMA_MODES, 1);
	if (data->sem_forks == SEM_FAILED || data->sem_ate_enough == SEM_FAILED
		|| data->sem_state_msg == SEM_FAILED)
		return (false);
	
	
	data->names_create_check = ft_calloc(sizeof(char *), data->philos_count);
	if (!data->names_create_check)
		return (false);
	data->names_create = ft_calloc(sizeof(char *), data->philos_count);
	if (!data->names_create)
		return (free(data->sem_create_check), false);
	i = 0;
	while (i < data->philos_count)
	{
		data->names_create[i] = ft_itoa(-(i + 1));
		data->names_create_check[i] = ft_itoa(i + 1);
		if (!data->names_create[i] || !data->names_create_check[i])
		{
			if (!data->names_create[i])
				free(data->names_create[i]);
			if (!data->names_create_check[i])
				free(data->names_create_check[i]);
			while (--i)
				(free(data->names_create[i]), free(data->names_create_check[i]));
			//close_semaphores();
			return (false);
		}
		i++;
	}

	data->sem_create_check = ft_calloc(sizeof(sem_t *), data->philos_count);
	if (!data->sem_create_check)
		return (false);
	data->sem_create = ft_calloc(sizeof(sem_t *), data->philos_count);
	if (!data->sem_create)
		return (free(data->sem_create_check), false);
	i = 0;
	while (i < data->philos_count)
	{
		data->sem_create_check[i] = sem_open(data->names_create_check[i], SEMA_FLAGS, SEMA_MODES, 1);
		if (!data->sem_create_check[i])
		{
			sem_unlink(data->names_create_check[i]);
			while (--i)
			{
				sem_close(data->sem_create_check[i]);
				sem_unlink(data->names_create_check[i]);
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
	
	
	return (true);
}

bool	init_philo(t_philo *philo, t_data *data, char **args) // modif les philo->data->...  ->  mettre directement dans init_data()
{
	sem_unlink(SEMA_LAST_MEAL);
	philo->data = data;
	philo->data->time_to_die = ft_atoi(args[0]);
	philo->data->time_to_eat = ft_atoi(args[1]);
	philo->data->time_to_sleep = ft_atoi(args[2]);
	philo->meals_count = 0;
	// philo->data->starting_time = 0;
	philo->last_meal_tstamp = 0;
	philo->data->number_of_times_each_philosopher_must_eat = -1;
	if (args[3])
		philo->data->number_of_times_each_philosopher_must_eat = ft_atoi(args[3]);
	philo->sem_last_meal = sem_open(SEMA_LAST_MEAL, SEMA_FLAGS, SEMA_MODES, 1);
	if (philo->sem_last_meal == SEM_FAILED)
		return (false);
	return (true);
}
