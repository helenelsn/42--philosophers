/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:19:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/06 13:29:17 by Helene           ###   ########.fr       */
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

    // if (!data->time_to_die)
    //     return (true);
    // if (!data->time_to_eat || !data->time_to_sleep)
    //     return (false); // que faire ?
    time_to_die = data->time_to_die * 1000;
    state_time = data->time_to_eat * 1000;
    if (state)
        state_time = data->time_to_sleep;
    while (state_time > 0 && time_to_die > 0)
    {
        if (ft_is_end(data)) // checks regularly if one of the philos died while eating/sleeping/thinking
        {
            // mettre end_simulation à true
            return (true);
        }
        usleep(500);
        state_time -= 500;
        time_to_die -= 500;
    }
    /* Retoutne 0 dans le cas ou le philo a fini de manger normalement,
    et 1 s'il est mort pendant */
    return (false);      
}