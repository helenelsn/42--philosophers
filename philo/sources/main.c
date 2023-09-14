/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:24:33 by Helene            #+#    #+#             */
/*   Updated: 2023/09/14 17:22:10 by Helene           ###   ########.fr       */
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

    if (!valid_input(argc - 1, argv + 1))
        return (1);
    init_data(argv + 1, &data);
    philos = (t_philo *)malloc(sizeof(t_philo) * data.philos_count);
    if (!philos)
        return (2);
    create_threads(philos, data);
    supervise_simulation(philos);
    join_threads(philos);
    destroy_mutexes(philos, data);
    free_all(data, philos);
}