/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:31:58 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/10 22:40:13 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    init_data(t_data *data, char *count)
{
    data->philos_count = ft_atoi(count);
    data->ate_enough = sem_open(SEMA_MEALS, SEMA_FLAGS, SEMA_MODES, data->philos_count);
    data->forks = sem_open(SEMA_FORKS, SEMA_FLAGS, SEMA_MODES, data->philos_count / 2);
    data->state_msg = sem_open(SEMA_STREAM, SEMA_FLAGS, SEMA_MODES, 1);
    if (data->forks == SEM_FAILED || data->ate_enough == SEM_FAILED
        || data->one_died == SEM_FAILED || data->state_msg == SEM_FAILED)
        return (2);
}

void    init_philo(t_philo *philo, char **args)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    philo->time_to_die = ft_atoi(args[0]);
    philo->time_to_eat = ft_atoi(args[1]);
    philo->time_to_sleep = ft_atoi(args[2]);
    philo->meals_count = 0;
    philo->starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    philo->last_meal_tstamp = 0;
    philo->number_of_times_each_philosopher_must_eat = -1;
    if (args[3])
        philo->number_of_times_each_philosopher_must_eat = ft_atoi(args[3]);
}