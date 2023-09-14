/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_while_waiting.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 23:21:59 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/14 20:52:45 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"



void    *check_for_death(void *arg)
{
    int             i;
    unsigned long   last_meal;
    t_philo *philo_monitor;
    
    i = 0;
    philo_monitor = (t_philo *)arg;
    while (true)
    {
        sem_wait(philo_monitor->sem_last_meal);
        last_meal = philo_monitor->last_meal_tstamp;
        sem_post(philo_monitor->sem_last_meal);
        
        if (sem_open(SEMA_END, 0) != SEM_FAILED) // check if another philo died
        {
            //printf("%ld ------------- philo %d, exiting monitoring thread\n", get_current_time(philo_monitor), philo_monitor->philo_id + 1);
            return (NULL);
        }
        if (get_current_time(philo_monitor) >= last_meal + philo_monitor->time_to_die)
        {
            
            //sem_wait(philo_monitor->data->sem_state_msg);
            philo_monitor->data->sem_end = sem_open(SEMA_END, SEMA_FLAGS, SEMA_MODES, 0);
            sem_wait(philo_monitor->data->sem_state_msg);
            if (sem_open(SEMA_END_MSG, 0) == SEM_FAILED && errno == ENOENT)
            {
                philo_monitor->data->sem_end_msg = sem_open(SEMA_END_MSG, SEMA_FLAGS, SEMA_MODES, 0);
                printf("%ld %d died\n", get_current_time(philo_monitor), philo_monitor->philo_id + 1);
            }
            sem_post(philo_monitor->data->sem_state_msg);
            //printf("%d ok there\n", philo_monitor->philo_id + 1);
            
            
            //sem_post(philo_monitor->data->sem_state_msg);
            /* while (i < philo_monitor->data->philos_count)
            {
                sem_post(philo_monitor->data->sem_forks);
                i++;
            } */
            
            // sem_unlink(SEMA_END);
            // sem_open(SEMA_END, O_CREAT);
            //sem_post(philo_monitor->data->sem_one_died);
            return (NULL);
        }
        usleep(5);
    }
    return (NULL);
}
