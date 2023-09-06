/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 13:22:20 by Helene            #+#    #+#             */
/*   Updated: 2023/09/06 19:58:23 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

pthread_mutex_t *get_fork(t_philo *philo, int fork_status)
{
    // pthread_mutex_lock(&philo->data->msg_display);
    // printf("dans philo %d, fork status = %d\n", philo->philo_id + 1, fork_status);
    // pthread_mutex_unlock(&philo->data->msg_display);
    if (fork_status == left)
        return (&philo->data->forks[(philo->philo_id - 1) % philo->data->philos_count]); // a modifier ?
    else if (fork_status == right)
    {
        // pthread_mutex_lock(&philo->data->msg_display);
        // printf("dans else if(), philo %d, philos count = %d, \n", philo->philo_id + 1, philo->data->philos_count);
        // pthread_mutex_unlock(&philo->data->msg_display);
        return (&philo->data->forks[(philo->philo_id + 1) % philo->data->philos_count]);
    }
    return (NULL);
}

bool    ft_is_end(t_data *data)
{
    bool    is_end;
	
    is_end = false;
    pthread_mutex_lock(&data->end_simulation_m);
    is_end = data->end_simulation;
    pthread_mutex_unlock(&data->end_simulation_m);
    /* pthread_mutex_lock(&data->msg_display);
    printf("ft_is_end(), ok ici\n");
    pthread_mutex_unlock(&data->msg_display); */
	return (is_end);   
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

void    print_action() // prints the current's philo's action, giving the current timestamp in ms
{
	
}

suseconds_t	get_current_time(t_data *data)
{
	struct timeval curr_tv;

	gettimeofday(&curr_tv, NULL);
	return (curr_tv.tv_sec * 1000 - data->starting_time);
}

void    update_last_meal(t_philo *philo)
{
	struct timeval current_tv;
	
	gettimeofday(&current_tv, NULL);
	pthread_mutex_lock(&philo->last_meal_m);
	philo->last_meal_tstamp = current_tv.tv_sec * 1000 - philo->data->starting_time;
	pthread_mutex_unlock(&philo->last_meal_m);
}

void    *philo_routine(void *routine_data)
{
    t_philo         *philo;
    struct timeval  tv;
    bool            philo_state;

    philo = (t_philo *)routine_data;
    philo_state = true;
    if (philo->philo_id % 2) // ie ceux qui grab leurs fourchettes en deuxieme
        usleep(10); // quelle valeur ?
        // écrire aussi "is thinking" a la mano la premiere fois ici?
    
    while (true) // bof
    {
/* 		pthread_mutex_lock(&philo->data->msg_display);
            printf("dans philo %d, ok ici\n", philo->philo_id + 1);
            pthread_mutex_unlock(&philo->data->msg_display); */
        /* Eat */
        if (philo->philo_id % 2 == 0) // pair, droitier (choix arbitraire)
        {
    		pthread_mutex_lock(&philo->data->msg_display);
            printf("dans philo %d, ok ici\n", philo->philo_id + 1);
            pthread_mutex_unlock(&philo->data->msg_display);
            /* if (ft_is_end(philo->data))
                return (NULL); */
            // fork à droite
			
            pthread_mutex_lock(get_fork(philo, right));
            pthread_mutex_lock(&philo->data->msg_display);
            printf("%ld ms %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
            pthread_mutex_unlock(&philo->data->msg_display);
            if (end_thread(philo, right))
                return (NULL);
            // fork à gauche
            pthread_mutex_lock(get_fork(philo, left));
            pthread_mutex_lock(&philo->data->msg_display);
            printf("%ld ms %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
            pthread_mutex_unlock(&philo->data->msg_display);
        }
        else // impair, gauchier
        {
			pthread_mutex_lock(&philo->data->msg_display);
            printf("dans philo %d, ok ici\n", philo->philo_id + 1);
            pthread_mutex_unlock(&philo->data->msg_display);
            /* if (ft_is_end(philo->data))
                return (NULL); */
            // fork à gauche
            pthread_mutex_lock(get_fork(philo, left));
            pthread_mutex_lock(&philo->data->msg_display);
            printf("%ld ms %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
            pthread_mutex_unlock(&philo->data->msg_display);
            if (end_thread(philo, left))
                return (NULL);
            // fork à droite
            pthread_mutex_lock(get_fork(philo, right));
            pthread_mutex_lock(&philo->data->msg_display);
            printf("%ld ms %d grabbed a fork\n", get_current_time(philo->data), philo->philo_id + 1);
            pthread_mutex_unlock(&philo->data->msg_display);
        }
        if (end_thread(philo, both))
            return (NULL);
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld ms %d is eating\n", get_current_time(philo->data), philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
        
        /* Set the last_meal_tstamp */
        update_last_meal(philo);
        
        philo_state = ft_usleep(philo->data, eating);
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
        if (philo_state)
            return (NULL);
        pthread_mutex_lock(&philo->meals_count_m);
        philo->meals_count++;
        pthread_mutex_unlock(&philo->meals_count_m);
        
        /* Sleep */
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld ms %d is sleeping\n", get_current_time(philo->data), philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
        
        philo_state = ft_usleep(philo->data, sleeping);
        if (philo_state)
            return (NULL);

        /* Think */
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%ld ms %d is thinking\n", get_current_time(philo->data), philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
		
		//usleep(10);
    }
}