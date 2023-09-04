/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:24:33 by Helene            #+#    #+#             */
/*   Updated: 2023/09/04 16:40:47 by Helene           ###   ########.fr       */
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

    philo = data; // ça caste bien ?
    if (philo->philo_id % 2) // ie ceux qui grab leurs fourchettes en deuxieme
        usleep(10); // quelle valeur ?
        // écrire aussi "is thinking" a la mano la premiere fois ici?
    
    // avoir plutot une variable someone_died dans t_data, qui est mise a un des qu'un philo est mort,
    // et c'est cette variable qui va servir de base pour le monotoring
    while (philo->last_meal_tstamp < philo->data->time_to_die) // rajouter la condition de l'argument optionnel
    {
        /* Eat */
        if (philo->philo_id % 2 == 0) // pair, droitier (choix arbitraire)
        {
            // fork à droite
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id + 1) % philo->data->philos_count]);
            // fork à gauche
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id - 1) % philo->data->philos_count]); // a modifier
            
            pthread_mutex_lock(&philo->data->msg_display);
            // print eating msg
            printf("%d is eating\n", philo->philo_id + 1);
            pthread_mutex_unlock(&philo->data->msg_display);
            usleep(philo->data->time_to_eat);
            
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id + 1) % philo->data->philos_count]);
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id - 1) % philo->data->philos_count]); // a modifier
            philo->meals_count++;
        }
        else // impair, gauchier
        {
            // fork à gauche
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id - 1) % philo->data->philos_count]); // a modifier
            // fork à droite
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id + 1) % philo->data->philos_count]);

            pthread_mutex_lock(&philo->data->msg_display);
            // print eating msg
            printf("%d is eating\n", philo->philo_id + 1);
            pthread_mutex_unlock(&philo->data->msg_display);
            usleep(philo->data->time_to_eat);
            
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id - 1) % philo->data->philos_count]); // a modifier
            pthread_mutex_lock(&philo->data->forks[(philo->philo_id + 1) % philo->data->philos_count]);
            philo->meals_count++;
        }
        
        /* Sleep */
        pthread_mutex_lock(&philo->data->msg_display);
        // print sleeping msg
        printf("%d is sleeping\n", philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
        usleep(philo->data->time_to_sleep);

        /* Think */
        pthread_mutex_lock(&philo->data->msg_display);
        // print thinkiing msg
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
int main(int argc, char *argv)
{
    int     philos_nb;
    t_data  data;

    philos_nb = atoi(argv[1]); // remplacer par ft_atoi
    
    /* Initialise forks' mutexes*/
    int i = 0;
    int err;
    while (i < philos_nb)
    {
        err = pthread_mutex_init(&data.forks[i], NULL);
        if (err)
            write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);
            // return ? 
        i++;
    }
     err = pthread_mutex_init(&data.msg_display, NULL);
        if (err)
            write(STDERR_FILENO, "pthread_mutex_init() failed\n", 28);

    /* Create threads */
    t_philo     *philos;
    philos = (t_philo *)malloc(sizeof(t_philo), philos_nb);
    if (!philos)
        return (EXIT_FAILURE);
        
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

    /* Monitoring */
    while(!data.someone_died)
    {
        // ?
    }
    
    while (i < philos_nb)
    {
        err = pthread_join(philos[i].tid, NULL);
        if (err)
            write(STDERR_FILENO, "pthread_join() failed\n", 22);
            // return ?    
        i++;
    }

    while (i < philos_nb)
    {
        err = pthread_mutex_destroy(&data.forks[i]);
        if (err)
            write(STDERR_FILENO, "pthread_mutex_destroy() failed\n", 31);
            // return ? 
        i++;
    }
     err = pthread_mutex_destroy(&data.msg_display);
        if (err)
            write(STDERR_FILENO, "pthread_mutex_destroy() failed\n", 28);
    
    // t_philo     *philosophers;
    // philosophers = (t_philo *)malloc(sizeof(t_philo) * ());
    
    /*
    pthread_create(&tid, NULL, thread_routine, NULL);   
    pthread_mutex_init(&mutex, NULL);

    pthread_mutex_destroy(&mutex); // détruit un mutex DÉVÉROUILLÉ
    */
}