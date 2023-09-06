/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:19:03 by Helene            #+#    #+#             */
/*   Updated: 2023/09/06 12:53:37 by Helene           ###   ########.fr       */
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

typedef struct  s_data
{
	int				philos_count; // ou unsigned int ?
	/* unsigned long ou autre type de donnée ? */
	suseconds_t	time_to_die;
	suseconds_t	time_to_eat;
	suseconds_t	time_to_sleep;
	long long 	number_of_times_each_philosopher_must_eat; // mettre a -1 si n'a pas ce parametre dans argv
	bool		end_simulation;
	
	/* equal amount of forks and philosophers */
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
	struct timeval	simulation_start;
	pthread_mutex_t meals_count_m;
	pthread_mutex_t last_meal_m;
    struct s_data	*data; 
}               t_philo;

enum	e_state
{
	eating,
	sleeping
};

enum	e_fork
{
	left,
	right,
	both
};

/* ft_usleep */
bool    ft_is_end(t_data *data);
bool    ft_usleep(t_data *data, int state);

/* initialise */
void    init_data_mutexes(t_data *data);
void    init_data(char **program_args, t_data *data);
void    create_threads(t_philo *philos, t_data data);

/* monitoring */
bool    ft_end_simulation(t_data data, t_philo *philos);

/* destroy */
void    join_threads(t_philo *philos);
void    destroy_mutexes(t_philo *philos, t_data data);

/* routine */
// pthread_mutex_t *get_fork(t_philo *philo, int fork_status);
bool    		ft_is_end(t_data *data);
//bool    		end_thread(t_philo *philo, int fork_status);
void    		*philo_routine(void *routine_data);

/* utils */
int		ft_atoi(const char *nb_str);

#endif 