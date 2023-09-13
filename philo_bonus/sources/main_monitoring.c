/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_monitoring.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:23:04 by Helene            #+#    #+#             */
/*   Updated: 2023/09/13 02:02:58 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    *check_death_routine(void *data_check)
{
    int     i;
    t_data *data;

    i = 0;
    data = (t_data *)data_check;
    sem_wait(data->sem_one_died);
    /*while (i < data->philos_count)
    {
        sem_post(data->sem_ate_enough);
        i++;
    }*/
    sem_wait(data->sem_monitor);
    data->stop_sim = true;
    sem_post(data->sem_monitor);
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
    sem_post(data->sem_one_died); // obligé car sinon, s'ils ont tous mangé, le thread check_death ne se finit jamais
    
    sem_wait(data->sem_monitor);
    data->stop_sim = true;
    sem_post(data->sem_monitor);
    return (NULL);
}

void    kill_processes(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philos_count)
    {
        kill(data->pids[i], SIGKILL);
        i++;
    }
}

bool    end_processes(t_data *data)
{
    bool    end_all;
    
    end_all = false;
    sem_wait(data->sem_monitor);
    end_all = data->stop_sim;
    //printf("end_processes = %d\n", end_all);
    sem_post(data->sem_monitor);
    return (end_all);
}

void    parent_process(t_philo *philo, t_data *data)
{
    //printf("entered parent_process()\n");
    while (end_processes(data) == false)
        usleep(500);
    kill_processes(data);
    //printf("lolilol i m a killer\n");
    
    /* Wait for children processes */
    int i;
    i = 0;
    while (i < data->philos_count)
    {
        //printf("waiting for process %d\n", i);
        if (waitpid(data->pids[i], NULL, 0) < 0)
            write(STDERR_FILENO, "waitpid() failed\n", 17);
        //printf("process %d done\n", i);
        i++;
    }
    //dprintf(2, "apres les gosses\n");
}

void    create_threads(t_data *data, int args)
{
    //printf("creating threads\n");
    if (pthread_create(&data->check_death, NULL, check_death_routine, (void *)data))
        write(STDERR_FILENO, "pthread_create() failed\n", 24);
    if (args == 5)
    {
        if (pthread_create(&data->check_meals, NULL, check_meals_routine, (void *)data))
            write(STDERR_FILENO, "pthread_create() failed\n", 24);
        /*if (pthread_join(data->check_meals, NULL))
            write(STDERR_FILENO, "pthread_join() failed\n", 22);*/
    }
    /*if (pthread_join(data->check_death, NULL))
        write(STDERR_FILENO, "pthread_join() failed\n", 22);*/
}
