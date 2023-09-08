/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:19:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/08 15:05:58 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/*
Gerer le cas ou, si un philo doit manger pendant 5h mais meurt
s'il n'a pas fini de manger au bout de 2h, 
le ft_usleep ne doit durer que 2h

States :    0 when eating
            1 when sleeping
*/
bool    ft_usleep(t_data *data, int state) 
{
    unsigned int    state_length;
    
    state_length = get_current_time(data) + data->time_to_eat;
    if (state == sleeping)
        state_length = get_current_time(data) + data->time_to_sleep;
    while (get_current_time(data) < state_length)
    {
        if (ft_is_end(data)) // checks regularly if one of the philos died while eating/sleeping/thinking
            return (false);
        usleep(100);
    }
    return (true);      
}