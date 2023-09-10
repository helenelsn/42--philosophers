/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 17:42:41 by Helene            #+#    #+#             */
/*   Updated: 2023/09/10 23:52:23 by hlesny           ###   ########.fr       */
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

# define SEMA_MODES		S_IRUSR | S_IWUSR
# define SEMA_FLAGS		O_CREAT | O_EXCL
# define SEMA_FORKS 	"/philo_forks"
# define SEMA_STREAM	"/philo_state_display"
# define SEMA_MEALS		"/philo_meals_count"
# define SEMA_DEATH 	"/philo_one_died"


enum	e_state
{
	eating,
	sleeping,
	thinking, // useful for the print_action() function
	fork
};

enum 	e_forks
{
	one,
	both 
};

typedef struct  s_philo
{
	int				philo_id; // de 0 à [philos_count - 1]
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long 			number_of_times_each_philosopher_must_eat;
	unsigned int	meals_count;
	unsigned long 	starting_time;
	unsigned long	last_meal_tstamp;
	sem_t 			*last_meal_sem;
}               t_philo;

typedef struct 	s_data
{
	pthread_t	check_death;
	pthread_t 	check_meals;
	int			philos_count;
	pid_t 		*pids;
	sem_t   	*forks;
    sem_t   	*state_msg;
	sem_t 		*ate_enough;
	sem_t 		*one_died;
	
}				t_data;

/* initialise */
void    init_data(t_data *data, char *count);

/* utils */
long	get_current_time(t_philo *philo);
void    ft_usleep(t_philo *philo, int state) ;
int		ft_atoi(const char *nb_str);

#endif