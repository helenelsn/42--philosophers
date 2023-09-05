/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 13:22:20 by Helene            #+#    #+#             */
/*   Updated: 2023/09/05 13:22:56 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

pthread_mutex_t *get_fork(t_philo *philo, int fork_status)
{
    if (fork_status == left)
        return (&philo->data->forks[(philo->philo_id - 1) % philo->data->philos_count]); // a modifier ?
    else if (fork_status == right)
        return (&philo->data->forks[(philo->philo_id + 1) % philo->data->philos_count]);
    return (NULL);
}

bool    ft_is_end(t_data *data)
{
    bool    is_end;

    is_end = false;
    pthread_mutex_lock(&data->end_simulation_m);
    is_end = data->end_simulation;
    pthread_mutex_unlock(&data->end_simulation_m);
    return (is_end);   
}

bool    end_thread(t_philo *philo, int fork_status)
{
    if (!ft_is_end(philo->data))
        return (false);
    if (fork_status == left || fork_status == both)
        pthread_mutex_unlock(get_fork(philo, left));
    if (fork_status == right || fork_status == both)
        pthread_mutex_unlock(get_fork(philo, right));
    return (true);
}

void    *philo_routine(void *routine_data)
{
    t_philo *philo;
    bool    philo_state;

    philo = routine_data; // ça caste bien ?
    philo_state = true;
    if (philo->philo_id % 2) // ie ceux qui grab leurs fourchettes en deuxieme
        usleep(10); // quelle valeur ?
        // écrire aussi "is thinking" a la mano la premiere fois ici?
    
    while (true) // bof
    {
        /* Eat */
        if (philo->philo_id % 2 == 0) // pair, droitier (choix arbitraire)
        {
            if (ft_is_end(philo->data))
                return ;
            // fork à droite
            pthread_mutex_lock(get_fork(philo, right));
            if (end_thread(philo, right))
                return ;
            // fork à gauche
            pthread_mutex_lock(get_fork(philo, left));
        }
        else // impair, gauchier
        {
            if (ft_is_end(philo->data))
                return ;
            // fork à gauche
            pthread_mutex_lock(get_fork(philo, left));
            if (end_thread(philo, left))
                return ;
            // fork à droite
            pthread_mutex_lock(get_fork(philo, right));
        }
        if (end_thread(philo, both))
            return ;
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%d is eating\n", philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
        philo_state = ft_usleep(philo->data, eating);
        if (philo->philo_id % 2 == 0)
        {
            pthread_mutex_unlock(get_fork(philo, left));
            pthread_mutex_unlock(get_fork(philo, right));
        }
        else
        {
            pthread_mutex_unlock(get_fork(philo, right));
            pthread_mutex_unlock(get_fork(philo, left));
        }
        if (philo_state)
            return ;
        pthread_mutex_lock(&philo->meals_count_m);
        philo->meals_count++;
        pthread_mutex_unlock(&philo->meals_count_m);
        
        /* Sleep */
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%d is sleeping\n", philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
        
        philo_state = ft_usleep(philo->data, sleeping);
        if (philo_state)
            return ;

        /* Think */
        pthread_mutex_lock(&philo->data->msg_display);
        printf("%d is thinking\n", philo->philo_id + 1);
        pthread_mutex_unlock(&philo->data->msg_display);
    }
}