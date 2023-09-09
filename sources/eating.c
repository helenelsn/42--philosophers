/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:00:23 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/09 13:30:28 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

pthread_mutex_t *get_fork(t_philo *philo, int fork_status)
{
    if (fork_status == left)
    {
        if (philo->philo_id == 0)
            return (&philo->data->forks[philo->data->philos_count - 1]);
        return (&philo->data->forks[(philo->philo_id - 1)]);
    }
    else if (fork_status == right)
        return (&philo->data->forks[philo->philo_id]);
    return (NULL);
}

bool    end_thread(t_philo *philo, int fork_status)
{
    if (!ft_is_end(philo->data))
        return (false);
    if (fork_status == left || fork_status == both)
        pthread_mutex_unlock(get_fork(philo, left));
    if (fork_status == right || fork_status == both)
        pthread_mutex_unlock(get_fork(philo, right));
    return (true);
}

void    update_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_m);
	philo->last_meal_tstamp = get_current_time(philo->data);
	pthread_mutex_unlock(&philo->last_meal_m);
}

bool    eating_time(t_philo *philo)
{
    bool is_alive;

    is_alive = true;
    if (philo->philo_id % 2 == 0) // pair, droitier (choix arbitraire)
    {   
        if (ft_is_end(philo->data))
            return (false);
        // fork à droite
        pthread_mutex_lock(get_fork(philo, right));
        if (end_thread(philo, right))
            return (false);
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
            
        // fork à gauche
        pthread_mutex_lock(get_fork(philo, left));
        /*if (end_thread(philo, both))
            return (false);
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);*/
    }
    else // impair, gauchier
    {
        if (ft_is_end(philo->data))
            return (false);
        // fork à gauche
        pthread_mutex_lock(get_fork(philo, left));
        if (end_thread(philo, left))
            return (false);
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
        // fork à droite
        pthread_mutex_lock(get_fork(philo, right));
        /*if (end_thread(philo, both))
            return (false);
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);*/
    }
    
    /*if (end_thread(philo, both))
        return (false);*/
    if (end_thread(philo, both))
            return (false);
    pthread_mutex_lock(&philo->data->msg_display);
    printf("%ld %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
    pthread_mutex_unlock(&philo->data->msg_display);
    if (ft_is_end(philo->data))
        return (false);
    pthread_mutex_lock(&philo->data->msg_display);
    printf("%ld %d is eating\n", get_current_time(philo->data), philo->philo_id + 1);
    pthread_mutex_unlock(&philo->data->msg_display);


    /* Set the last_meal_tstamp */
    update_last_meal(philo);
    
    // pthread_mutex_lock(&philo->data->msg_display);
    // printf("dans philo %d, ok ici\n", philo->philo_id + 1);
    // pthread_mutex_unlock(&philo->data->msg_display);
    
    is_alive = ft_usleep(philo->data, eating);
    if (philo->philo_id % 2 == 0)
    {
        pthread_mutex_unlock(get_fork(philo, left));
        pthread_mutex_unlock(get_fork(philo, right));
    }
    else
    {
        pthread_mutex_unlock(get_fork(philo, right));
        pthread_mutex_unlock(get_fork(philo, left));
    }
    if (!is_alive)
        return (false);
    pthread_mutex_lock(&philo->meals_count_m);
    philo->meals_count++;
    pthread_mutex_unlock(&philo->meals_count_m);
    return (true);
}