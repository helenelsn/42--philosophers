/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:42:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/23 16:01:37 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

// void	close_semaphores(t_philo *philo, t_data *data)
// {
// 	int i;

// 	i = 0;
// 	sem_close(data->sem_ate_enough);
// 	sem_close(data->sem_forks);
// 	sem_close(data->sem_state_msg);
// 	sem_close(philo->sem_last_meal);
// 	while (i < data->philos_count)
// 	{
// 		sem_close(data->sem_create_check[i]); //rajouté 
// 		if (data->sem_create[i])
// 			sem_close(data->sem_create[i]);
// 		i++;
// 	}
// 	/* if (data->sem_end)
// 		sem_close(data->sem_end); */
// 	if (data->sem_end_msg)
// 		sem_close(data->sem_end_msg);
// }

// void	exit_philo(t_philo *philo, t_data *data, pthread_t *philo_monitor)
// {
// 	pthread_join(*philo_monitor, NULL);
	
// 	close_semaphores(philo, data);
// 	free(data->sem_create);
// 	free(data->sem_create_check);
// 	//printf("incremented sem_ate_enough\n");
// 	//printf("philo %d in exit_philo()\n", philo->philo_id + 1);
// 	exit(1);
// }

// void	exit_parent(t_philo *philo, t_data *data)
// {
// 	close_semaphores(philo, data);
	
// 		int i = 0;
// 		while (i < data->philos_count)
// 		{
// 			free(data->names_create[i]);
// 			free(data->names_create_check[i]);
// 			i++;
// 		}
	
// 		free(data->names_create);
// 		free(data->names_create_check);
// 		free(data->sem_create);
// 		free(data->sem_create_check);
// 	unlink_semaphores(data->philos_count);
// 	free(data->pids);
// 	data->pids = NULL;
// }

void	join_main_threads(t_data *data, int args_nb)
{
	if (args_nb == 5)
	{
		if (pthread_join(data->check_meals, NULL))
			write(STDERR_FILENO, "pthread_join() failed\n", 22);
	}
}

void	exit_philo(t_philo *philo, t_data *data, pthread_t *philo_monitor)
{
	int i;
	
	pthread_join(*philo_monitor, NULL);
	
	sem_close(data->sem_ate_enough);
	sem_close(data->sem_forks);
	sem_close(data->sem_state_msg);
	sem_close(philo->sem_last_meal);
	if (data->sem_end_msg)
		sem_close(data->sem_end_msg);

	i = 0;
	while (i < data->philos_count)
	{
		sem_close(data->sem_create_check[i]);
		if (data->sem_create[i])
			sem_close(data->sem_create[i]);
		i++;
	}

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

	exit(0);
	
}

void	exit_parent(t_philo *philo, t_data *data)
{
	int i;
		
	sem_close(data->sem_ate_enough);
	sem_close(data->sem_forks);
	sem_close(data->sem_state_msg);
	sem_close(philo->sem_last_meal);
	if (data->sem_end_msg)
		sem_close(data->sem_end_msg);

	i = 0;
	while (i < data->philos_count)
	{
		sem_close(data->sem_create_check[i]);
		if (data->sem_create[i])
			(sem_close(data->sem_create[i]), sem_close(data->sem_create[i]));
		i++;
	}

	i = 0;
	sem_unlink(SEMA_FORKS);
	sem_unlink(SEMA_STREAM);
	sem_unlink(SEMA_MEALS);
	sem_unlink(SEMA_LAST_MEAL);
	sem_unlink(SEMA_END_MSG);
	while (i < data->philos_count)
	{
		sem_unlink(data->names_create[i]);
		sem_unlink(data->names_create_check[i]);
		i++;
	}

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
