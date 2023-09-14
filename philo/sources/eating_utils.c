/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 21:38:55 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/14 21:41:32 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	update_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_m);
	philo->last_meal_tstamp = get_current_time(philo->data);
	pthread_mutex_unlock(&philo->last_meal_m);
}

pthread_mutex_t	*get_fork(t_philo *philo, int fork_status)
{
	if (fork_status == left)
	{
		if (philo->philo_id == 0)
			return (&philo->data->forks[philo->data->philos_count - 1]);
		return (&philo->data->forks[(philo->philo_id - 1)]);
	}
	else if (fork_status == right)
		return (&philo->data->forks[philo->philo_id]);
	return (NULL);
}

bool	end_thread(t_philo *philo, int fork_status)
{
	if (!ft_is_end(philo->data))
		return (false);
	if (fork_status == left || fork_status == both)
		pthread_mutex_unlock(get_fork(philo, left));
	if (fork_status == right || fork_status == both)
		pthread_mutex_unlock(get_fork(philo, right));
	return (true);
}
