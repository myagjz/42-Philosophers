/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_print_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:16:24 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:16:26 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ph_bonus.h"

void	print_eat(t_data *data)
{
	sem_wait(data->print_sem);
	printf("%lld %d is eating\n", time_stamp() - data->start_time,
		data->sit + 1);
	sem_post(data->print_sem);
	ph_usleep(data->t.eat);
}

void	print_sleep(t_data *data)
{
	sem_wait(data->print_sem);
	printf("%lld %d is sleeping\n", time_stamp() - data->start_time,
		data->sit + 1);
	sem_post(data->print_sem);
	ph_usleep(data->t.sleep);
}

void	print_think(t_data *data)
{
	sem_wait(data->print_sem);
	printf("%lld %d is thinking\n", time_stamp() - data->start_time,
		data->sit + 1);
	sem_post(data->print_sem);
}

void	print_fork(t_data *data)
{
	sem_wait(data->print_sem);
	printf("%lld %d has taken a fork\n", time_stamp() - data->start_time,
		data->sit + 1);
	sem_post(data->print_sem);
}
