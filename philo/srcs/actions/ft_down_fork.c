/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_down_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:21:22 by myagiz            #+#    #+#             */
/*   Updated: 2023/07/25 15:21:24 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	down_fork(t_philo *philo, int f1, int f2)
{
	pthread_mutex_unlock(&philo->info->forks[f1]);
	pthread_mutex_unlock(&philo->info->forks[f2]);
}
