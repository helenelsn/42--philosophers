/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_monitoring.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:23:04 by Helene            #+#    #+#             */
/*   Updated: 2023/09/12 01:47:21 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

bool    end_processes(t_data *data)
{
    bool    end_all;
    
    end_all = false;
    sem_wait(data->sem_one_died);
    end_all = data->stop_sim;
    sem_post(data->sem_one_died);
    return (end_all);
}

void    parent_process(t_philo *philo, t_data *data)
{
    while (end_processes(data) == false)
        usleep(50);
    printf("lolilol i m a killer");
    kill_processes(data);
    /* free memory, unlink semaphores, join threads */
    exit_parent(philo, data);
}

void    *check_death_routine(void *data_check)
{
    int     i;
    t_data *data;

    i = 0;
    data = (t_data *)data_check;
    sem_wait(data->sem_one_died);
    while (i < data->philos_count)
    {
        sem_post(data->sem_ate_enough);
        i++;
    }
    return (NULL);
}

void    *check_meals_routine(void *data_check)
{
    int     i;
    t_data *data;

    i = 0;
    data = (t_data *)data_check;
    while (i < data->philos_count)
    {
        sem_wait(data->sem_ate_enough);
        printf("incremented sem_ate_enough\n");
        i++;
    }
    sem_post(data->sem_one_died); // à vérifier
    return (NULL);
}
