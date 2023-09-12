/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 22:57:42 by Helene            #+#    #+#             */
/*   Updated: 2023/09/12 01:41:24 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    print_state(t_philo *philo, t_data *data, int state)
{
    sem_wait(data->sem_state_msg);
    printf("%ld %d ", get_current_time(philo), philo->philo_id + 1);
    if (state == eating)
        printf("is eating\n");
    else if (state == sleeping)
        printf("is sleeping\n");
    else if (state == thinking) 
        printf("is thinking\n");
    else if (state == got_fork)
        printf("has taken a fork\n");
    sem_post(data->sem_state_msg);
}

long	get_current_time(t_philo *philo)
{
	struct timeval curr_tv;

	gettimeofday(&curr_tv, NULL);
	return ((curr_tv.tv_sec * 1000 + curr_tv.tv_usec / 1000) - philo->starting_time);
}

/*
States :    0 when eating
            1 when sleeping
            2 when thinking
*/
void    ft_usleep(t_philo *philo, t_data *data, int state) 
{
    unsigned int    state_length;
    
    state_length = get_current_time(philo) + philo->time_to_eat;
    if (state == sleeping)
        state_length = get_current_time(philo) + philo->time_to_sleep;
    else if (state == thinking)
        state_length = get_current_time(philo) + ((philo->time_to_die - philo->time_to_eat - philo->time_to_sleep) / 2);
    while (get_current_time(philo) < state_length)
    {
        /*if (ft_is_end(data))
            return ;*/
        self_monitoring(philo, data);
        usleep(100);
    }
}