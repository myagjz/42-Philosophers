/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:22:10 by myagiz            #+#    #+#             */
/*   Updated: 2023/07/25 15:22:11 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_init_all(t_info *info, int ac, char **av)
{
	if (!info)
		return (FT_FAILURE);
	ft_get_args(info, ac, av);
	if (ft_check_info(*info, ac))
		return (FT_FAILURE);
	info->philo = ft_calloc(info->number_of_philosophers, sizeof(t_philo));
	info->forks = ft_calloc(info->number_of_philosophers,
			sizeof(pthread_mutex_t));
	if (!info->philo || !info->forks)
		return (FT_FAILURE);
	info->finish = false;
	info->all_ate = 0;
	ft_init_mutex(info);
	ft_create_philos(info);
	return (FT_SUCCESS);
}
