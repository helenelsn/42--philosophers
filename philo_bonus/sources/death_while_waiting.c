/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_while_waiting.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 23:21:59 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/25 16:38:07 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	wait_for_starting_time(t_data *data)
{
	while (get_current_time() < data->starting_time)
		usleep(100);
}

void	set_end(t_philo *philo)
{
	int	i;

	i = 0;
	end_simulation(philo->data);
	while (i < philo->data->philos_count)
		(sem_post(philo->data->sem_ate_enough), i++);
	sem_wait(philo->data->sem_state_msg);
	if (sem_open(SEMA_END_MSG, 0) == SEM_FAILED && errno == ENOENT)
	{
		philo->data->sem_end_msg = sem_open(SEMA_END_MSG,
				SEMA_FLAGS, SEMA_MODES, 0);
		printf("%lld %d died\n", get_relative_time(philo), philo->philo_id + 1);
	}
	sem_post(philo->data->sem_state_msg);
	sem_post(philo->data->sem_forks);
}

void	*check_for_death(void *arg)
{
	unsigned long	last_meal;
	t_philo			*philo;

	philo = (t_philo *)arg;
	wait_for_starting_time(philo->data);
	while (true)
	{
		sem_wait(philo->sem_last_meal);
		last_meal = philo->last_meal_tstamp;
		sem_post(philo->sem_last_meal);
		if (check_create_state(philo->data, philo->philo_id))
			return (NULL);
		if (get_relative_time(philo) >= last_meal + philo->data->time_to_die)
			return (set_end(philo), NULL);
		usleep(50);
	}
	return (NULL);
}
