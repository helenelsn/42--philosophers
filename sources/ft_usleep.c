/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:19:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/07 13:12:49 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/*
Gerer le cas ou, si un philo doit manger pendant 5h mais meurt
s'il n'a pas fini de manger au bout de 2h, 
le ft_usleep ne doit durer que 2h

quel type pour les time_to_die et time_to_eat ? 
sachant qu'il s'qgit de millisecondes

Passer directement en argument t_data, car aura aussi besoin de check la valeur de someone_died (?)

States :    0 when eating
            1 when sleeping
*/
bool    ft_usleep(t_data *data, int state) 
{
    unsigned int    state_time;
    unsigned int    time_to_die;

    time_to_die = data->time_to_die * 1000;
    state_time = data->time_to_eat * 1000;
    if (state)
        state_time = data->time_to_sleep;
    while (state_time > 500 && time_to_die > 500)
    {
        if (ft_is_end(data)) // checks regularly if one of the philos died while eating/sleeping/thinking
        {
            pthread_mutex_lock(&data->end_simulation_m);
            data->end_simulation = true;
            pthread_mutex_unlock(&data->end_simulation_m);
            return (true);
        }
        usleep(500);
        state_time -= 500;
        time_to_die -= 500;
    }
    /* Retourne 0 dans le cas ou le philo a fini de manger normalement,
    et 1 s'il est mort pendant */
    return (false);      
}