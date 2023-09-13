/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:24:15 by Helene            #+#    #+#             */
/*   Updated: 2023/09/13 01:48:09 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    sleeping_state(t_philo *philo, t_data *data)
{
    print_state(philo, data, sleeping);
    ft_usleep(philo, data, sleeping);
}

void    thinking_state(t_philo *philo, t_data *data)
{
    print_state(philo, data, thinking);
    ft_usleep(philo, data, thinking);
}

void    self_monitoring(t_philo *philo, t_data *data)
{
    unsigned long   last_meal;
    
    sem_wait(philo->sem_last_meal);
    last_meal = philo->last_meal_tstamp;
    sem_post(philo->sem_last_meal);
    /*printf("%d : get_current_time = %ld, last meal = %ld, last meal + time_to_die = %ld\n",
        philo->philo_id + 1, get_current_time(philo), last_meal, last_meal + philo->time_to_die);*/
    
    if (get_current_time(philo) >= last_meal + philo->time_to_die)
    {
        sem_post(data->sem_one_died);
        sem_wait(data->sem_state_msg); // ne sem_post pas apres comme ca plus personne d'autre ne peut écrire
        printf("%ld %d died\n", get_current_time(philo), philo->philo_id + 1);
        exit_philo(philo, data); 
    }
}

void    take_forks(t_philo *philo, t_data *data)
{
    /*while (data->sem_forks->__align < 2) //check le retour du monitoring en meme temps ?
    {
        self_monitoring(philo, data);
        usleep(10);
    }*/
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
    ft_usleep(philo, data, eating);
    
    if (philo->number_of_times_each_philosopher_must_eat != -1 
        && ++(philo->meals_count) == philo->number_of_times_each_philosopher_must_eat)
        {
        printf("%ld %d ATE ENOUGH\n", get_current_time(philo), philo->philo_id + 1);
        sem_post(data->sem_ate_enough);
        }
    /*if (--(philo->meals_count) == 0)
    {
        printf("%ld %d ATE ENOUGH\n", get_current_time(philo), philo->philo_id + 1);
        sem_post(data->sem_ate_enough);
    }*/
}

void        set_starting_time(t_philo *philo)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    philo->starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void    philo_process(t_philo *philo, t_data *data, int i)
{
    philo->philo_id = i;
   
     if (data->philos_count == 1)
         (sem_wait(data->sem_forks), print_state(philo, data, got_fork),
             usleep(philo->time_to_die * 1000), sem_post(data->sem_one_died), 
                 exit_philo(philo, data));
   
    if (philo->number_of_times_each_philosopher_must_eat == 0)
         sem_post(data->sem_ate_enough);

    set_starting_time(philo);
    while (true)
    {
        self_monitoring(philo, data);
        
        take_forks(philo, data);
        eating_state(philo, data);
        self_monitoring(philo, data);
        drop_forks(philo, data);
        self_monitoring(philo, data);

        sleeping_state(philo, data);
        self_monitoring(philo, data);

        thinking_state(philo, data);
        self_monitoring(philo, data);
    }
    exit_philo(philo, data);
}