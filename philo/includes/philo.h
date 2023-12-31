/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:19:03 by Helene            #+#    #+#             */
/*   Updated: 2023/09/25 16:50:18 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <fcntl.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

enum				e_state
{
	eating,
	sleeping,
	thinking,
	got_fork
};

enum				e_fork
{
	left,
	right,
	both
};

enum				e_parsing_error_type
{
	non_numeric,
	negative_philos,
	invalid_amount
};

typedef struct s_data
{
	int				philos_count;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	long			number_of_times_each_philosopher_must_eat;
	bool			end_simulation;
	long long		starting_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	msg_display;
	pthread_mutex_t	end_simulation_m;
}					t_data;

typedef struct s_philo
{
	pthread_t		tid;
	int				philo_id;
	unsigned int	meals_count;
	long long		last_meal_tstamp;
	pthread_mutex_t	meals_count_m;
	pthread_mutex_t	last_meal_m;
	struct s_data	*data;
}					t_philo;

/* initialise */
void				init_data_mutexes(t_data *data);
void				init_data(char **program_args, t_data *data);
void				create_threads(t_philo *philos, t_data data);

/* monitoring */
void				supervise_simulation(t_philo *philos);
bool				ft_end_simulation(t_data data, t_philo *philos);

/* eating */
bool				eating_time(t_philo *philo);
pthread_mutex_t		*get_fork(t_philo *philo, int fork_status);
void				update_last_meal(t_philo *philo);
bool				end_thread(t_philo *philo, int fork_status);

/* routine */
bool				ft_is_end(t_data *data);
bool				end_thread(t_philo *philo, int fork_status);
void				*philo_routine(void *routine_data);
void				*philo_routine_one(void *routine_data);

/* destroy */
void				join_threads(t_philo *philos);
void				destroy_mutexes(t_philo *philos, t_data data);
void				free_all(t_data data, t_philo *philos);

/* utils */
long				ft_atoi(const char *nb_str);
bool				valid_input(int args_nb, char **inputs);
long				get_current_time(t_data *data);
void				ft_usleep(t_data *data, int state);
void				print_state(t_philo *philo, t_data *data, int state);

#endif