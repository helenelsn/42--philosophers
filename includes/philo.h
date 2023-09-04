/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:19:03 by Helene            #+#    #+#             */
/*   Updated: 2023/09/04 16:28:42 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>

typedef struct  s_data
{
	int				philos_count;
	/* unsigned long ou autre type de donnée ? */
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long 	number_of_times_each_philosopher_must_eat;
	bool			someone_died;
	
	/* equal amount of forks and philosophers */
	pthread_mutex_t	*forks;
	pthread_mutex_t msg_display;
}               t_data;


typedef struct  s_philo
{
	int				philo_id; // de 0 à [philos_count - 1]
	int				meals_count;
	int				last_meal_tstamp; // ?
	pthread_t		tid;
    struct s_data	*data; 
}               t_philo;

#endif 