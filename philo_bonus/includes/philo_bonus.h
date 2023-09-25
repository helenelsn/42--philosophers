/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 17:42:41 by Helene            #+#    #+#             */
/*   Updated: 2023/09/25 16:41:30 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <errno.h>
# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h> // pour obtenir les définitions des bits de permissions
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define SEMA_MODES 0644    //S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
# define SEMA_FLAGS O_CREAT //| O_EXCL
# define SEMA_FORKS "/philo_forks"
# define SEMA_STREAM "/philo_state_display"
# define SEMA_MEALS "/philo_meals_count"
# define SEMA_LAST_MEAL "/philo_last_meal_timestamp"
# define SEMA_END_MSG "/philo_death_message"

# define SEMA_PROTECT_COUNT 20

enum				e_state
{
	eating,
	sleeping,
	thinking,
	got_fork
};

enum				e_forks
{
	one,
	both
};

enum				e_data_type
{
	sem_name,
	sem
};

enum				e_parsing_error_type
{
	non_numeric,
	negative_philos,
	invalid_amount
};

typedef struct s_data
{
	pthread_t		check_meals;
	int				philos_count;
	unsigned long	starting_time;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			number_of_times_each_philosopher_must_eat;
	bool			stop_sim;
	pid_t			*pids;
	sem_t			*sem_forks;
	sem_t			*sem_state_msg;
	sem_t			*sem_ate_enough;
	sem_t			*sem_end_msg;
	sem_t			**sem_create;
	sem_t			**sem_create_check;
	char			**names_create;
	char			**names_create_check;
}					t_data;

typedef struct s_philo
{
	int				philo_id;
	pthread_t		monitoring_thread;
	unsigned int	meals_count;
	unsigned long	last_meal_tstamp;
	sem_t			*sem_last_meal;
	t_data			*data;
}					t_philo;

/* initialise */
bool				init_data(t_data *data, char **args);
bool				init_philo(t_philo *philo, t_data *data);
bool				allocate_data_memory(t_data *data, int data_type);
void				init_close(t_data *data);
void				unlink_semaphores(long philos_nb);

/* routine */
bool				create_philos(t_philo *philo);
void				philo_process(t_philo *philo, int i);
void				wait_for_starting_time(t_data *data);
void				*check_for_death(void *arg);

/* philosophers states */
void				sleeping_state(t_philo *philo);
void				thinking_state(t_philo *philo);
void				drop_forks(t_philo *philo);
void				take_forks(t_philo *philo);
void				eating_state(t_philo *philo);

/* main monitoring */
void				create_threads(t_data *data, int args);
void				*check_meals_routine(void *data_check);
void				parent_process(t_philo *philo);

/* destroy */
void				close_semaphores(t_philo *philo);
void				free_data(t_data *data);
void				exit_philo(t_philo *philo);
void				join_main_threads(t_data *data, int args_nb);
void				exit_parent(t_philo *philo);

/* parsing */
bool				valid_input(int args_nb, char **inputs);
long				ft_atoi(const char *nb_str);

/* utils */
long long			get_current_time(void);
long long			get_relative_time(t_philo *philo);
void				set_starting_time(t_philo *philo);
void				ft_usleep(t_philo *philo, int state);
void				print_state(t_philo *philo, int state);
void				print_error(int error_type);

/* sem_death_protection */
bool				check_create_state(t_data *data, int n);
void				end_simulation(t_data *data);
bool				exit_main_process(t_data *data);

char				*ft_itoa(int n);
void				*ft_calloc(size_t nmemb, size_t size);

#endif