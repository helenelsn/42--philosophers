/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:24:33 by Helene            #+#    #+#             */
/*   Updated: 2023/09/09 13:29:23 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/*
argc[1] : number_of_philosophers ( = number of forks)
argc[2] : time_to_die 
argc[3] : time_to_eat
argc[4] : time_to_sleep
argc[5] : /optional/ number_of_times_each_philosopher_must_eat
*/
int main(int argc, char **argv)
{
    t_data          data;
    t_philo         *philos;

    if (argc < 5 || argc > 6)
    {
        printf("This program requires between 4 and 5 arguments!\n");
        printf("Example : ./philo 7 1500 300 200\n");
        return (1);
    }
    /* Initialise mutexes */
    init_data(argv + 1, &data);
    /* Create threads */
    philos = (t_philo *)malloc(sizeof(t_philo) * data.philos_count);
    if (!philos)
        return (2);
    create_threads(philos, data);
    supervise_simulation(philos);
    join_threads(philos);
    destroy_mutexes(philos, data);
    free_all(data, philos);
}