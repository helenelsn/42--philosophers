/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_while_waiting.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 23:21:59 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/14 16:21:21 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"



void    *check_for_death(void *arg)
{
    unsigned long   last_meal;
    t_philo *philo_monitor;
    
    philo_monitor = (t_philo *)arg;
    while (true)
    {
        sem_wait(philo_monitor->sem_last_meal);
        last_meal = philo_monitor->last_meal_tstamp;
        sem_post(philo_monitor->sem_last_meal);
        
        if (sem_open(SEMA_END, 0) != SEM_FAILED) // check if another philo died
        {
            printf("%ld ------------- philo %d, exiting monitoring thread\n", get_current_time(philo_monitor), philo_monitor->philo_id + 1);
            return (NULL);
        }
        if (get_current_time(philo_monitor) >= last_meal + philo_monitor->time_to_die)
        {
            philo_monitor->data->sem_end = sem_open(SEMA_END, SEMA_FLAGS, SEMA_MODES, 0);
            sem_wait(philo_monitor->data->sem_state_msg); // ne sem_post pas apres comme ca plus personne d'autre ne peut écrire
            printf("%ld %d died\n", get_current_time(philo_monitor), philo_monitor->philo_id + 1);
            //sem_post(philo_monitor->data->sem_one_died);
            return (NULL);
        }
        usleep(5);
    }
    return (NULL);
}
