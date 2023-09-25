/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:24:15 by Helene            #+#    #+#             */
/*   Updated: 2023/09/25 16:35:53 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	start_process(t_philo *philo)
{
	if (pthread_create(&philo->monitoring_thread, NULL, check_for_death,
			(void *)philo))
		write(STDERR_FILENO, "pthread_create() failed\n", 24);
	if (philo->data->number_of_times_each_philosopher_must_eat == 0)
		sem_post(philo->data->sem_ate_enough);
	while (get_current_time() < philo->data->starting_time)
		usleep(100);
	if (philo->philo_id % 2)
		usleep(philo->data->time_to_eat * 500);
	if (philo->data->philos_count == 1)
		(sem_wait(philo->data->sem_forks), print_state(philo, got_fork),
			usleep(philo->data->time_to_die * 1000), exit_philo(philo));
}

void	philo_process(t_philo *philo, int i)
{
	philo->philo_id = i;
	start_process(philo);
	while (true)
	{
		take_forks(philo);
		if (check_create_state(philo->data, philo->philo_id))
			exit_philo(philo);
		eating_state(philo);
		if (check_create_state(philo->data, philo->philo_id))
			exit_philo(philo);
		drop_forks(philo);
		if (check_create_state(philo->data, philo->philo_id))
			exit_philo(philo);
		sleeping_state(philo);
		if (check_create_state(philo->data, philo->philo_id))
			exit_philo(philo);
		thinking_state(philo);
	}
	exit_philo(philo);
}

bool	create_philos(t_philo *philo)
{
	int	i;

	i = 0;
	set_starting_time(philo);
	while (i < philo->data->philos_count)
	{
		philo->data->pids[i] = fork();
		if (philo->data->pids[i] < 0)
		{
			while (--i)
				waitpid(philo->data->pids[i], NULL, 0);
			free(philo->data->pids);
			return (false);
		}
		if (philo->data->pids[i] == 0)
			philo_process(philo, i);
		i++;
	}
	return (true);
}
