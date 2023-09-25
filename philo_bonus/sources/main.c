/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 20:43:28 by Helene            #+#    #+#             */
/*   Updated: 2023/09/25 13:51:37 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	main(int argc, char **argv)
{
	t_philo	philo;
	t_data	data;

	if (!valid_input(argc - 1, argv + 1))
		return (1);
	if (init_data(&data, argv + 1) == false || init_philo(&philo,
			&data) == false)
		return (1);
	// data.pids = (pid_t *)malloc(sizeof(pid_t) * data.philos_count);
	// if (!data.pids)
	// 	return (2);
	if (create_philos(&philo) == false)
		return (3);
	create_threads(&data, argc - 1);
	parent_process(&philo);
	//printf("main, ok ici\n");
	join_main_threads(&data, argc - 1);
	exit_parent(&philo);
	return (0);
}
