/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:01:17 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/07 23:06:52 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    init_data_mutexes(t_data *data)
{
    /* Forks and meals counts */
    int i;
    int err;

    i = 0;
    err = 0;
    data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->philos_count);
    if (!data->forks)
        return ; // and exit
    while (i < data->philos_count)
    {
        err = pthread_mutex_init(&data->forks[i], NULL); // doit malloc le *forks ?
        if (err)
            write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
            // return ? 
        i++;
    }
    /* Printing */
    err = pthread_mutex_init(&data->msg_display, NULL);
    if (err)
        write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
    /* End_simulation */
    err = pthread_mutex_init(&data->end_simulation_m, NULL);
    if (err)
        write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
    
}

void    init_data(char **program_args, t_data *data)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    data->starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000; // convert in ms
    data->number_of_times_each_philosopher_must_eat = -1;
    data->philos_count = ft_atoi(program_args[0]);
    data->time_to_die = ft_atoi(program_args[1]);
    data->time_to_eat = ft_atoi(program_args[2]);
    data->time_to_sleep = ft_atoi(program_args[3]);
    data->end_simulation = false;
    if (program_args[4])
        data->number_of_times_each_philosopher_must_eat = ft_atoi(program_args[4]);
    init_data_mutexes(data);
}

void    create_threads(t_philo *philos, t_data data)
{
    int i;
    int err;
    
    i = 0;
    err = 0;
    while (i < data.philos_count)
    {
        philos[i].meals_count = 0;
        philos[i].data = &data; 
        philos[i].philo_id = i;
        philos[i].tid = 0;
        philos[i].last_meal_tstamp = data.starting_time;
        
        err = pthread_mutex_init(&philos[i].meals_count_m, NULL); // mutex à mettre dans data ou dans philo ?
        if (err)
            write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);

        err = pthread_create(&philos[i].tid, NULL, philo_routine, (void *)&philos[i]);
        if (err)
            write(STDERR_FILENO, "pthread_create() failed\n", 24);
            // return ?
        i++;
    }
}