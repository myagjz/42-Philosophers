/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sitting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:14:50 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:15:03 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*sit_one(void *tmp)
{
	t_data	*data;

	data = (t_data *)tmp;
	pthread_mutex_lock(&(data->share->start_mutex));
	data->share->start_time = time_stamp();
	pthread_mutex_unlock(&(data->share->start_mutex));
	pthread_mutex_lock(&(data->eat_mutex));
	data->last_eat = time_stamp();
	pthread_mutex_unlock(&(data->eat_mutex));
	while (42)
	{
		pthread_mutex_lock(&(data->share->die_mutex));
		if (data->share->die == 1)
		{
			pthread_mutex_unlock(&(data->share->die_mutex));
			return (data);
		}
		pthread_mutex_unlock(&(data->share->die_mutex));
		sit_one_util(data);
		pthread_mutex_unlock(&(data->share->mutex[0]));
	}
	return (data);
}

void	*sit_table(void *tmp)
{
	t_data	*data;
	int		right;
	int		left;

	data = (t_data *)tmp;
	start_sit(&right, &left, data);
	while (42)
	{
		pthread_mutex_lock(&(data->share->start_mutex));
		if (data->share->count == data->share->total)
		{
			pthread_mutex_unlock(&(data->share->start_mutex));
			break ;
		}
		pthread_mutex_unlock(&(data->share->start_mutex));
		usleep(100);
	}
	pthread_mutex_lock(&(data->eat_mutex));
	data->last_eat = time_stamp();
	pthread_mutex_unlock(&(data->eat_mutex));
	if (data->sit % 2 == 1)
		ft_usleep(data->share->t.eat / 2, data);
	start_eating(data, right, left);
	return (data);
}

void	start_sit(int *right, int *left, t_data *data)
{
	*right = data->sit;
	if (data->sit == 0)
		*left = data->share->total - 1;
	else
		*left = data->sit - 1;
	pthread_mutex_lock(&(data->share->start_mutex));
	data->share->count++;
	pthread_mutex_unlock(&(data->share->start_mutex));
}

void	start_eating(t_data *data, int right, int left)
{
	int	die;

	die = 0;
	pthread_mutex_lock(&(data->share->start_mutex));
	if (data->share->start_time == 0)
		data->share->start_time = time_stamp();
	pthread_mutex_unlock(&(data->share->start_mutex));
	while (42)
	{
		ph_fork_and_eat(data, right, left);
		print_sleep(data);
		print_think(data);
		pthread_mutex_lock(&(data->share->die_mutex));
		if (data->share->die == 1)
			die = 1;
		pthread_mutex_unlock(&(data->share->die_mutex));
		if (die == 1)
			return ;
	}
}

void	ph_fork_and_eat(t_data *data, int right, int left)
{
	pthread_mutex_lock(&(data->share->mutex[right]));
	print_fork(data);
	pthread_mutex_lock(&(data->share->mutex[left]));
	print_fork(data);
	pthread_mutex_lock(&(data->eat_mutex));
	data->last_eat = time_stamp();
	pthread_mutex_unlock(&(data->eat_mutex));
	print_eat(data);
	pthread_mutex_lock(&(data->eat_mutex));
	if (data->must_eat > 0)
		data->must_eat--;
	pthread_mutex_unlock(&(data->eat_mutex));
	pthread_mutex_unlock(&(data->share->mutex[left]));
	pthread_mutex_unlock(&(data->share->mutex[right]));
}
