/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:27:12 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 16:23:20 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* set_sim_stop_flag:
*	Simülasyon durdurma bayrağını doğru veya yanlış olarak ayarlar.
*	Sadece grim reaper (ölüm meleği) iş parçacığı bu bayrağı ayarlayabilir.
*	Eğer simülasyon durdurma bayrağı doğru olarak ayarlanırsa,
*	bu simülasyonun bir sonlandırma koşuluna ulaştığı anlamına gelir.
*/
static void	set_sim_stop_flag(t_table *table, bool state)
{
	pthread_mutex_lock(&table->sim_stop_lock);
		table->sim_stop = state;
	pthread_mutex_unlock(&table->sim_stop_lock);
}

/* has_simulation_stopped:
*	Simülasyonun sona erip ermediğini kontrol eder.
*	Durma bayrağı, herhangi bir çakışma olmadan
*	simulasyon durumunu kontrol etmek için bir mutex
*	kilidiyle korunur. Eğer simulasyon durma bayrağı
*	true olarak ayarlanmışsa true döner,
*	bayrak false olarak ayarlanmışsa false döner.
*/
bool	has_simulation_stopped(t_table *table)
{
	bool	r;

	r = false;
	pthread_mutex_lock(&table->sim_stop_lock);
	if (table->sim_stop == true)
		r = true;
	pthread_mutex_unlock(&table->sim_stop_lock);
	return (r);
}

/* kill_philo:
*	Filozofun öldürülüp öldürülmeyeceğini,
*	filozofun son öğününden bu yana geçen süreyi
*	ve time_to_die parametresini karşılaştırarak kontrol eder.
*	Eğer filozofun ölmesi gerekiyorsa,
*	simulasyon durma bayrağını ayarlar ve ölüm durumunu gösterir.
*	Filozof öldürüldüyse true döner, aksi takdirde false döner.
*/
static bool	kill_philo(t_philo *philo)
{
	time_t	time;

	time = get_time_in_ms();
	if ((time - philo->last_meal) >= philo->table->time_to_die)
	{
		set_sim_stop_flag(philo->table, true);
		write_status(philo, true, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	return (false);
}

/* end_condition_reached:
*	Her bir filozofun iki farklı sonlandırma koşulundan birine
*	ulaşıp ulaşmadığını kontrol eder. Eğer bir filozofun öldürülmesi gerekiyorsa
*	veya her bir filozof yeterli miktarda yemek yedi ise, simulasyonu durdurur.
*	Bir sonlandırma koşuluna ulaşıldıysa true döner, aksi takdirde false döner.
*/
static bool	end_condition_reached(t_table *table)
{
	unsigned int	i;
	bool			all_ate_enough;

	all_ate_enough = true;
	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&table->philos[i]->meal_time_lock);
		if (kill_philo(table->philos[i]))
			return (true);
		if (table->must_eat_count != -1)
			if (table->philos[i]->times_ate
				< (unsigned int)table->must_eat_count)
				all_ate_enough = false;
		pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
		i++;
	}
	if (table->must_eat_count != -1 && all_ate_enough == true)
	{
		set_sim_stop_flag(table, true);
		return (true);
	}
	return (false);
}

/* grim_reaper:
*	Grim Reaper (ölüm meleği) iş parçacığı rutini.
*	Bir filozofun öldürülmesi gerekip gerekmediğini ve
*	tüm filozofların yeterli miktarda yemek yiyip-yemediğini
*	kontrol eder. Bu iki sonlandırma koşulundan birine ulaşılırsa,
*	simulasyonu durdurur.
*/
void	*grim_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	set_sim_stop_flag(table, false);
	sim_start_delay(table->start_time);
	while (true)
	{
		if (end_condition_reached(table) == true)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
