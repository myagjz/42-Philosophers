/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_actions_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:25:40 by myagiz            #+#    #+#             */
/*   Updated: 2023/07/25 15:25:41 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_check_death(void *arg)
{
	t_philo	*philo;

	philo = arg;
	while (true)
	{
		if ((philo->last_meal + philo->info->time_to_die) < ft_get_time())
		{
			ft_print_msg(philo, "died");
			exit(FT_FAILURE);
		}
		usleep(1000);
	}
	return (NULL);
}

void	ft_philo_actions(t_philo *philo)
{
	pthread_t	monitor;

	pthread_create(&monitor, NULL, ft_check_death, philo);
	while (true)
	{
		ft_grab_fork(philo);
		ft_grab_fork(philo);
		ft_eating(philo);
		ft_down_forks(philo);
		ft_sleep_then_think(philo);
	}
	pthread_join(monitor, NULL);
}
