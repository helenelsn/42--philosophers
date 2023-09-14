/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 17:42:41 by Helene            #+#    #+#             */
/*   Updated: 2023/09/14 23:47:31 by hlesny           ###   ########.fr       */
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
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h> // pour obtenir les définitions des bits de permissions
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define SEMA_MODES 0644 //S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH 
# define SEMA_FLAGS O_CREAT //| O_EXCL
# define SEMA_FORKS "/philo_forks"
# define SEMA_STREAM "/philo_state_display"
# define SEMA_MEALS "/philo_meals_count"
# define SEMA_DEATH "/philo_one_died"
# define SEMA_LAST_MEAL "/philo_last_meal_timestamp"
# define SEMA_END "/philo_end_simulation"
# define SEMA_END_MSG "/philo_death_message"

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

typedef struct s_data
{
	pthread_t		check_death;
	pthread_t		check_meals;
	int				philos_count;
	bool			stop_sim;
	pid_t			*pids;
	sem_t			*sem_forks;
	sem_t			*sem_state_msg;
	sem_t			*sem_ate_enough;
	sem_t			*sem_one_died;
	sem_t			*sem_end;
	sem_t			*sem_end_msg;

}					t_data;

typedef struct s_philo
{
	int				philo_id;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			number_of_times_each_philosopher_must_eat;
	unsigned int	meals_count;
	unsigned long	starting_time;
	unsigned long	last_meal_tstamp;
	sem_t			*sem_last_meal;
	t_data			*data;
}					t_philo;

/* initialise */
void				set_starting_time(t_philo *philo);
bool				init_data(t_data *data, char *count);
bool				init_philo(t_philo *philo, t_data *data, char **args);
void				unlink_semaphores(void);

/* routine */
bool				create_philos(t_data *data, t_philo *philo);
void				philo_process(t_philo *philo, t_data *data, int i);
void				self_monitoring(t_philo *philo, t_data *data,
						pthread_t *philo_monitor);
void				*check_for_death(void *arg);

/* main monitoring */
void				create_threads(t_data *data, int args);
void				*check_meals_routine(void *data_check);
void				parent_process(t_philo *philo, t_data *data);

/* destroy */
void				kill_processes(t_data *data);
void				exit_philo(t_philo *philo, t_data *data,
						pthread_t *philo_monitor);
void				join_main_threads(t_data *data, int args_nb);
void				exit_parent(t_philo *philo, t_data *data);

/* utils */
bool				valid_input(int args_nb, char **inputs);
long				ft_atoi(const char *nb_str);
long				get_current_time(t_philo *philo);
void				ft_usleep(t_philo *philo, t_data *data, int state,
						pthread_t *philo_monitor);
void				print_state(t_philo *philo, t_data *data, int state);

#endif