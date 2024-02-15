/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_utils_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:16:36 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:16:37 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ph_bonus.h"

long long	time_stamp(void)
{
	struct timeval	te;

	gettimeofday(&te, NULL);
	return ((te.tv_sec * 1000) + (te.tv_usec / 1000));
}

void	error_print(int flag)
{
	if (flag == 1)
		write(2, "malloc error\n", 13);
	if (flag == 2)
		write(2, "input error\n", 12);
	exit (1);
}

int	ph_usleep(int time)
{
	long long	target;

	target = (long long)time / 1000 + time_stamp();
	while (target > time_stamp())
		usleep(100);
	return (0);
}

void	start_setting(t_data *data)
{
	while (data->start_time > time_stamp())
		usleep(100);
}
