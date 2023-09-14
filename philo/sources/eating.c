/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:00:23 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/14 21:44:45 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	right_handed(t_philo *philo)
{
	pthread_mutex_lock(get_fork(philo, right));
	if (end_thread(philo, right))
		return (false);
	pthread_mutex_lock(&philo->data->msg_display);
	printf("%ld %d has taken a fork\n", get_current_time(philo->data),
		philo->philo_id + 1);
	pthread_mutex_unlock(&philo->data->msg_display);
	pthread_mutex_lock(get_fork(philo, left));
	return (true);
}

bool	left_handed(t_philo *philo)
{
	pthread_mutex_lock(get_fork(philo, left));
	if (end_thread(philo, left))
		return (false);
	pthread_mutex_lock(&philo->data->msg_display);
	printf("%ld %d has taken a fork\n", get_current_time(philo->data),
		philo->philo_id + 1);
	pthread_mutex_unlock(&philo->data->msg_display);
	pthread_mutex_lock(get_fork(philo, right));
	return (true);
}

bool	take_forks(t_philo *philo)
{
	if (ft_is_end(philo->data))
		return (false);
	if (philo->philo_id % 2 == 0)
	{
		if (right_handed(philo) == false)
			return (false);
	}
	else if (left_handed(philo) == false)
		return (false);
	if (end_thread(philo, both))
		return (false);
	return (true);
}

void	drop_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(get_fork(philo, left));
		pthread_mutex_unlock(get_fork(philo, right));
	}
	else
	{
		pthread_mutex_unlock(get_fork(philo, right));
		pthread_mutex_unlock(get_fork(philo, left));
	}
}

bool	eating_time(t_philo *philo)
{
	if (take_forks(philo) == false)
		return (NULL);
	print_state(philo, philo->data, got_fork);
	if (end_thread(philo, both))
		return (false);
	print_state(philo, philo->data, eating);
	update_last_meal(philo);
	if (end_thread(philo, both))
		return (false);
	ft_usleep(philo->data, eating);
	drop_forks(philo);
	if (ft_is_end(philo->data))
		return (false);
	pthread_mutex_lock(&philo->meals_count_m);
	philo->meals_count++;
	pthread_mutex_unlock(&philo->meals_count_m);
	return (true);
}
