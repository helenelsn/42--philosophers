/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:24:15 by Helene            #+#    #+#             */
/*   Updated: 2023/09/14 02:00:50 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    sleeping_state(t_philo *philo, t_data *data)
{
    print_state(philo, data, sleeping);
    usleep(philo->time_to_sleep * 1000);
    //ft_usleep(philo, data, sleeping);
}

void    thinking_state(t_philo *philo, t_data *data)
{
    usleep(500);
    print_state(philo, data, thinking);
    usleep((philo->time_to_die - philo->time_to_eat - philo->time_to_sleep) * 500);
    //ft_usleep(philo, data, thinking);
    /* while (data->sem_forks->__align < 2) //check le retour du monitoring en meme temps ?
    {
        self_monitoring(philo, data);
        usleep(10);
    } */
}

/* void    self_monitoring(t_philo *philo, t_data *data, pthread_t *philo_monitor)
{
    unsigned long   last_meal;
    
    sem_wait(philo->sem_last_meal);
    last_meal = philo->last_meal_tstamp;
    sem_post(philo->sem_last_meal);
    
    if (get_current_time(philo) >= last_meal + philo->time_to_die)
    {
        sem_post(data->sem_one_died);
        sem_wait(data->sem_state_msg); // ne sem_post pas apres comme ca plus personne d'autre ne peut écrire
        printf("%ld %d died\n", get_current_time(philo), philo->philo_id + 1);
        exit_philo(philo, data, philo_monitor); 
    }
} */

void    take_forks(t_philo *philo, t_data *data)
{
    sem_wait(data->sem_forks);
    print_state(philo, data, got_fork);
    sem_wait(data->sem_forks);
    print_state(philo, data, got_fork);
}

void    drop_forks(t_philo *philo, t_data *data)
{
    sem_post(data->sem_forks);
    sem_post(data->sem_forks);
}

void    eating_state(t_philo *philo, t_data *data)
{
    /* update last meal timestamp */
    sem_wait(philo->sem_last_meal);
    philo->last_meal_tstamp = get_current_time(philo);
    sem_post(philo->sem_last_meal);

    print_state(philo, data, eating);
    usleep(philo->time_to_eat * 1000);
    //ft_usleep(philo, data, eating);
    
    if (philo->number_of_times_each_philosopher_must_eat != -1 
        && ++(philo->meals_count) == philo->number_of_times_each_philosopher_must_eat)
            sem_post(data->sem_ate_enough);
}

void    philo_process(t_philo *philo, t_data *data, int i)
{
    pthread_t   monitoring_thread;
    
    philo->philo_id = i;
    if (pthread_create(&monitoring_thread, NULL, check_for_death, (void *)philo))
        write(STDERR_FILENO, "pthread_create() failed\n", 24);
    if (pthread_detach(monitoring_thread))
        write(STDERR_FILENO, "pthread_detach() failed\n", 24);
    
     if (data->philos_count == 1)
         (sem_wait(data->sem_forks), print_state(philo, data, got_fork),
             usleep(philo->time_to_die * 1000), sem_post(data->sem_one_died), 
                 exit_philo(philo, data, &monitoring_thread));
   
    if (philo->number_of_times_each_philosopher_must_eat == 0)
         sem_post(data->sem_ate_enough);

    while (true)
    {
        //self_monitoring(philo, data);
        take_forks(philo, data);
        //self_monitoring(philo, data);
        eating_state(philo, data);
        //self_monitoring(philo, data);
        drop_forks(philo, data);

        //printf("%d ok ici\n", philo->philo_id + 1);
        //self_monitoring(philo, data);
        sleeping_state(philo, data);

        //self_monitoring(philo, data);
        thinking_state(philo, data);
    }
    // exit_philo(philo, data, &monitoring_thread);
}