/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 22:57:42 by Helene            #+#    #+#             */
/*   Updated: 2023/09/14 20:11:56 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

bool    valid_input(int args_nb, char **inputs)
{
    int	i;
	int	j;

    if (args_nb < 4 || args_nb > 5)
    {
        write(STDERR_FILENO, "This program requires between 4 and 5 arguments!\n", 49);
        return (false);
    }
	i = 0;
	while (i < args_nb)
	{
		j = 0;
        if (inputs[i][j] == '+')
            j++;
		while (inputs[i][j])
		{
			if (inputs[i][j] < '0' || inputs[i][j] > '9')
			{
                write(STDERR_FILENO, "This program requires numeric and non-negative arguments!\n", 58);
                return (false);
            }
			j++;
		}
		i++;
	}
	return (true);
}

void    print_state(t_philo *philo, t_data *data, int state)
{
    // if (sem_open(SEMA_END, 0) != SEM_FAILED)    
    //     return;
    sem_wait(data->sem_state_msg);
    if (sem_open(SEMA_END, 0) != SEM_FAILED)    
    {
        sem_post(data->sem_state_msg);
        return;
    }
     printf("%ld %d ", get_current_time(philo), philo->philo_id + 1);
    if (state == eating)
        printf("is eating\n");
    else if (state == sleeping)
        printf("is sleeping\n");
    else if (state == thinking) 
        printf("is thinking\n");
    else if (state == got_fork)
        printf("has taken a fork\n");
    sem_post(data->sem_state_msg);
}

long	get_current_time(t_philo *philo)
{
	struct timeval curr_tv;

	gettimeofday(&curr_tv, NULL);
	return ((curr_tv.tv_sec * 1000 + curr_tv.tv_usec / 1000) - philo->starting_time);
}

/*
States :    0 when eating
            1 when sleeping
            2 when thinking
*/
void    ft_usleep(t_philo *philo, t_data *data, int state, pthread_t *philo_monitor) 
{
    unsigned int    state_length;
    
    state_length = get_current_time(philo) + philo->time_to_eat;
    if (state == sleeping)
        state_length = get_current_time(philo) + philo->time_to_sleep;
    else if (state == thinking)
        state_length = get_current_time(philo) + ((philo->time_to_die - philo->time_to_eat - philo->time_to_sleep) / 2);
    
    while (get_current_time(philo) < state_length)
    {
        if (sem_open(SEMA_END, 0) != SEM_FAILED)
        {
            //printf("%ld ---------------------- philo %d in ft_usleep, about to exit in state %d\n", get_current_time(philo), philo->philo_id + 1, state);
            exit_philo(philo, data, philo_monitor);
        }
        usleep(5);
    }
}