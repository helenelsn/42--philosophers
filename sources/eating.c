/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:00:23 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/07 23:12:50 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

pthread_mutex_t *get_fork(t_philo *philo, int fork_status)
{
    // pthread_mutex_lock(&philo->data->msg_display);
    // printf("dans philo %d, fork status = %d\n", philo->philo_id + 1, fork_status);
    // pthread_mutex_unlock(&philo->data->msg_display);

    if (fork_status == left)
    {
        if (philo->philo_id == 0)
            return (&philo->data->forks[philo->data->philos_count - 1]);
        return (&philo->data->forks[(philo->philo_id - 1)]); // a modifier ?
    }
    else if (fork_status == right)
    {
        // pthread_mutex_lock(&philo->data->msg_display);
        // printf("dans else if(), philo %d, philos count = %d, \n", philo->philo_id + 1, philo->data->philos_count);
        // pthread_mutex_unlock(&philo->data->msg_display);
        return (&philo->data->forks[philo->philo_id]);
    }
    return (NULL);
}

void    update_last_meal(t_philo *philo)
{
	struct timeval current_tv;
	
	gettimeofday(&current_tv, NULL);
	pthread_mutex_lock(&philo->last_meal_m);
	philo->last_meal_tstamp = (current_tv.tv_sec * 1000 + current_tv.tv_usec / 1000); //- philo->data->starting_time;
	pthread_mutex_unlock(&philo->last_meal_m);
}

bool    end_thread(t_philo *philo, int fork_status)
{
    // pthread_mutex_lock(&philo->data->msg_display);
    // printf("dans philo %d, ok ici\n", philo->philo_id + 1);
    // pthread_mutex_unlock(&philo->data->msg_display);
    if (!ft_is_end(philo->data))
        return (false);
    if (fork_status == left || fork_status == both)
        pthread_mutex_unlock(get_fork(philo, left));
    if (fork_status == right || fork_status == both)
        pthread_mutex_unlock(get_fork(philo, right));
    return (true);
}

bool    eating_time(t_philo *philo)
{
    bool is_alive;

    is_alive = true;
    if (philo->philo_id % 2 == 0) // pair, droitier (choix arbitraire)
    {
    	/* pthread_mutex_lock(&philo->data->msg_display);
        printf("dans philo %d, ok ici\n", philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display); */
        
        if (ft_is_end(philo->data))
            return (false);
        // fork à droite
        pthread_mutex_lock(get_fork(philo, right));
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld ms %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
        //printf("%ld ms %d grabbed fork %d, right\n", get_current_time(philo->data), philo->philo_id + 1, philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
        if (end_thread(philo, right))
            return (false);
            
        // fork à gauche
        pthread_mutex_lock(get_fork(philo, left));
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld ms %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
        /* if (philo->philo_id == 0)
            printf("%ld ms %d grabbed fork %d, left\n", get_current_time(philo->data), philo->philo_id + 1, philo->data->philos_count);
        else
            printf("%ld ms %d grabbed fork %d, left\n", get_current_time(philo->data), philo->philo_id + 1, philo->philo_id); */
        pthread_mutex_unlock(&philo->data->msg_display);
    }
    else // impair, gauchier
    {
		/* pthread_mutex_lock(&philo->data->msg_display);
        printf("dans philo %d, ok ici\n", philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display); */
        if (ft_is_end(philo->data))
            return (false);
        // fork à gauche
        pthread_mutex_lock(get_fork(philo, left));
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld ms %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
        /* if (philo->philo_id == 0)
            printf("%ld ms %d grabbed fork %d, left\n", get_current_time(philo->data), philo->philo_id + 1, philo->data->philos_count);
        else
            printf("%ld ms %d grabbed fork %d, left\n", get_current_time(philo->data), philo->philo_id + 1, philo->philo_id);             */
        pthread_mutex_unlock(&philo->data->msg_display);
        if (end_thread(philo, left))
            return (false);
        // fork à droite
        pthread_mutex_lock(get_fork(philo, right));
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld ms %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
        //printf("%ld ms %d grabbed fork %d, right\n", get_current_time(philo->data), philo->philo_id + 1, (philo->philo_id + 1));
        pthread_mutex_unlock(&philo->data->msg_display);
    }
    
    if (end_thread(philo, both))
        return (false);
    pthread_mutex_lock(&philo->data->msg_display);
    printf("%ld ms %d is eating\n", get_current_time(philo->data), philo->philo_id + 1);
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