/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:42:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/14 21:55:28 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	close_semaphores(t_philo *philo, t_data *data)
{
	sem_close(data->sem_ate_enough);
	sem_close(data->sem_forks);
	sem_close(data->sem_one_died);
	sem_close(data->sem_state_msg);
	sem_close(philo->sem_last_meal);
	if (data->sem_end)
		sem_close(data->sem_end);
	if (data->sem_end_msg)
		sem_close(data->sem_end_msg);
}

void	exit_philo(t_philo *philo, t_data *data, pthread_t *philo_monitor)
{
	pthread_join(*philo_monitor, NULL);
	close_semaphores(philo, data);
	exit(1);
}

void	join_main_threads(t_data *data, int args_nb)
{
	if (args_nb == 5)
	{
		if (pthread_join(data->check_meals, NULL))
			write(STDERR_FILENO, "pthread_join() failed\n", 22);
	}
}

void	exit_parent(t_philo *philo, t_data *data)
{
	close_semaphores(philo, data);
	unlink_semaphores();
	free(data->pids);
	data->pids = NULL;
}
