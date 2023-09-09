/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 17:42:41 by Helene            #+#    #+#             */
/*   Updated: 2023/09/09 22:59:31 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> // pour obtenir les définitions des bits de permissions
#include <semaphore.h>

# define SEMA_MODE		S_IRUSR | S_IWUSR
# define SEMA_FORKS 	"/philo_forks"
# define SEMA_STREAM	"/philo_state_display"

enum	e_state
{
	eating,
	sleeping,
	thinking // useful for the print_action() function
};

typedef struct  s_philo
{
	int				philo_id; // de 0 à [philos_count - 1]
    int				philos_count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long 			number_of_times_each_philosopher_must_eat;
	unsigned int	meals_count;
	unsigned long 	starting_time;
	unsigned long	last_meal_tstamp;
}               t_philo;

/* utils */
long	get_current_time(t_philo *philo);
void    ft_usleep(t_philo *philo, int state) ;

#endif