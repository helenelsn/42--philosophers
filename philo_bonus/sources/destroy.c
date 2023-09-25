/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:42:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/25 16:35:34 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	join_main_threads(t_data *data, int args_nb)
{
	if (args_nb == 5)
	{
		if (pthread_join(data->check_meals, NULL))
			write(STDERR_FILENO, "pthread_join() failed\n", 22);
	}
}

void	close_semaphores(t_philo *philo)
{
	int	i;

	sem_close(philo->data->sem_ate_enough);
	sem_close(philo->data->sem_forks);
	sem_close(philo->data->sem_state_msg);
	sem_close(philo->sem_last_meal);
	if (philo->data->sem_end_msg)
		sem_close(philo->data->sem_end_msg);
	i = 0;
	while (i < philo->data->philos_count)
	{
		sem_close(philo->data->sem_create_check[i]);
		if (philo->data->sem_create[i])
			sem_close(philo->data->sem_create[i]);
		i++;
	}
}

void	free_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_count)
	{
		free(data->names_create[i]);
		free(data->names_create_check[i]);
		i++;
	}
	free(data->names_create);
	free(data->names_create_check);
	free(data->sem_create);
	free(data->sem_create_check);
	free(data->pids);
}

void	exit_philo(t_philo *philo)
{
	pthread_join(philo->monitoring_thread, NULL);
	close_semaphores(philo);
	free_data(philo->data);
	exit(0);
}

void	exit_parent(t_philo *philo)
{
	close_semaphores(philo);
	unlink_semaphores(philo->data->philos_count);
	free_data(philo->data);
}
