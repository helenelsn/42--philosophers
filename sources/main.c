/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:24:33 by Helene            #+#    #+#             */
/*   Updated: 2023/06/13 21:30:20 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    *thread_routine(void *data)
{
    pthread_t   tid;

    tid = pthread_self();
    printf("coucou du thread %ld\n", tid);

    return (NULL);
}

int main(int argc, char *argv)
{
    pthread_t       tid;
    pthread_mutex_t mutex;
    
    pthread_create(&tid, NULL, thread_routine, NULL);   
    pthread_mutex_init(&mutex, NULL);

    pthread_mutex_destroy(&mutex); // détruit un mutex DÉVÉROUILLÉ 
}