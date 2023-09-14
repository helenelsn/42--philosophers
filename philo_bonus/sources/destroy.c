/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:42:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/14 17:32:25 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    close_semaphores(t_philo *philo, t_data *data)
{
    sem_close(data->sem_ate_enough);
    sem_close(data->sem_forks);
    sem_close(data->sem_one_died);
    sem_close(data->sem_state_msg);
    sem_close(data->sem_monitor);
    sem_close(philo->sem_last_meal);
    if (data->sem_end)
        sem_close(data->sem_end);
}

void    exit_philo(t_philo *philo, t_data *data, pthread_t *philo_monitor)
{
    pthread_join(*philo_monitor, NULL);
    //printf("%ld -- philo %d : entered exit_philo \n", get_current_time(philo), philo->philo_id + 1);
    close_semaphores(philo, data);
    unlink_semaphores();
    printf("%ld ------ philo %d : joined monitoring thread, about to exit \n", get_current_time(philo), philo->philo_id + 1);
    exit(1); 
}

void 	join_main_threads(t_data *data, int args_nb)
{
	// if (pthread_join(data.check_death, NULL))
    //     write(STDERR_FILENO, "pthread_join() failed\n", 22);
    if (args_nb == 5)
    {
        if (pthread_join(data->check_meals, NULL))
            write(STDERR_FILENO, "pthread_join() failed\n", 22);
    }
}

void    exit_parent(t_philo *philo, t_data *data)
{
    close_semaphores(philo, data);
    unlink_semaphores();
    free(data->pids);
    data->pids = NULL;
    
}