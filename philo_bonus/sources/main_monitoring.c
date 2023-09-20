/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_monitoring.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:23:04 by Helene            #+#    #+#             */
/*   Updated: 2023/09/20 16:01:31 by Helene           ###   ########.fr       */
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
	while (i < data->philos_count)
	{
		sem_wait(data->sem_ate_enough);
		//printf("-----waited %d for sem_ate_enough in check_meals_routine \n", i);
		i++;
	}
	//printf("in check_meals_routine(), done waiting for sem_ate_enough posts\n");
	//sem_open(SEMA_END, SEMA_FLAGS, SEMA_MODES, 0);
	end_simulation(data);
	//usleep(500);
	return (NULL);
}

void	parent_process(t_philo *philo, t_data *data)
{
	int	i;

	while (exit_main_process(data) == false)
		usleep(100);
	/* while (sem_open(SEMA_END, 0) == SEM_FAILED)
		usleep(50); */
	i = 0;
	while (i < data->philos_count)
	{
		if (waitpid(data->pids[i], NULL, 0) < 0)
			write(STDERR_FILENO, "waitpid() failed\n", 17);
		//printf("waited for child %d\n", i);
		i++;
	}
	//printf("no more children\n");
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
