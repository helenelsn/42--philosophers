/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_while_waiting.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 23:21:59 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/25 13:03:10 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void 	wait_for_starting_time(t_philo *philo)
{
	int	i;

	i = 0;
	if (philo->data->time_to_die > philo->data->time_to_eat)
	{
		while (get_current_time() < philo->data->starting_time) //+ philo->data->time_to_eat)
			usleep(100);
	}
	else
	{
		while (get_current_time() < philo->data->starting_time) //+ philo->data->time_to_die)
			usleep(100);
	}
}

void 	set_end(t_philo *philo)
{
	int 	i;

	i = 0;
	end_simulation(philo->data);

	while (i < philo->data->philos_count)
		(sem_post(philo->data->sem_ate_enough), i++);
	
	sem_wait(philo->data->sem_state_msg);
	if (sem_open(SEMA_END_MSG, 0) == SEM_FAILED && errno == ENOENT)
	{
		philo->data->sem_end_msg = sem_open(SEMA_END_MSG,
				SEMA_FLAGS, SEMA_MODES, 0);
		printf("%lld %d died\n", get_relative_time(philo),
			philo->philo_id + 1);
	}
	sem_post(philo->data->sem_state_msg);
	sem_post(philo->data->sem_forks);
}

void	*check_for_death(void *arg)
{
	unsigned long	last_meal;
	t_philo			*philo;

	philo = (t_philo *)arg;
	wait_for_starting_time(philo);
	while (true)
	{
		sem_wait(philo->sem_last_meal);
		last_meal = philo->last_meal_tstamp;
		sem_post(philo->sem_last_meal);
		/* if (sem_open(SEMA_END, 0) != SEM_FAILED)
			return (NULL); */
		if (check_create_state(philo->data, philo->philo_id))
			return (NULL);
		if (get_relative_time(philo) >= last_meal
			+ philo->data->time_to_die)
		{
			//printf("philo %d is in end_simulation()\n", philo->philo_id + 1);
			//printf("philo %d about to exit the end_simulation \'if\'\n", philo->philo_id + 1);
			
			return (set_end(philo), NULL);
		}
		usleep(50);
	}
	return (NULL);
}
