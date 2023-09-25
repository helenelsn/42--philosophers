/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 22:57:42 by Helene            #+#    #+#             */
/*   Updated: 2023/09/25 16:32:04 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	print_state(t_philo *philo, int state)
{
	sem_wait(philo->data->sem_state_msg);
	if (check_create_state(philo->data, philo->philo_id))
	{
		sem_post(philo->data->sem_state_msg);
		return ;
	}
	printf("%lld %d ", get_relative_time(philo), philo->philo_id + 1);
	if (state == eating)
		printf("is eating\n");
	else if (state == sleeping)
		printf("is sleeping\n");
	else if (state == thinking)
		printf("is thinking\n");
	else if (state == got_fork)
		printf("has taken a fork\n");
	sem_post(philo->data->sem_state_msg);
}

long long	get_current_time(void)
{
	struct timeval	curr_tv;

	gettimeofday(&curr_tv, NULL);
	return (curr_tv.tv_sec * 1000 + curr_tv.tv_usec / 1000);
}

long long	get_relative_time(t_philo *philo)
{
	return (get_current_time() - philo->data->starting_time);
}

void	set_starting_time(t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	philo->data->starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000
		+ 10 * philo->data->philos_count;
}

void	ft_usleep(t_philo *philo, int state)
{
	unsigned int	state_length;

	state_length = get_relative_time(philo) + philo->data->time_to_eat;
	if (state == sleeping)
		state_length = get_relative_time(philo) + philo->data->time_to_sleep;
	else if (state == thinking)
		state_length = get_relative_time(philo) + ((philo->data->time_to_die
					- philo->data->time_to_eat - philo->data->time_to_sleep)
				/ 2);
	while (get_relative_time(philo) < state_length)
	{
		if (check_create_state(philo->data, philo->philo_id))
			exit_philo(philo);
		usleep(5);
	}
}
