/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_eating_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:25:23 by myagiz            #+#    #+#             */
/*   Updated: 2023/07/25 15:25:25 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_eating(t_philo *philo)
{
	philo->last_meal = ft_get_time();
	ft_print_msg(philo, "is eating");
	usleep(philo->info->time_to_eat * 1000);
}
