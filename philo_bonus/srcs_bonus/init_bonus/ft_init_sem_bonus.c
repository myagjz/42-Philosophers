/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_sem_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:26:18 by myagiz            #+#    #+#             */
/*   Updated: 2023/07/25 15:26:19 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*0644
0 > no access to file
6 > read/write permisson for the user
4 > read only for the group and others
*/
int	ft_init_sem(t_info *info)
{
	info->forks = sem_open("sem", O_CREAT | O_EXCL, 0644,
			info->number_of_philosophers);
	if (info->forks == SEM_FAILED || sem_unlink("sem"))
		ft_puterr("ft_init_sem:12 - error initializing semaphore\n");
	return (FT_SUCCESS);
}
