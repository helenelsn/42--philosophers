/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_while_waiting.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 23:21:59 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/14 02:28:14 by hlesny           ###   ########.fr       */
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
        
        // if (get_current_time(philo_monitor) >= last_meal + philo_monitor->time_to_die)
        //     printf("-------------------------------------------------------------------------------------------------lol\n");
        // else
        // {
        //      printf("philo %d, current_time = %ld, last_meal + time_to_die = %ld\n", \
        // philo_monitor->philo_id + 1, get_current_time(philo_monitor), \
        // last_meal + philo_monitor->time_to_die);
        // }
            // printf("---------------------------------------------------------youpi\n");
        
        if (get_current_time(philo_monitor) >= last_meal + philo_monitor->time_to_die)
        {
            /* printf("-------------------------------------------------------------------------------------------------lol\n");
             printf("philo %d died at %ld ms\n", philo_monitor->philo_id + 1, get_current_time(philo_monitor));
             printf("sem val is\n %p\n",philo_monitor->data);
            printf("-------------------------------------------------------------------------------------------------lol1\n"); */
            sem_wait(philo_monitor->data->sem_state_msg); // ne sem_post pas apres comme ca plus personne d'autre ne peut écrire
           
            printf("%ld %d died\n", get_current_time(philo_monitor), philo_monitor->philo_id + 1);
            
           //printf("-------------------------------------------------------------------------------------------------lol1.5\n");
            sem_post(philo_monitor->data->sem_one_died);
             //printf("-------------------------------------------------------------------------------------------------lol2\n");
            return (NULL); 
        }
        usleep(50);
    }
    return (NULL);
}
