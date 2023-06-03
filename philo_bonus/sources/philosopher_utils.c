/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:38:20 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 18:29:53 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* grab_fork:
*	Filozofun çatal alabilmesi için çatal semaforunu bekler.
*	Filozof çatal aldığında, çatal havuzundaki mevcut çatal sayısının
*	bir azaldığını temsil etmek için semafor 1 azaltılır.
*	Bir filozofun aldığı ilk ve ikinci çatal arasında
*	gerçek bir ayrım olmadığı için,
*	ayrım sadece hata ayıklama amacıyla burada yapılmaktadır.
*/
void	grab_fork(t_philo *philo)
{
	sem_wait(philo->sem_forks);
	sem_wait(philo->sem_meal);
	if (philo->nb_forks_held <= 0)
		write_status(philo, false, GOT_FORK_1);
	if (philo->nb_forks_held == 1)
		write_status(philo, false, GOT_FORK_2);
	philo->nb_forks_held += 1;
	sem_post(philo->sem_meal);
}
