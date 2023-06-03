/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:29:44 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 17:24:37 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* get_time_in_ms:
*	Başlangıç (Epoch) tarihinden (1970-01-01 00:00:00) bu yana geçen
*	milisaniye cinsinden mevcut zamanı alır. Zaman değerini döndürür.
*/
time_t	get_time_in_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/* philo_sleep:
*	Filozof iş parçacığını belirli bir süre (milisaniye cinsinden) duraklatır.
*	Uyku süresi boyunca periyodik olarak simulasyonun sona erip ermediğini
*	kontrol eder ve eğer sona ermişse uykuyu kısaltır.
*/
void	philo_sleep(t_table *table, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = get_time_in_ms() + sleep_time;
	while (get_time_in_ms() < wake_up)
	{
		if (has_simulation_stopped(table))
			break ;
		usleep(100);
	}
}

/* sim_start_delay:
*	Her iş parçacığının yürütmesi başlamadan önce küçük bir gecikme bekler,
*	böylece tüm iş parçacıkları aynı zamanda ve aynı başlangıç zamanı referansıyla başlar.
*	Bu, ölüm meleği iş parçacığının filozof iş parçacıklarıyla senkronize olmasını sağlar.
*/
void	sim_start_delay(time_t start_time)
{
	while (get_time_in_ms() < start_time)
		continue ;
}
