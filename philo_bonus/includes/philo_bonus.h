/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 17:42:41 by Helene            #+#    #+#             */
/*   Updated: 2023/09/12 01:33:42 by Helene           ###   ########.fr       */
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
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h> // pour obtenir les définitions des bits de permissions
#include <semaphore.h>

# define SEMA_MODES		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
# define SEMA_FLAGS		O_CREAT //| O_EXCL
# define SEMA_FORKS 	"/philo_forks"
# define SEMA_STREAM	"/philo_state_display"
# define SEMA_MEALS		"/philo_meals_count"
# define SEMA_DEATH 	"/philo_one_died"
# define SEMA_LAST_MEAL	"/philo_last_meal_timestamp"


enum	e_state
{
	eating,
	sleeping,
	thinking, // useful for the print_action() function
	got_fork
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
	sem_t 			*sem_last_meal;
}               t_philo;

typedef struct 	s_data
{
	pthread_t	check_death;
	pthread_t 	check_meals;
	int			philos_count;
	bool 		stop_sim;
	pid_t 		*pids;
	sem_t		*sem_forks;
    sem_t		*sem_state_msg;
	sem_t		*sem_ate_enough;
	sem_t		*sem_one_died;
	
}				t_data;

/* initialise */
bool    init_data(t_data *data, char *count);
bool    init_philo(t_philo *philo, char **args);

/* routine */
void    self_monitoring(t_philo *philo, t_data *data);
void    philo_process(t_philo *philo, t_data *data, int i);

/* main monitoring */
void    *check_meals_routine(void *data_check);
void    *check_death_routine(void *data_check);
void    parent_process(t_philo *philo, t_data *data);

/* destroy */
void    kill_processes(t_data *data);
void    exit_philo(t_philo *philo, t_data *data);
void    exit_parent(t_philo *philo, t_data *data);

/* utils */
long	get_current_time(t_philo *philo);
void    ft_usleep(t_philo *philo, t_data *data, int state) ;
int		ft_atoi(const char *nb_str);
void    print_state(t_philo *philo, t_data *data, int state);

#endif