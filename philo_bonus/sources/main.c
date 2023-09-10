/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 20:43:28 by Helene            #+#    #+#             */
/*   Updated: 2023/09/10 23:57:54 by hlesny           ###   ########.fr       */
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

bool    sleeping_state(t_philo *philo, t_data *data)
{
    print_state(philo, data, sleeping);
    ft_usleep(philo, sleeping);
}

bool    thinking_state(t_philo *philo, t_data *data)
{
    print_state(philo, data, thinking);
    ft_usleep(philo, thinking);
}

void    self_monitoring_routine(t_philo *philo, t_data *data)
{
    unsigned long   last_meal;
    
    while (true)
    {
        sem_wait(philo->last_meal_sem);
        last_meal = philo->last_meal_tstamp;
        sem_post(philo->last_meal_sem);
        
        if (philo->meals_count == philo->number_of_times_each_philosopher_must_eat)
            sem_post(data->ate_enough); // rajouter une securite car sinon un meme philo pourra decrementer la valeur plusieurs fois   
        if (get_current_time(philo) >= last_meal + philo->time_to_die)
        {
            sem_post(data->one_died);
            exit_philo(philo);
            return ;
        }
    }
}

void    print_state(t_philo *philo, t_data *data, int state)
{
    sem_wait(data->state_msg);
    printf("%ld %d ", get_current_time(philo), philo->philo_id);
    if (state == eating)
        printf("is eating\n");
    else if (state == sleeping)
        printf("is sleeping\n");
    else if (state == thinking) 
        printf("is thinking\n");
    else if (state == fork)
        printf("has taken a fork\n");
    sem_post(data->state_msg);
}

void    take_forks(t_philo *philo, t_data *data)
{
    while (data->forks->__align < 2) /* check le retour du monitoring en meme temps ?*/
        usleep(100);
    sem_wait(data->forks);
    print_state(philo, data, fork);
    sem_wait(data->forks);
    print_state(philo, data, fork);
}

void    eating_state(t_philo *philo, t_data *data)
{
    /* update last meal timestamp */
    sem_wait(philo->last_meal_sem);
    philo->last_meal_tstamp = get_current_time(philo);
    sem_post(philo->last_meal_sem);

    print_state(philo, data, eating);
    ft_usleep(philo, eating);
}

void    drop_forks(t_philo *philo, t_data *data)
{
    sem_post(data->forks);
    sem_post(data->forks);
}

void    philo_process(t_philo *philo, t_data *data, int i)
{
    philo->philo_id = i;
    /* Open preexisting semaphores in child process */
    /* forks = sem_open(SEMA_FORKS, 0);
    state_msg = sem_open(SEMA_STREAM, 0); */ // pas la peine ? 

    while (true)
    {
        take_forks(philo, data);
        eating_state(philo, data);
        drop_forks(philo, data);

        sleeping_state(philo, data);
        
        thinking_state(philo, data);
    }
}

void    check_death_routine(t_data *data)
{
    
}

void    check_meals_routine(t_data *data)
{
    
}

void    create_threads(t_data *data)
{
    int err;

    err = pthread_create(&data->check_death, NULL, check_death_routine, (void *)data);
    if (err)
        write(STDERR_FILENO, "pthread_create() failed\n", 24);
    err = pthread_create(&data->check_meals, NULL, check_meals_routine, (void *)data);
    if (err)
        write(STDERR_FILENO, "pthread_create() failed\n", 24);
}

int main(int argc, char **argv)
{
    t_philo philo;
    t_data  data;
    int     i;
    int     wstatus;
    
    if (argc < 5 || argc > 6)
    {
        printf("This program requires between 4 and 5 arguments!\n"); // a ecrire sur stderr
        printf("Example : ./philo 7 1500 300 200\n");
        return (1);
    }
    wstatus = 0;
    init_philo(&philo, argv + 2);
    data.pids = (pid_t *)malloc(sizeof(pid_t) * data.philos_count);
    if (!data.pids)
        return (1);
    
    /* Create semaphores */
    init_data(&data, argv[1]);

    /* Create monitoring threads */
    
    /* Create child processes, ie philosophers */
    i = 0;
    while (i < data.philos_count)
    {
        data.pids[i] = fork();
        if (data.pids[i] < 0)
            return (3);
        if (data.pids[i] == 0)
            philo_process(&philo, &data, i);
        else 
        {
            // monitoring ?
        }
        i++;
    }

    /* Wait for childs processes, and unlink semaphores*/
    i = 0;
    while (i < data.philos_count)
    {
        if (waitpid(data.pids[i], NULL, 0) < 0)
            write(STDERR_FILENO, "waitpid() failed\n", 17);
        i++;
    }

    /* free memory, unlink semaphores, join threads */
    exit_program(philo, &data);
    unlink_semaphores(&data);
}