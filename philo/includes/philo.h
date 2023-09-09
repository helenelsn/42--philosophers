/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:19:03 by Helene            #+#    #+#             */
/*   Updated: 2023/09/09 21:34:20 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>

#include <string.h>

typedef struct  s_data
{
	int				philos_count;
	/* Quel type de données ? */
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long 			number_of_times_each_philosopher_must_eat;
	bool			end_simulation;
	unsigned long 	starting_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t msg_display;
	pthread_mutex_t	end_simulation_m;
}               t_data;


typedef struct  s_philo
{
	pthread_t		tid;
	int				philo_id; // de 0 à [philos_count - 1]
	unsigned int	meals_count;
	unsigned long	last_meal_tstamp;
	pthread_mutex_t meals_count_m;
	pthread_mutex_t last_meal_m;
    struct s_data	*data; 
}               t_philo;

enum	e_state
{
	eating,
	sleeping,
	thinking // useful for the print_action() function
};

enum	e_fork
{
	left,
	right,
	both
};

/* initialise */
void    init_data_mutexes(t_data *data);
void    init_data(char **program_args, t_data *data);
void    create_threads(t_philo *philos, t_data data);

/* monitoring */
void    supervise_simulation(t_philo *philos);
bool    ft_end_simulation(t_data data, t_philo *philos);

/* eating */
bool    eating_time(t_philo *philo);

/* routine */
bool    ft_is_end(t_data *data);
bool    end_thread(t_philo *philo, int fork_status);
void    *philo_routine(void *routine_data);

/* destroy */
void    join_threads(t_philo *philos);
void    destroy_mutexes(t_philo *philos, t_data data);
void    free_all(t_data data, t_philo *philos);

/* utils */
long	get_current_time(t_data *data);
void    ft_usleep(t_data *data, int state);
int		ft_atoi(const char *nb_str);

#endif