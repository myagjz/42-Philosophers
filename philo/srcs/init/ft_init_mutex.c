/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_mutex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:22:14 by myagiz            #+#    #+#             */
/*   Updated: 2023/07/25 15:22:15 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_init_mutex(t_info *info)
{
	int	i;

	i = 0;
	pthread_mutex_init(&info->finish_lock, NULL);
	while (i < info->number_of_philosophers)
		pthread_mutex_init(&info->forks[i++], NULL);
}
