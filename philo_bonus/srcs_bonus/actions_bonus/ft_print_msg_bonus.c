/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_msg_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:25:49 by myagiz            #+#    #+#             */
/*   Updated: 2023/07/25 15:25:50 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_print_msg(t_philo *philo, char *msg)
{
	if (!philo->should_die)
		printf("%lli\t%i\t%s\n", ft_current_time(philo), philo->id + 1, msg);
}
