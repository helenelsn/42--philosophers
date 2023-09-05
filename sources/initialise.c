/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:01:17 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/05 12:56:17 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    init_mutexes(int philos_nb, t_data data, t_philo *philos)
{
    /* Initialise mutexes*/
    /* Forks */
    int i;
    int err;

    i = 0;
    err = 0;
    while (i < philos_nb)
    {
        err = pthread_mutex_init(&data.forks[i], NULL); // doit malloc le *forks ?
        if (err)
            write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
        err = pthread_mutex_init(&philos[i].meals_count_m, NULL);
        if (err)
            write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
            // return ? 
        i++;
    }
    /* Printing */
    err = pthread_mutex_init(&data.msg_display, NULL);
    if (err)
        write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
    /* End_simulation */
    err = pthread_mutex_init(&data.end_simulation_m, NULL);
    if (err)
        write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
}

void    init_data(char **program_args, t_data *data)
{
       data->philos_count = philos_nb;
       data->time_to_die
}

void    create_threads(int philos_nb, t_philo *philos, t_data data)
{
    int i;
    int err;
    
    i = 0;
    err = 0;
    while (i < philos_nb)
    {
        /* ? */
        philos[i].meals_count = 0;
        philos[i].data = &data; 
        philos[i].philo_id = i;
        philos[i].tid = 0;
        
        err = pthread_create(philos[i].tid, NULL, philo_routine, (void *)&philos[i]);
        if (err)
            write(STDERR_FILENO, "pthread_create() failed\n", 24);
            // return ?
        i++;
    }
}