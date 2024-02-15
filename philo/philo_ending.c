/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_ending.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:14:22 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:14:24 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	end_philo(t_data *data, t_thre *tid)
{
	int		i;
	int		eat_num;

	start_monitor(data);
	while (42)
	{
		i = -1;
		eat_num = 0;
		while (++i < data[0].share->total)
		{
			pthread_mutex_lock(&data[i].eat_mutex);
			if ((time_stamp() - data[i].last_eat) * 1000 > data[0].share->t.die)
			{
				pthread_mutex_unlock(&data[i].eat_mutex);
				return (clean_all(data, i, tid, data[0].share->total));
			}
			if (data[i].must_eat == 0)
				eat_num++;
			pthread_mutex_unlock(&data[i].eat_mutex);
			if (eat_num == data[0].share->total)
				return (clean_all(data, i, tid, data[0].share->total));
		}
	}
	return (1);
}

void	start_monitor(t_data *data)
{
	int		i;

	i = -1;
	while (++i < data[0].share->total)
	{
		while (42)
		{
			pthread_mutex_lock(&data[i].eat_mutex);
			if (data[i].last_eat != 0)
			{
				pthread_mutex_unlock(&data[i].eat_mutex);
				break ;
			}
			pthread_mutex_unlock(&data[i].eat_mutex);
			usleep(100);
		}
	}
}

int	clean_all(t_data *data, int j, t_thre *tid, int total)
{
	int		i;
	void	*re;

	pthread_mutex_lock(&(data[0].share->die_mutex));
	pthread_mutex_lock(&(data[0].share->print_mutex));
	if (data[j].must_eat == -2)
		printf("\033[0;31m%lld %d died\n",
			time_stamp() - data[0].share->start_time, j + 1);
	data[0].share->die = 1;
	data[0].share->print = 1;
	pthread_mutex_unlock(&(data[0].share->print_mutex));
	pthread_mutex_unlock(&(data[0].share->die_mutex));
	i = 0;
	while (i < total)
	{
		pthread_join(tid[i], &re);
		pthread_mutex_destroy(&(data[i].eat_mutex));
		pthread_mutex_destroy(&(data[0].share->mutex[i++]));
	}
	pthread_mutex_destroy(&(data[0].share->die_mutex));
	free(data[0].share->mutex);
	free(tid);
	return (0);
}
