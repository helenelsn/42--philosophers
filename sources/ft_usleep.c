/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:19:46 by hlesny            #+#    #+#             */
/*   Updated: 2023/09/04 19:53:27 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Gerer le cas ou, si un philo doit manger pendant 5h mais meurt
s'il n'a pas fini de manger au bout de 2h, 
le ft_usleep ne doit durer que 2h

quel type pour les time_to_die et time_to_eat ? 
sachant qu'il s'qgit de millisecondes

Passer directement en argument t_data, car aura aussi besoin de check la valeur de someone_died (?)
*/
int    ft_usleep(unsigned long time_to_eat, unsigned long time_to_die)
{
    while (time_to_eat > 0 && time_to_die > 0)
    {
        usleep(10);
        time_to_eat -= 10;
        time_to_die -= 10;
    }
    /* Retoutne 1 dans le cas ou le philo a fini de manger normalement,
    et 0 s'il est mort pendant */
    if (time_to_eat > 0 )
        return (0);
    return (1);      
}