/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:42:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/10 23:49:45 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    exit_philo(t_philo *philo, t_data *data)
{
    sem_close(data->ate_enough);
    sem_close(data->forks);
    sem_close(data->one_died);
    sem_close(data->state_msg);
    sem_close(philo->last_meal_sem);
    free(data->pids);
    data->pids = NULL;
    exit(1);
}

void    unlink_semaphores(t_data *data)
{
    if (sem_unlink(SEMA_FORKS) < 0)
        write(STDERR_FILENO, "sem_unlink() failed\n", 20);
    if (sem_unlink(SEMA_STREAM) < 0)
        write(STDERR_FILENO, "sem_unlink() failed\n", 20);
}