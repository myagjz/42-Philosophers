/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:15:02 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:15:14 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sit_one_util(t_data *data)
{
	pthread_mutex_lock(&(data->share->mutex[0]));
	print_fork(data);
	while (42)
	{
		pthread_mutex_lock(&(data->share->die_mutex));
		if (data->share->die != 0)
		{
			pthread_mutex_unlock(&(data->share->die_mutex));
			return ;
		}
		pthread_mutex_unlock(&(data->share->die_mutex));
		usleep(100);
	}
}

int	ft_usleep(int time, t_data *data)
{
	long long	target;

	target = (long long)time / 1000 + time_stamp();
	while (target > time_stamp())
	{
		pthread_mutex_lock(&(data->share->die_mutex));
		if (data->share->die == 1)
		{
			pthread_mutex_unlock(&(data->share->die_mutex));
			return (1);
		}
		pthread_mutex_unlock(&(data->share->die_mutex));
		usleep(100);
	}
	return (0);
}

long long	time_stamp(void)
{
	struct timeval	te;

	gettimeofday(&te, NULL);
	return ((te.tv_sec * 1000) + (te.tv_usec / 1000));
}

int	error_print(void)
{
	write(2, "Error!\n", 7);
	return (0);
}

int	error_free(t_thre *tid, t_share *share)
{
	free(tid);
	free(share->mutex);
	return (error_print());
}
