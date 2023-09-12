/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:42:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/12 19:01:47 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    close_semaphores(t_philo *philo, t_data *data)
{
    sem_close(data->sem_ate_enough);
    sem_close(data->sem_forks);
    sem_close(data->sem_one_died);
    sem_close(data->sem_state_msg);
    sem_close(philo->sem_last_meal);
}

void    exit_philo(t_philo *philo, t_data *data)
{
    close_semaphores(philo, data);
    unlink_semaphores();
    free(data->pids);
    data->pids = NULL;
    exit(1); 
}

void    exit_parent(t_philo *philo, t_data *data)
{
    close_semaphores(philo, data);
    unlink_semaphores();
    //join_threads(philo);
    free(data->pids);
    data->pids = NULL;
    
}