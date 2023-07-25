/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_args_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:26:06 by myagiz            #+#    #+#             */
/*   Updated: 2023/07/25 15:26:07 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_get_args(t_info *info, int ac, char **av)
{
	info->number_of_philosophers = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]);
	info->time_to_eat = ft_atoi(av[3]);
	info->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		info->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
	else
		info->number_of_times_each_philosopher_must_eat = -1;
	info->id_table = ft_calloc(info->number_of_philosophers, sizeof(int));
	if (!info->id_table)
		ft_puterr("ft_get_args:17 - Allocation Error!\n");
}
