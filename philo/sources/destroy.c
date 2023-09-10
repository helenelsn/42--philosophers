/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:02:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/10 21:11:55 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    join_threads(t_philo *philos)
{
    int i;
    int err;

    i = 0;
    err = 0;
    while (i < philos->data->philos_count)
    {
        err = pthread_join(philos[i].tid, NULL);
        if (err)
            write(STDERR_FILENO, "pthread_join() failed\n", 22);
        i++;
    }
}

void    destroy_mutexes(t_philo *philos, t_data data)
{
    int i;
    int err;

    i = 0;
    err = 0;
    while (i < philos->data->philos_count)
    {
        err = pthread_mutex_destroy(&data.forks[i]);
        if (err)
            write(STDERR_FILENO, "pthread_mutex_destroy() failed\n", 31);
        err = pthread_mutex_destroy(&philos[i].meals_count_m);
        if (err)
            write(STDERR_FILENO, "pthread_mutex_destroy() failed\n", 31);
        i++;
    }
    err = pthread_mutex_destroy(&data.msg_display);
    if (err)
        write(STDERR_FILENO, "pthread_mutex_destroy() failed\n", 28);
    err = pthread_mutex_destroy(&data.end_simulation_m);
    if (err)
        write(STDERR_FILENO, "pthread_mutex_destroy() failed\n", 28);
}

void    free_all(t_data data, t_philo *philos)
{
    free(data.forks);
    free(philos);
}
