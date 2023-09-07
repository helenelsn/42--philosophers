/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 18:03:41 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/07 23:26:29 by Helene           ###   ########.fr       */
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
    unsigned long   last_meal;

    end_things = false;
    i = 0;
    ate_enough = 0;

    /* Checks if a philosopher died */
    gettimeofday(&time, NULL);
    while (i < data.philos_count && !end_things)
    {
        pthread_mutex_lock(&philos[i].last_meal_m);
        last_meal = philos[i].last_meal_tstamp;
        pthread_mutex_unlock(&philos[i].last_meal_m);
        if ((get_timestamp() - last_meal) >= data.time_to_die)
        {
            pthread_mutex_lock(&data.msg_display);
            printf("----------- \n starting time : %ld ms, current timestamp : %ld ms, last meal : %d ms, time to die : %d ms \n-----------\n ", data.starting_time, time.tv_sec * 1000 + time.tv_usec / 1000, last_meal, data.time_to_die);
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
    // if (end_things)
    //     return (end_things);

    /* If the number_of_times_each_philosopher_must_eat variable is given,
    checks if all philosophers have eaten enough times.
    If so, the simulation must stop */
    i = 0;
    if (data.number_of_times_each_philosopher_must_eat > -1)
    {
        while (i < data.philos_count)
        {
            pthread_mutex_lock(&philos[i].meals_count_m);
            /* pthread_mutex_lock(&data.msg_display);
            printf("philo %d ate %d times\n", philos[i].philo_id + 1 ,philos[i].meals_count);
            pthread_mutex_unlock(&data.msg_display); */
            if (philos[i].meals_count >= data.number_of_times_each_philosopher_must_eat)
                ate_enough++;
            pthread_mutex_unlock(&philos[i].meals_count_m);
            i++;
        }
        if (ate_enough == data.philos_count)
            end_things = true;
    }
    if (end_things)
    {
        pthread_mutex_lock(&philos->data->end_simulation_m);
        philos->data->end_simulation = true;
        pthread_mutex_unlock(&philos->data->end_simulation_m);
        pthread_mutex_lock(&data.msg_display);
        printf("amount of philos who ate enough : %d. Times they ate : %ld\n", ate_enough, data.number_of_times_each_philosopher_must_eat);
        pthread_mutex_unlock(&data.msg_display);
    }
    return (end_things);
}