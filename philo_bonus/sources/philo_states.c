/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_states.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 12:53:08 by Helene            #+#    #+#             */
/*   Updated: 2023/09/25 15:48:22 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	sleeping_state(t_philo *philo)
{
	print_state(philo, sleeping);
	ft_usleep(philo, sleeping);
}

void	thinking_state(t_philo *philo)
{
	usleep(500);
	print_state(philo, thinking);
	ft_usleep(philo, thinking);
}

void	drop_forks(t_philo *philo)
{
	sem_post(philo->data->sem_forks);
	sem_post(philo->data->sem_forks);
}

void	take_forks(t_philo *philo)
{
	sem_wait(philo->data->sem_forks);
	if (check_create_state(philo->data, philo->philo_id))
	{
		sem_post(philo->data->sem_forks);
		exit_philo(philo);
	}
	print_state(philo, got_fork);
	sem_wait(philo->data->sem_forks);
	if (check_create_state(philo->data, philo->philo_id))
	{
		drop_forks(philo);
		exit_philo(philo);
	}
	print_state(philo, got_fork);
}

void	eating_state(t_philo *philo)
{
	sem_wait(philo->sem_last_meal);
	philo->last_meal_tstamp = get_relative_time(philo);
	sem_post(philo->sem_last_meal);
	print_state(philo, eating);
	ft_usleep(philo, eating);
	if (philo->data->number_of_times_each_philosopher_must_eat != -1
		&& ++(philo->meals_count)
		== philo->data->number_of_times_each_philosopher_must_eat)
		sem_post(philo->data->sem_ate_enough);
}
