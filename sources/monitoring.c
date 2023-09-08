/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 18:03:41 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/08 15:22:07 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Checks if the end_simulation variable is true */
bool    ft_end_simulation(t_data data, t_philo *philos)
{
    bool            end_things;
    int             ate_enough;
    int             i;
    unsigned long   last_meal;

    end_things = false;
    i = 0;
    ate_enough = 0;

    /* Checks if a philosopher died */
    while (i < data.philos_count && !end_things)
    {
        pthread_mutex_lock(&philos[i].last_meal_m);
        last_meal = philos[i].last_meal_tstamp;
        pthread_mutex_unlock(&philos[i].last_meal_m);
        if ((get_current_time(&data) - last_meal) >= data.time_to_die)
        {
            pthread_mutex_lock(&data.msg_display);
            // printf("----------- \n starting time : %ld ms, current timestamp : %ld ms, last meal : %d ms, time to die : %d ms \n-----------\n ", data.starting_time, time.tv_sec * 1000 + time.tv_usec / 1000, last_meal, data.time_to_die);
            // printf("gettimestamp() - last_meal = %lu\n", get_timestamp() - last_meal);
            printf("%d ms %d has died\n", get_current_time(&data), i + 1);
            pthread_mutex_unlock(&data.msg_display);
            end_things = true;
        }
        i++;
    }
    
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
    if (end_things)
    {
        pthread_mutex_lock(&philos->data->end_simulation_m);
        philos->data->end_simulation = true;
        pthread_mutex_unlock(&philos->data->end_simulation_m);
        // pthread_mutex_lock(&data.msg_display);
        // printf("amount of philos who ate enough : %d. Times they ate : %ld\n", ate_enough, data.number_of_times_each_philosopher_must_eat);
        // pthread_mutex_unlock(&data.msg_display);
    }
    return (end_things);
}

bool    check_hunger(t_philo *philos)
{
    int             i;
    unsigned long   last_meal;

    i = 0;
    /* Checks if a philosopher died */
    while (i < philos->data->philos_count)
    {
        pthread_mutex_lock(&philos[i].last_meal_m);
        last_meal = philos[i].last_meal_tstamp;
        pthread_mutex_unlock(&philos[i].last_meal_m);
        if ((get_current_time(philos->data) - last_meal) >= philos->data->time_to_die)
        {
            pthread_mutex_lock(&philos->data->msg_display);
            printf("%d ms %d has died\n", get_current_time(philos->data), i + 1);
            pthread_mutex_unlock(&philos->data->msg_display);
            return (true);
        }
        i++;
    }
    return (false);
}

bool    check_meals_count(t_philo *philos)
{
    int i;
    int ate_enough;

    i = 0;
    ate_enough = 0;
    if (philos->data->number_of_times_each_philosopher_must_eat > -1)
    {
        while (i < philos->data->philos_count)
        {
            pthread_mutex_lock(&philos[i].meals_count_m);
            if (philos[i].meals_count >= philos->data->number_of_times_each_philosopher_must_eat)
                ate_enough++;
            pthread_mutex_unlock(&philos[i].meals_count_m);
            i++;
        }
        if (ate_enough == philos->data->philos_count)
            return (true);
    }
    return (false);
}

void    supervise_simulation(t_philo *philos)
{
    bool end_simulation;

    end_simulation = false;
    while (end_simulation == false)
    {
        end_simulation = check_hunger(philos);
        end_simulation = check_meals_count(philos);
        if (end_simulation)
        {
            pthread_mutex_lock(&philos->data->end_simulation_m);
            philos->data->end_simulation = true;
            pthread_mutex_unlock(&philos->data->end_simulation_m);
        }
        usleep(50);
    }
    return ;
}