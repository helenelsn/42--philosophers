/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 20:43:28 by Helene            #+#    #+#             */
/*   Updated: 2023/09/12 19:18:32 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

/*void    print_error_msg(char *syscall)
{
    char *msg;
    
    msg = ft_strjoin(syscall, " has failed\n");
    write(STDERR_FILENO, msg, ft_strlen(msg));
    free(msg);
}*/

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
    if (init_philo(&philo, argv + 2) == false || init_data(&data, argv[1]) == false)
        return (1);
    // if (init_data(&data, argv[1]) == false)
    //     return (2);
    data.pids = (pid_t *)malloc(sizeof(pid_t) * data.philos_count);
    if (!data.pids)
        return (1);
    

    /* Create child processes, ie philosophers */
    i = 0;
    while (i < data.philos_count)
    {
        //dprintf(2, "i = %d\n", i);
        data.pids[i] = fork();
        if (data.pids[i] < 0)
            return (3);
        if (data.pids[i] == 0)
        {
            if (i % 2)
                usleep(philo.time_to_eat * 500);
            philo_process(&philo, &data, i);
        }
        i++;
    }
    
    /* Create monitoring threads */
    create_threads(&data, argc - 1);
    parent_process(&philo, &data);
    if (pthread_join(data.check_death, NULL))
        write(STDERR_FILENO, "pthread_join() failed\n", 22);
    if (argc == 6)
    {
        if (pthread_join(data.check_meals, NULL))
            write(STDERR_FILENO, "pthread_join() failed\n", 22);
    }
    exit_parent(&philo, &data);
    return (0);

    /*
    i = 0;
    while (i < data.philos_count)
    {
        printf("waiting for process %d\n", i);
        if (waitpid(data.pids[i], NULL, 0) < 0)
            write(STDERR_FILENO, "waitpid() failed\n", 17);
        i++;
    }
    dprintf(2, "apres les gosses\n");
    exit_parent(&philo, &data);*/
}