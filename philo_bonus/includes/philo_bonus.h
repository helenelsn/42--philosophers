/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 17:42:41 by Helene            #+#    #+#             */
/*   Updated: 2023/09/14 17:35:46 by Helene           ###   ########.fr       */
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
# define SEMA_ONLY_ONE	"/philo_one_at_a_time"
# define SEMA_STREAM	"/philo_state_display"
# define SEMA_MEALS		"/philo_meals_count"
# define SEMA_DEATH 	"/philo_one_died"
# define SEMA_MONITOR 	"/philo_global_monitor"
# define SEMA_LAST_MEAL	"/philo_last_meal_timestamp"
# define SEMA_END 		"/philo_end_simulation"


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

typedef struct 	s_data
{
	pthread_t	check_death;
	pthread_t 	check_meals;
	int			philos_count;
	bool 		stop_sim;
	pid_t 		*pids;
	sem_t		*sem_forks;
    sem_t		*sem_monitor;
	sem_t 		*sem_state_msg;
	sem_t		*sem_ate_enough;
	sem_t		*sem_one_died;
	sem_t 		*sem_end;
	
}				t_data;

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
	sem_t 			*sem_last_meal; // pas utile si n'a pas un thread par philo, qui monitore le philo
	t_data 			*data;
}               t_philo;

/* initialise */
void    set_starting_time(t_philo *philo);
bool    init_data(t_data *data, char *count);
bool    init_philo(t_philo *philo, t_data *data, char **args);
void    unlink_semaphores(void);

/* routine */
bool    create_philos(t_data *data, t_philo *philo);
void    philo_process(t_philo *philo, t_data *data, int i);
void    self_monitoring(t_philo *philo, t_data *data, pthread_t *philo_monitor);
void    *check_for_death(void *arg);

/* main monitoring */
void    create_threads(t_data *data, int args);
void    *check_meals_routine(void *data_check);
void    *check_death_routine(void *data_check);
void    parent_process(t_philo *philo, t_data *data);

/* destroy */
void    kill_processes(t_data *data);
void    exit_philo(t_philo *philo, t_data *data, pthread_t *philo_monitor);
void 	join_main_threads(t_data *data, int args_nb);
void    exit_parent(t_philo *philo, t_data *data);

/* utils */
bool    valid_input(int args_nb, char **inputs);
int		ft_atoi(const char *nb_str);
long	get_current_time(t_philo *philo);
void    ft_usleep(t_philo *philo, t_data *data, int state, pthread_t *philo_monitor) ;
void    print_state(t_philo *philo, t_data *data, int state);

#endif