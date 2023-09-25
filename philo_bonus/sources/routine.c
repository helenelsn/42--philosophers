/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:24:15 by Helene            #+#    #+#             */
/*   Updated: 2023/09/25 14:00:06 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    start_process(t_philo *philo)
{
    if (pthread_create(&philo->monitoring_thread, NULL, check_for_death, (void *)philo))
        write(STDERR_FILENO, "pthread_create() failed\n", 24);
   
    if (philo->data->number_of_times_each_philosopher_must_eat == 0)
         sem_post(philo->data->sem_ate_enough);

    while (get_current_time() < philo->data->starting_time)
        usleep(100);
    
    if (philo->philo_id % 2)
        usleep(philo->data->time_to_eat * 500);
    
     if (philo->data->philos_count == 1)
         (sem_wait(philo->data->sem_forks), print_state(philo, got_fork),
             usleep(philo->data->time_to_die * 1000), 
                exit_philo(philo));
}

void    philo_process(t_philo *philo, int i)
{
    
    philo->philo_id = i;
    start_process(philo);
    // if (pthread_create(&philo->monitoring_thread, NULL, check_for_death, (void *)philo))
    //     write(STDERR_FILENO, "pthread_create() failed\n", 24);
    // // if (pthread_detach(monitoring_thread))
    // //     write(STDERR_FILENO, "pthread_detach() failed\n", 24);
   
    // if (philo->data->number_of_times_each_philosopher_must_eat == 0)
    //      sem_post(philo->data->sem_ate_enough);

    // //modif
    // while (get_current_time() < philo->data->starting_time)
    //     usleep(100);
    
    // if (i % 2)
    //     usleep(philo->data->time_to_eat * 500);
    
    // // printf("coucou\n");
    //  if (philo->data->philos_count == 1)
    //      (sem_wait(philo->data->sem_forks), print_state(philo, got_fork),
    //          usleep(philo->data->time_to_die * 1000), 
    //             exit_philo(philo));

    while (true)
    {
        take_forks(philo);
        if (check_create_state(philo->data, philo->philo_id))
            exit_philo(philo);
        //self_monitoring(philo, philo->data);
        
        eating_state(philo);
        if (check_create_state(philo->data, philo->philo_id))
            exit_philo(philo);
        //self_monitoring(philo, philo->data);
        drop_forks(philo);

        //printf("%d ok ici\n", philo->philo_id + 1);
        //self_monitoring(philo, philo->data);
        if (check_create_state(philo->data, philo->philo_id))
            exit_philo(philo);
        sleeping_state(philo);

        //self_monitoring(philo, philo->data);
        if (check_create_state(philo->data, philo->philo_id))
            exit_philo(philo);
        thinking_state(philo);
    }
    exit_philo(philo);
}

bool    create_philos(t_philo *philo)
{
    int	i;

	i = 0;
	set_starting_time(philo); 
    while (i < philo->data->philos_count)
    {
        philo->data->pids[i] = fork();
        if (philo->data->pids[i] < 0)
        {
            while (--i)
                waitpid(philo->data->pids[i], NULL, 0);
            free(philo->data->pids);
            return (false);
        }
        if (philo->data->pids[i] == 0)
        {
            // free(data->pids);
            // data->pids = NULL;
            philo_process(philo, i);
        }
        i++;
    }
    return (true);
}
