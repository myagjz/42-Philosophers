/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:15:21 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:15:23 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data	*data;
	int		num_philo;

	if (ac != 5 && ac != 6)
		return (error_print());
	num_philo = ph_atoi(av[1]);
	if (num_philo == -1)
		return (error_print());
	data = (t_data *)malloc(sizeof(t_data) * num_philo);
	if (data == 0)
		return (error_print());
	make_philo(ac, av, num_philo, data);
	free(data);
	return (0);
}

int	ph_atoi(char *str)
{
	long	toss;

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
