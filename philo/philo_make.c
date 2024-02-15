/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_make.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:14:36 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:14:38 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	make_philo(int ac, char **av, int num_philo, t_data *data)
{
	int		i;
	int		error_check;
	t_thre	*tid;
	t_share	share;

	error_check = share_init(&share, num_philo, av);
	if (error_check == -1)
		return (error_print());
	tid = (t_thre *)malloc(sizeof(t_thre) * num_philo);
	if (tid == 0)
		return (error_free(tid, &share));
	i = -1;
	while (++i < num_philo)
	{
		if (data_init(&(data[i]), i, ac, av) == -1)
			return (error_free(tid, &share));
		data[i].share = &share;
		if (num_philo == 1)
			pthread_create(&(tid[i]), 0, sit_one, &(data[i]));
		else
			pthread_create(&(tid[i]), 0, sit_table, &(data[i]));
	}
	return (end_philo(data, tid));
}

int	share_init(t_share *share, int num_philo, char **av)
{
	int	i;

	share->die = 0;
	share->print = 0;
	share->start_time = 0;
	share->count = 0;
	share->total = num_philo;
	share->t.die = ph_atoi(av[2]) * 1000;
	share->t.eat = ph_atoi(av[3]) * 1000;
	share->t.sleep = ph_atoi(av[4]) * 1000;
	if (share->t.die < 0 || share->t.eat < 0 || share->t.sleep < 0)
		return (-1);
	share->mutex = (t_mutex *)malloc(sizeof(t_mutex) * num_philo);
	if (share->mutex == 0)
		return (-1);
	i = 0;
	while (i < num_philo)
		pthread_mutex_init(&(share->mutex[i++]), 0);
	pthread_mutex_init(&(share->start_mutex), 0);
	pthread_mutex_init(&(share->die_mutex), 0);
	pthread_mutex_init(&(share->print_mutex), 0);
	return (0);
}

int	data_init(t_data *data, int i, int ac, char **av)
{
	data->sit = i;
	data->last_eat = 0;
	pthread_mutex_init(&(data->eat_mutex), 0);
	if (ac == 6)
		data->must_eat = ph_atoi(av[5]);
	else
		data->must_eat = -2;
	if (data->must_eat == -1)
		return (-1);
	return (0);
}
