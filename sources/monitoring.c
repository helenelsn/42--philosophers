/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 18:03:41 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/05 20:13:20 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Checks if the end_simulation variable is true */
bool    ft_end_simulation(t_data data, t_philo *philos)
{
    struct timeval  time;
    bool            end_things;
    int             ate_enough;
    int             i;

    end_things = false;
    i = 0;
    ate_enough = 0;

    /* Checks if a philosopher died */
    while (i < data.philos_count && !end_things)
    {
        if (philos[i].last_meal_tstamp >= data.time_to_die) // comment ecrire le last_meal_tstamp ?
        {
            pthread_mutex_lock(&data.msg_display);
            printf("%d has died\n", i + 1);
            pthread_mutex_unlock(&data.msg_display);
            end_things = true;
        }
        i++;
    }
    
    /* pthread_mutex_lock(&data.end_simulation_m);
    if (data.end_simulation)
        end_things = true;
    pthread_mutex_unlock(&data.end_simulation_m); */

    /* If one died, there is no need to check if they ate enough*/
    if (end_things)
        return (end_things);

    /* If the number_of_times_each_philosopher_must_eat variable is given,
    checks if all philosophers have eaten enough times.
    If so, the simulation must stop */
    i = 0;
    if (data.number_of_times_each_philosopher_must_eat > -1)
    {
        while (i < data.philos_count)
        {
            pthread_mutex_lock(&philos[i].meals_count_m);
            if (philos[i].meals_count >= data.number_of_times_each_philosopher_must_eat)
                ate_enough++;
            pthread_mutex_unlock(&philos[i].meals_count_m);
            i++;
        }
        if (ate_enough == data.philos_count)
            end_things = true;
    }
    return (end_things);
}