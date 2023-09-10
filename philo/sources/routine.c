/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 13:22:20 by Helene            #+#    #+#             */
/*   Updated: 2023/09/10 20:29:13 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool    ft_is_end(t_data *data)
{
    bool    is_end;
	
    is_end = false;
    pthread_mutex_lock(&data->end_simulation_m);
    is_end = data->end_simulation;
    pthread_mutex_unlock(&data->end_simulation_m);
	return (is_end);   
}

void    print_action() // prints the current's philo's action, giving the current timestamp in ms
{
	
}

bool    sleeping_time(t_philo *philo)
{
    if (ft_is_end(philo->data))
        return (false);
    pthread_mutex_lock(&philo->data->msg_display);
    printf("%ld %d is sleeping\n", get_current_time(philo->data), philo->philo_id + 1);
    pthread_mutex_unlock(&philo->data->msg_display);
    
    ft_usleep(philo->data, sleeping);
    if (ft_is_end(philo->data))
        return (false);
    return (true);
}

bool    thinking_time(t_philo *philo)
{
    if (ft_is_end(philo->data))
        return (false);
    pthread_mutex_lock(&philo->data->msg_display);
    printf("%ld %d is thinking\n", get_current_time(philo->data), philo->philo_id + 1);
    pthread_mutex_unlock(&philo->data->msg_display);
	
	usleep((philo->data->time_to_die - (philo->data->time_to_eat + philo->data->time_to_sleep)) * 500);
    if (ft_is_end(philo->data))
        return (false);
    return (true);
}

void    *philo_routine(void *routine_data)
{
    t_philo         *philo;

    philo = (t_philo *)routine_data;
    
    if (philo->philo_id % 2)
        usleep(philo->data->time_to_eat * 500);
    
    while (true)
    {
        /* Eat */
        if (!eating_time(philo))
            return (NULL);
        
        /* Sleep */
        if (!sleeping_time(philo))
            return (NULL);

        /* Think */
        if (!thinking_time(philo))
            return (NULL);
    }
    return (NULL);
}

void    *philo_routine_one(void *routine_data)
{
    t_philo         *philo;

    philo = (t_philo *)routine_data;
    pthread_mutex_lock(&philo->data->forks[0]);
    if (end_thread(philo, right))
        return (NULL);
    pthread_mutex_lock(&philo->data->msg_display);
    printf("%ld %d has taken a fork\n", get_current_time(philo->data), 1);
    pthread_mutex_unlock(&philo->data->msg_display);
    while (get_current_time(philo->data) < philo->data->time_to_die)
        usleep(100);
    pthread_mutex_unlock(&philo->data->forks[0]);
    return (NULL);
}