/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_eating.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:21:27 by myagiz            #+#    #+#             */
/*   Updated: 2023/07/25 15:21:28 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_eating(t_philo *philo)
{
	ft_print_msg(philo, "is eating");
	usleep(philo->info->time_to_eat * 1000);
	philo->last_meal = ft_get_time();
	philo->ate++;
	if (philo->ate == philo->info->number_of_times_each_philosopher_must_eat)
		philo->info->all_ate++;
}
