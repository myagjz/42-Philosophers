/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_bonus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:15:48 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:15:53 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ph_bonus.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		error_print(2);
	if ((data_init(&data, av)) == -1)
		error_print(2);
	make_philo(&data);
	return (0);
}

int	data_init(t_data *data, char **av)
{
	data->sit = -1;
	data->start_time = 0;
	data->last_eat = 0;
	data->num_philo = ph_atoi(av[1]);
	data->t.die = ph_atoi(av[2]) * 1000;
	data->t.eat = ph_atoi(av[3]) * 1000;
	data->t.sleep = ph_atoi(av[4]) * 1000;
	data->must_eat = ph_atoi(av[5]);
	if (data->num_philo < 0 || data->t.die < 0 || data->t.eat < 0
		|| data->t.sleep < 0 || data->must_eat == -1)
		return (-1);
	make_sem_file(data);
	make_print_sem_file(data);
	make_must_sem_file(data);
	make_eat_sem_file(data);
	return (0);
}

int	ph_atoi(char *str)
{
	long	toss;

	if (str == NULL)
		return (-2);
	toss = 0;
	while (*str >= '0' && *str <= '9' && *str != 0)
	{
		toss = 10 * toss + (*str - '0');
		str++;
	}
	if (*str != 0)
		return (-1);
	if (toss > 2147483647)
		return (-1);
	return (toss);
}
