/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 20:43:28 by Helene            #+#    #+#             */
/*   Updated: 2023/09/09 23:06:34 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    print_error_msg(char *syscall)
{
    char *msg;
    
    msg = ft_strjoin(syscall, " has failed\n");
    write(STDERR_FILENO, msg, ft_strlen(msg));
    free(msg);
}

void    init_philo(t_philo *philo, char **args)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    philo->philos_count = ft_atoi(args[0]);
    philo->time_to_die = ft_atoi(args[1]);
    philo->time_to_eat = ft_atoi(args[2]);
    philo->time_to_sleep = ft_atoi(args[3]);
    philo->meals_count = 0;
    philo->starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    philo->last_meal_tstamp = 0;
    philo->number_of_times_each_philosopher_must_eat = -1;
    if (args[4])
        philo->number_of_times_each_philosopher_must_eat = ft_atoi(args[4]);
}

bool    sleeping_time(t_philo *philo, sem_t *state_msg)
{
    /* Check if another philo died */
    
    sem_wait(state_msg);
    printf("%ld %d is sleeping\n", get_current_time(philo), philo->philo_id + 1);
    sem_post(state_msg);

    ft_usleep(philo, sleeping);
    
}

bool    thinking_time(t_philo *philo, sem_t *state_msg)
{
    /* Check if another philo died */
    
    sem_wait(state_msg);
    printf("%ld %d is thinking\n", get_current_time(philo), philo->philo_id + 1);
    sem_post(state_msg);

    ft_usleep(philo, thinking);
}

bool    eating_time(t_philo *philo, sem_t *forks, sem_t *state_msg)
{
    /* Check if another philo died */
    
    if (sem_wait(forks) < 0)
        print_error_msg("sem_wait()");
    sem_wait(state_msg);
    printf("%ld %d has taken a fork\n", get_current_time(philo), philo->philo_id + 1);
    sem_post(state_msg);

    /* Check if another philo died */
    
    if (sem_wait(forks) < 0)
        print_error_msg("sem_wait()");
    sem_wait(state_msg);
    printf("%ld %d has taken a fork\n", get_current_time(philo), philo->philo_id + 1);
    sem_post(state_msg);

    /* Check if other philos died */

    /* Update last_meal_tstamp */
    philo->last_meal_tstamp = get_current_time(philo);
    
    sem_wait(state_msg);
    printf("%ld %d is eating\n", get_current_time(philo), philo->philo_id + 1);
    sem_post(state_msg);

    /* Update meals_count */
    (philo->meals_count)++;

    /* Wait time_to_eat ms */
    ft_usleep(philo, eating);
    
    /* Drop forks */
    sem_post(forks);
    sem_post(forks);
}

void    exit_philo(t_philo, sem_t *forks, sem_t *state_msg)
{
    sem_close(forks);
    sem_close(state_msg);
    exit();
}

void    philo_process(t_philo *philo, int i)
{
    sem_t   *forks;
    sem_t   *state_msg;
    philo->philo_id = i;
    /* Open preexisting semaphores in child process */
    forks = sem_open(SEMA_FORKS, 0);
    state_msg = sem_open(SEMA_STREAM, 0);

    while (true)
    {
        /* Eats */
        if (eating_time(philo, forks, state_msg) == false)
            exit_philo(philo, forks, state_msg);

        /* Sleeps */
        if (sleeping_time(philo, state_msg) == false)
            exit_philo(philo, forks, state_msg);

        /* Thinks */
         if (thinking_time(philo, state_msg) == false)
            exit_philo(philo, forks, state_msg);
    }
}

int main(int argc, char **argv)
{
    t_philo philo;
    pid_t   *pids;
    sem_t   *forks;
    sem_t   *state_msg;
    int     i;
    int     wstatus;
    
    wstatus = 0;
    init_philo(&philo, argv + 1);
    pids = (pid_t *)malloc(sizeof(pid_t) * philo.philos_count);
    if (!pids)
        return (1);
    
    /* Create semaphores */
    forks = sem_open(SEMA_FORKS, O_CREAT | O_EXCL, SEMA_MODE, philo.philos_count / 2);
    if (forks == SEM_FAILED)
        return (2);
    state_msg = sem_open(SEMA_STREAM, O_CREAT | O_EXCL, SEMA_MODE, 1);
    
    /* Create child processes, ie philosophers */
    i = 0;
    while (i < philo.philos_count)
    {
        pids[i] = fork();
        if (pids[i] < 0)
            return (3);
        if (pids[i] == 0)
            philo_process(&philo, i);
        else 
        {
            // monitoring ?
        }
        i++;
    }

    /* Wait for childs processes, and unlink semaphores*/
    i = 0;
    while (i < philo.philos_count)
    {
        if (waitpid(pids[i], NULL, 0) < 0)
            write(STDERR_FILENO, "waitpid() failed\n", 17);
        i++;
    }
    if (sem_unlink(SEMA_FORKS) < 0)
        write(STDERR_FILENO, "sem_unlink() failed\n", 20);
    if (sem_unlink(SEMA_STREAM) < 0)
        write(STDERR_FILENO, "sem_unlink() failed\n", 20);
}