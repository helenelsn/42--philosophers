/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:19:03 by Helene            #+#    #+#             */
/*   Updated: 2023/09/04 19:09:38 by hlesny           ###   ########.fr       */
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
	int				philos_count; // ou unsigned int ?
	/* unsigned long ou autre type de donnée ? */
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	long long 		number_of_times_each_philosopher_must_eat; // mettre a -1 si n'a pas ce parametre dans argv
	bool			end_simulation;
	
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
	pthread_mutex_t meals_count_m;
	int				last_meal_tstamp;
	pthread_mutex_t last_meal_m;
    struct s_data	*data; 
}               t_philo;

/* ft_usleep */
int    ft_usleep(unsigned long time_to_eat, unsigned long time_to_die);

/* initialise */
void    init_mutexes(int philos_nb, t_data data, t_philo *philos);
void    init_data(char **program_args, t_data *data);
void    create_threads(int philos_nb, t_philo *philos, t_data data);

/* monitoring */
bool    ft_end_simulation(t_data data, t_philo *philos);

/* destroy */
void    join_threads(int philos_nb, t_philo *philos);
void    destroy_mutexes(int philos_nb, t_philo *philos, t_data data);



#endif 