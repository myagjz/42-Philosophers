/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_monitor_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:16:19 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:16:22 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ph_bonus.h"

void	*monitoring(void *tmp)
{
	t_data	*data;
	int		flag;

	flag = 0;
	data = (t_data *)tmp;
	while (42)
	{
		sem_wait(data->eat_sem);
		if ((time_stamp() - data->last_eat) * 1000 > data->t.die)
		{
			sem_post(data->eat_sem);
			sem_wait(data->print_sem);
			exit(0);
		}
		else if (flag == 0 && data->must_eat == 0)
		{
			sem_post(data->must_sem);
			flag = 1;
		}
		sem_post(data->eat_sem);
		usleep(100);
	}
	return (data);
}

void	*must_monitor(void *tmp)
{
	t_data	*data;
	int		i;

	data = (t_data *)tmp;
	i = 0;
	start_setting(data);
	sem_post(data->print_sem);
	while (i++ < data->num_philo)
		sem_wait(data->must_sem);
	sem_wait(data->print_sem);
	i = 0;
	while (i < data->num_philo)
		kill(data->pid[i++], SIGKILL);
	free(data->pid);
	exit(0);
	return (data);
}
