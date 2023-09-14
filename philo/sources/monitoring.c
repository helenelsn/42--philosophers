/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 18:03:41 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/14 21:33:26 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* Check if a philosopher died */
bool	check_hunger(t_philo *philos)
{
	int				i;
	unsigned long	last_meal;

	i = 0;
	last_meal = 0;
	while (i < philos->data->philos_count)
	{
		pthread_mutex_lock(&philos[i].last_meal_m);
		last_meal = philos[i].last_meal_tstamp;
		pthread_mutex_unlock(&philos[i].last_meal_m);
		if ((get_current_time(philos->data)
				- last_meal) >= philos->data->time_to_die)
		{
			pthread_mutex_lock(&philos->data->msg_display);
			printf("%ld %d died\n", get_current_time(philos->data), i + 1);
			pthread_mutex_unlock(&philos->data->msg_display);
			return (true);
		}
		i++;
	}
	return (false);
}

/* Check if all the philosophers ate enough */
bool	check_meals_count(t_philo *philos)
{
	int	i;
	int	ate_enough;

	i = 0;
	ate_enough = 0;
	if (philos->data->number_of_times_each_philosopher_must_eat > -1)
	{
		while (i < philos->data->philos_count)
		{
			pthread_mutex_lock(&philos[i].meals_count_m);
			if (philos[i].meals_count
				>= philos->data->number_of_times_each_philosopher_must_eat)
					ate_enough++;
			pthread_mutex_unlock(&philos[i].meals_count_m);
			i++;
		}
		if (ate_enough == philos->data->philos_count)
			return (true);
	}
	return (false);
}

void	supervise_simulation(t_philo *philos)
{
	bool	end_simulation;

	end_simulation = false;
	while (end_simulation == false)
	{
		end_simulation = (check_hunger(philos) | check_meals_count(philos));
		if (end_simulation)
		{
			pthread_mutex_lock(&philos->data->end_simulation_m);
			philos->data->end_simulation = true;
			pthread_mutex_unlock(&philos->data->end_simulation_m);
		}
		usleep(10);
	}
	return ;
}
