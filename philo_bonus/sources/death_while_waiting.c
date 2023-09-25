/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_while_waiting.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 23:21:59 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/25 12:12:35 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	*check_for_death(void *arg)
{
	int				i;
	unsigned long	last_meal;
	t_philo			*philo;

	i = 0;
	philo = (t_philo *)arg;
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
			/* end_simulation */
			end_simulation(philo->data);
			
			/* sem_wait(philo->data->sem_create_check);
			philo->data->sem_end = sem_open(ft_itoa(philo->philo_id % SEMA_PROTECT_COUNT), SEMA_FLAGS,
					SEMA_MODES, 0);
			sem_post(philo->data->sem_create_check); */

			for (int i = 0; i < philo->data->philos_count; i++)
				sem_post(philo->data->sem_ate_enough);
			
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
			//printf("philo %d about to exit the end_simulation \'if\'\n", philo->philo_id + 1);
			return (NULL);
		}
		usleep(50);
	}
	return (NULL);
}
