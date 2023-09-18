/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_while_waiting.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 23:21:59 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/18 21:26:21 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	*check_for_death(void *arg)
{
	int				i;
	unsigned long	last_meal;
	t_philo			*philo_monitor;

	i = 0;
	philo_monitor = (t_philo *)arg;
	while (true)
	{
		sem_wait(philo_monitor->sem_last_meal);
		last_meal = philo_monitor->last_meal_tstamp;
		sem_post(philo_monitor->sem_last_meal);
		/* if (sem_open(SEMA_END, 0) != SEM_FAILED)
			return (NULL); */
		if (check_create_state(philo_monitor->data, philo_monitor->philo_id))
			return (NULL);
		if (get_current_time(philo_monitor) >= last_meal
			+ philo_monitor->time_to_die)
		{
			/* end_simulation */
			end_simulation(philo_monitor->data);
			
			/* sem_wait(philo_monitor->data->sem_create_check);
			philo_monitor->data->sem_end = sem_open(ft_itoa(philo_monitor->philo_id % SEMA_PROTECT_COUNT), SEMA_FLAGS,
					SEMA_MODES, 0);
			sem_post(philo_monitor->data->sem_create_check); */
			
			sem_wait(philo_monitor->data->sem_state_msg);
			if (sem_open(SEMA_END_MSG, 0) == SEM_FAILED && errno == ENOENT)
			{
				philo_monitor->data->sem_end_msg = sem_open(SEMA_END_MSG,
						SEMA_FLAGS, SEMA_MODES, 0);
				printf("%ld %d died\n", get_current_time(philo_monitor),
					philo_monitor->philo_id + 1);
			}
			sem_post(philo_monitor->data->sem_state_msg);
			sem_post(philo_monitor->data->sem_forks);
			return (NULL);
		}
		usleep(50);
	}
	return (NULL);
}
