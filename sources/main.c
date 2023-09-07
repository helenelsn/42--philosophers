/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:24:33 by Helene            #+#    #+#             */
/*   Updated: 2023/09/07 20:08:43 by hlesny           ###   ########.fr       */
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
    struct timeval  start_sim;

    if (argc < 5 || argc > 6)
        return (1);
    
    gettimeofday(&start_sim, NULL);
    /* Initialise mutexes */
    init_data(argv + 1, &data);
    
    /* Create threads */
    philos = (t_philo *)malloc(sizeof(t_philo) * data.philos_count);
    if (!philos)
        return (EXIT_FAILURE);
    create_threads(philos, data);
        
    /* Monitoring */
    while(!ft_end_simulation(data, philos))
        usleep(100); // usleep est en microsecondes
        
    pthread_mutex_lock(&data.end_simulation_m);
    data.end_simulation = true;
    pthread_mutex_unlock(&data.end_simulation_m);
    
    join_threads(philos);
    destroy_mutexes(philos, data);
}