/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:42:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/25 15:39:19 by hlesny           ###   ########.fr       */
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

void	exit_philo(t_philo *philo)
{
	int	i;

	pthread_join(philo->monitoring_thread, NULL);
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
	i = 0;
	while (i < philo->data->philos_count)
	{
		free(philo->data->names_create[i]);
		free(philo->data->names_create_check[i]);
		i++;
	}
	free(philo->data->names_create);
	free(philo->data->names_create_check);
	free(philo->data->sem_create);
	free(philo->data->sem_create_check);
	free(philo->data->pids);
	exit(0);
}

void	exit_parent(t_philo *philo)
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
	i = 0;
	sem_unlink(SEMA_FORKS);
	sem_unlink(SEMA_STREAM);
	sem_unlink(SEMA_MEALS);
	sem_unlink(SEMA_LAST_MEAL);
	sem_unlink(SEMA_END_MSG);
	while (i < philo->data->philos_count)
	{
		sem_unlink(philo->data->names_create[i]);
		sem_unlink(philo->data->names_create_check[i]);
		i++;
	}
	i = 0;
	while (i < philo->data->philos_count)
	{
		free(philo->data->names_create[i]);
		free(philo->data->names_create_check[i]);
		i++;
	}
	free(philo->data->names_create);
	free(philo->data->names_create_check);
	free(philo->data->sem_create);
	free(philo->data->sem_create_check);
	free(philo->data->pids);
}
