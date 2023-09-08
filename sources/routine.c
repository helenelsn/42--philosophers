/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 13:22:20 by Helene            #+#    #+#             */
/*   Updated: 2023/09/08 15:05:03 by Helene           ###   ########.fr       */
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
    /* pthread_mutex_lock(&data->msg_display);
    printf("ft_is_end(), end_simulation = %d\n", is_end);
    pthread_mutex_unlock(&data->msg_display); */
	return (is_end);   
}

void    print_action() // prints the current's philo's action, giving the current timestamp in ms
{
	
}

suseconds_t	get_current_time(t_data *data)
{
	struct timeval curr_tv;

	gettimeofday(&curr_tv, NULL);
	return ((curr_tv.tv_sec * 1000 + curr_tv.tv_usec / 1000) - data->starting_time);
}

void    *philo_routine(void *routine_data)
{
    t_philo         *philo;
    struct timeval  tv;
    bool            is_alive;

    philo = (t_philo *)routine_data;
    is_alive = true;
    
    if (philo->philo_id % 2) // ie ceux qui grab leurs fourchettes en deuxieme
        usleep(philo->data->time_to_eat * 500); // quelle valeur ?
    
    while (true)
    {
        /* Eat */
        if (!eating_time(philo))
            return (NULL);
        
        /* Sleep */
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld ms %d is sleeping\n", get_current_time(philo->data), philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
        
        is_alive = ft_usleep(philo->data, sleeping);
        if (!is_alive)
            return (NULL);

        /* Think */
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld ms %d is thinking\n", get_current_time(philo->data), philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
		
		usleep((philo->data->time_to_die - (philo->data->time_to_eat + philo->data->time_to_sleep)) * 500);
    }
    return (NULL);
}