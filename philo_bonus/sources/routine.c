/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:24:15 by Helene            #+#    #+#             */
/*   Updated: 2023/09/25 12:12:54 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    sleeping_state(t_philo *philo, t_data *data, pthread_t *philo_monitor)
{
    print_state(philo, data, sleeping);
    ft_usleep(philo, data, sleeping, philo_monitor);
}

void    thinking_state(t_philo *philo, t_data *data, pthread_t *philo_monitor)
{
    usleep(500);
    print_state(philo, data, thinking);
    ft_usleep(philo, data, thinking, philo_monitor);
}

/* void    self_monitoring(t_philo *philo, t_data *data, pthread_t *philo_monitor)
{
    unsigned long   last_meal;
    
    sem_wait(philo->sem_last_meal);
    last_meal = philo->last_meal_tstamp;
    sem_post(philo->sem_last_meal);
    
    if (get_relative_time(philo) >= last_meal + philo->time_to_die)
    {
        sem_post(data->sem_one_died);
        sem_wait(data->sem_state_msg); // ne sem_post pas apres comme ca plus personne d'autre ne peut écrire
        printf("%ld %d died\n", get_relative_time(philo), philo->philo_id + 1);
        exit_philo(philo, data, philo_monitor); 
    }
} */

void    drop_forks(t_philo *philo, t_data *data)
{
    sem_post(data->sem_forks);
    sem_post(data->sem_forks);
}

void    take_forks(t_philo *philo, t_data *data, pthread_t *philo_monitor)
{
    sem_wait(data->sem_forks);
    if (check_create_state(data, philo->philo_id))
    {
        //printf("philo %d died while waiting fot a fork, about to exit\n", philo->philo_id + 1);
        sem_post(data->sem_forks);
        exit_philo(philo, data, philo_monitor);
    }
    print_state(philo, data, got_fork);
    
    sem_wait(data->sem_forks);
    if (check_create_state(data, philo->philo_id))
    {
        //printf("philo %d died while waiting fot a fork, about to exit\n", philo->philo_id + 1);
        drop_forks(philo, data);
        exit_philo(philo, data, philo_monitor);
    }
    print_state(philo, data, got_fork);
}

void    eating_state(t_philo *philo, t_data *data, pthread_t *philo_monitor)
{
    /* update last meal timestamp */
    sem_wait(philo->sem_last_meal);
    philo->last_meal_tstamp = get_relative_time(philo);
    sem_post(philo->sem_last_meal);

    print_state(philo, data, eating);
    //usleep(philo->time_to_eat * 1000);
    ft_usleep(philo, data, eating, philo_monitor);
    
    if (philo->data->number_of_times_each_philosopher_must_eat != -1 
        && ++(philo->meals_count) == philo->data->number_of_times_each_philosopher_must_eat)
            sem_post(data->sem_ate_enough);
}

void    philo_process(t_philo *philo, t_data *data, int i)
{
    pthread_t   monitoring_thread;
    
    philo->philo_id = i;
    if (pthread_create(&monitoring_thread, NULL, check_for_death, (void *)philo))
        write(STDERR_FILENO, "pthread_create() failed\n", 24);
    // if (pthread_detach(monitoring_thread))
    //     write(STDERR_FILENO, "pthread_detach() failed\n", 24);
   
    if (philo->data->number_of_times_each_philosopher_must_eat == 0)
         sem_post(data->sem_ate_enough);

    //modif
    while (get_current_time() < philo->data->starting_time)
        usleep(100);
    
    // printf("coucou\n");
     if (data->philos_count == 1)
         (sem_wait(data->sem_forks), print_state(philo, data, got_fork),
             usleep(philo->data->time_to_die * 1000), 
                 exit_philo(philo, data, &monitoring_thread));

    while (true)
    {
        take_forks(philo, data, &monitoring_thread);
        if (check_create_state(data, philo->philo_id))
            exit_philo(philo, data, &monitoring_thread);
        //self_monitoring(philo, data);
        
        eating_state(philo, data, &monitoring_thread);
        if (check_create_state(data, philo->philo_id))
            exit_philo(philo, data, &monitoring_thread);
        //self_monitoring(philo, data);
        drop_forks(philo, data);

        //printf("%d ok ici\n", philo->philo_id + 1);
        //self_monitoring(philo, data);
        if (check_create_state(data, philo->philo_id))
            exit_philo(philo, data, &monitoring_thread);
        sleeping_state(philo, data, &monitoring_thread);

        //self_monitoring(philo, data);
        if (check_create_state(data, philo->philo_id))
            exit_philo(philo, data, &monitoring_thread);
        thinking_state(philo, data, &monitoring_thread);
    }
    exit_philo(philo, data, &monitoring_thread);
}

/* Create child processes, ie philosophers */
bool    create_philos(t_data *data, t_philo *philo)
{
    int	i;

	i = 0;
	set_starting_time(philo); 
    while (i < data->philos_count)
    {
        data->pids[i] = fork();
        if (data->pids[i] < 0)
        {
            while (--i)
                waitpid(data->pids[i], NULL, 0);
            free(data->pids);
            return (false);
        }
        if (data->pids[i] == 0)
        {
            free(data->pids);
            data->pids = NULL;
            if (i % 2)
                usleep(philo->data->time_to_eat * 500);
            philo_process(philo, data, i);
        }
        i++;
    }
    return (true);
}
