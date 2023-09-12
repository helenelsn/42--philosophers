/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:31:58 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/12 01:45:56 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    unlink_semaphores(void)
{
    if (sem_unlink(SEMA_FORKS) < 0)
        write(STDERR_FILENO, "sem_unlink() failed\n", 20);
    if (sem_unlink(SEMA_STREAM) < 0)
        write(STDERR_FILENO, "sem_unlink() failed\n", 20);
    if (sem_unlink(SEMA_MEALS) < 0)
        write(STDERR_FILENO, "sem_unlink() failed\n", 20);
    if (sem_unlink(SEMA_DEATH) < 0)
        write(STDERR_FILENO, "sem_unlink() failed\n", 20);
    if (sem_unlink(SEMA_LAST_MEAL) < 0)
        write(STDERR_FILENO, "sem_unlink() failed\n", 20);
}

bool    init_data(t_data *data, char *count)
{
    unlink_semaphores();
        
    data->stop_sim = false;
    data->philos_count = ft_atoi(count);
    data->sem_ate_enough = sem_open(SEMA_MEALS, SEMA_FLAGS, SEMA_MODES, 0);
    data->sem_forks = sem_open(SEMA_FORKS, SEMA_FLAGS, SEMA_MODES, data->philos_count);
    data->sem_state_msg = sem_open(SEMA_STREAM, SEMA_FLAGS, SEMA_MODES, 1);
    data->sem_one_died = sem_open(SEMA_DEATH, SEMA_FLAGS, SEMA_MODES, 0);
    if (data->sem_forks == SEM_FAILED || data->sem_ate_enough == SEM_FAILED
        || data->sem_one_died == SEM_FAILED || data->sem_state_msg == SEM_FAILED)
        return (false);
    return (true);
}

bool    init_philo(t_philo *philo, char **args)
{
    struct timeval tv;
    
    sem_unlink(SEMA_LAST_MEAL);
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
    philo->sem_last_meal = sem_open(SEMA_LAST_MEAL, SEMA_FLAGS, SEMA_MODES, 1);
    if (philo->sem_last_meal == SEM_FAILED)
        return (false);
    return (true);
}