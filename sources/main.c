/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:24:33 by Helene            #+#    #+#             */
/*   Updated: 2023/09/04 19:48:34 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    *thread_routine(void *data)
{
    pthread_t   tid;

    tid = pthread_self();
    printf("coucou du thread %ld\n", tid);

    return (NULL);
}

void    *philo_routine(void *data)
{
    t_philo *philo;
    bool    state;

    philo = data; // ça caste bien ?
    state = true;
    if (philo->philo_id % 2) // ie ceux qui grab leurs fourchettes en deuxieme
        usleep(10); // quelle valeur ?
        // écrire aussi "is thinking" a la mano la premiere fois ici?
    
    while (true) // degueulasse
    {
        /* Eat */
        if (philo->philo_id % 2 == 0) // pair, droitier (choix arbitraire)
        {
            // fork à droite
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id + 1) % philo->data->philos_count]);
            // fork à gauche
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id - 1) % philo->data->philos_count]); // a modifier
            
            pthread_mutex_lock(&philo->data->msg_display);
            printf("%d is eating\n", philo->philo_id + 1);
            pthread_mutex_unlock(&philo->data->msg_display);
            usleep(philo->data->time_to_eat);
            
            pthread_mutex_unlock(&philo->data->forks[(philo->philo_id - 1) % philo->data->philos_count]); // a modifier
            pthread_mutex_unlock(&philo->data->forks[(philo->philo_id + 1) % philo->data->philos_count]);
            pthread_mutex_lock(&philo->meals_count_m);
            philo->meals_count++;
            pthread_mutex_unlock(&philo->meals_count_m);
        }
        else // impair, gauchier
        {
            // fork à gauche
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id - 1) % philo->data->philos_count]); // a modifier
            // fork à droite
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id + 1) % philo->data->philos_count]);
            pthread_mutex_lock(&philo->data->msg_display);
            printf("%d is eating\n", philo->philo_id + 1);
            pthread_mutex_unlock(&philo->data->msg_display);
            
            ft_usleep(philo->data->time_to_eat, philo->data->time_to_die);
        
            pthread_mutex_unlock(&philo->data->forks[(philo->philo_id + 1) % philo->data->philos_count]);
            pthread_mutex_unlock(&philo->data->forks[(philo->philo_id - 1) % philo->data->philos_count]); // a modifier
            pthread_mutex_lock(&philo->meals_count_m);
            philo->meals_count++;
            pthread_mutex_unlock(&philo->meals_count_m);
        }
        
        pthread_mutex_lock(&philo->data->end_simulation_m);
        if (philo->data->end_simulation)
            state = false;
        pthread_mutex_unlock(&philo->data->end_simulation_m);
        
        /* Sleep */
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%d is sleeping\n", philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
        
        usleep(philo->data->time_to_sleep);

        /* Think */
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%d is thinking\n", philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
    }
}


/*
argc[1] : number_of_philosophers ( = number of forks)
argc[2] : time_to_die 
argc[3] : time_to_eat
argc[4] : time_to_sleep
argc[5] : /optional/ number_of_times_each_philosopher_must_eat
*/
int main(int argc, char **argv)
{
    int     philos_nb;
    t_data  data;
    t_philo     *philos;

    philos_nb = atoi(argv[1]); // remplacer par ft_atoi
    init_data(argv + 1, &data);
    /* Create threads */
    philos = (t_philo *)malloc(sizeof(t_philo), philos_nb);
    if (!philos)
        return (EXIT_FAILURE);
    create_threads(philos_nb, philos, data);
    
    /* Monitoring */
    while(!ft_end_simulation(data, philos))
    {
        usleep(10);
    }
    
    join_threads(philos_nb, philos);
    destroy_mutexes(philos_nb, philos, data);
    
}