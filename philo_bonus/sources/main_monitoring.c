/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_monitoring.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:23:04 by Helene            #+#    #+#             */
/*   Updated: 2023/09/25 15:41:10 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	*check_meals_routine(void *data_check)
{
	int		i;
	t_data	*data;

	//printf("entering check_meals_routine()\n");
	i = 0;
	data = (t_data *)data_check;
	if (data->time_to_die > data->time_to_eat)
	{
		while (get_current_time() < data->starting_time) // + data->time_to_eat / 2)
			usleep(100);
	}
	else
	{
		while (get_current_time() < data->starting_time) // + data->time_to_die / 2)
			usleep(100);
	}
	while (i < data->philos_count)
		(sem_wait(data->sem_ate_enough), i++);
	end_simulation(data);
	return (NULL);
}

void	parent_process(t_philo *philo)
{
	int	i;

	while (exit_main_process(philo->data) == false)
		usleep(100);
	i = 0;
	while (i < philo->data->philos_count)
	{
		if (waitpid(philo->data->pids[i], NULL, 0) < 0)
			write(STDERR_FILENO, "waitpid() failed\n", 17);
		i++;
	}
}

void	create_threads(t_data *data, int args)
{
	if (args == 5)
	{
		if (pthread_create(&data->check_meals, NULL, check_meals_routine,
				(void *)data))
			write(STDERR_FILENO, "pthread_create() failed\n", 24);
	}
}
