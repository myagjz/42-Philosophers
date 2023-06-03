/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:38:51 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 18:38:14 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* get_time_in_ms:
*	Zamanın Epoch'tan (1970-01-01 00:00:00) bu yana milisaniye
*	cinsinden geçen süresini alır.
*	Zaman değerini döndürür.
*/
time_t	get_time_in_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/* philo_sleep:
*	Filozof sürecini belirli bir süre (milisaniye cinsinden) duraklatır.
*/
void	philo_sleep(time_t sleep_time)
{
	time_t	wake_up;

	wake_up = get_time_in_ms() + sleep_time;
	while (get_time_in_ms() < wake_up)
	{
		usleep(100);
	}
}

/* sim_start_delay:
*	Her süreç yürütmesinin başlangıcında küçük bir gecikme bekler,
*	böylece tüm süreçler aynı zamanda aynı başlangıç zaman referansıyla başlar.
*	Bu, tüm süreçlerin ve ölüm meleği iş parçacıklarının senkronize olduğunu sağlar.
*/
void	sim_start_delay(time_t start_time)
{
	while (get_time_in_ms() < start_time)
		continue ;
}
