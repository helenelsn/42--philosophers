/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 22:57:42 by Helene            #+#    #+#             */
/*   Updated: 2023/09/14 17:21:54 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
    pthread_mutex_lock(&data->msg_display);
    if (ft_is_end(data))
    {
        pthread_mutex_unlock(&data->msg_display);
        return ;
    }
    printf("%ld %d ", get_current_time(data), philo->philo_id + 1);
    if (state == eating)
        printf("is eating\n");
    else if (state == sleeping)
        printf("is sleeping\n");
    else if (state == thinking) 
        printf("is thinking\n");
    else if (state == got_fork)
        printf("has taken a fork\n");
    pthread_mutex_unlock(&data->msg_display);
}

long	get_current_time(t_data *data)
{
	struct timeval curr_tv;

	gettimeofday(&curr_tv, NULL);
	return ((curr_tv.tv_sec * 1000 + curr_tv.tv_usec / 1000) - data->starting_time);
}

/*
Gerer le cas ou, si un philo doit manger pendant 5h mais meurt
s'il n'a pas fini de manger au bout de 2h, 
le ft_usleep ne doit durer que 2h

States :    0 when eating
            1 when sleeping
*/
void    ft_usleep(t_data *data, int state) 
{
    unsigned int    state_length;
    
    state_length = get_current_time(data) + data->time_to_eat;
    if (state == sleeping)
        state_length = get_current_time(data) + data->time_to_sleep;
    while (get_current_time(data) < state_length)
    {
        if (ft_is_end(data))
            return ;
        usleep(100);
    }
}