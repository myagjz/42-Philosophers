/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:32:05 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 18:05:24 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* kill_all_philos:
*	Tüm filozof işlemlerine sonlandırma sinyali gönderir.
*	Bir filozof ölmüşse veya bir filozof işlemi ölümcül bir hatayla
*	karşılaşmışsa simulasyonu sonlandırmak için kullanılır.
*	Verilen çıkış kodunu döndürür.
*/
int	kill_all_philos(t_table *table, int exit_code)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		kill(table->pids[i], SIGKILL);
		i++;
	}
	return (exit_code);
}

/* global_gluttony_reaper:
*	Eğer her bir filozof yeterince yemişse, tüm filozofları sonlandırır.
*	Her filozof işlemi sem_philo_full semaforunu azaltır.Bu iş parçacığı,
*	azaltmaları kaydederek kaç filozofun yeterince yediğini sayar.
*	Eğer tüm filozoflar yeterince yemişse,
*	simulasyonu sonlandırmak için tüm filozofları sonlandırır.
*/
void	*global_gluttony_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->must_eat_count < 0 || table->time_to_die == 0
		|| table->nb_philos == 1)
		return (NULL);
	sim_start_delay(table->start_time);
	while (table->philo_full_count < table->nb_philos)
	{
		if (has_simulation_stopped(table) == true)
			return (NULL);
		sem_wait(table->sem_philo_full);
		if (has_simulation_stopped(table) == false)
			table->philo_full_count += 1;
		else
			return (NULL);
	}
	sem_wait(table->sem_stop);
	table->stop_sim = true;
	kill_all_philos(table, EXIT_SUCCESS);
	sem_post(table->sem_philo_dead);
	sem_post(table->sem_stop);
	return (NULL);
}

/* global_famine_reaper:
*	Eğer bir filozof öldüyse, tüm filozofları öldür.
*	Her bir filozof işlemi, filozofun ölümü üzerine
*	sem_philo_dead semaforunu azaltır.
*	Bu iş parçacığı, ilk azaltmayı kaydeder ve
*	hemen tüm filozofları öldürür.
*/
void	*global_famine_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->nb_philos == 1)
		return (NULL);
	sim_start_delay(table->start_time);
	if (has_simulation_stopped(table) == true)
		return (NULL);
	sem_wait(table->sem_philo_dead);
	if (has_simulation_stopped(table) == true)
		return (NULL);
	sem_wait(table->sem_stop);
	table->stop_sim = true;
	kill_all_philos(table, EXIT_SUCCESS);
	sem_post(table->sem_philo_full);
	sem_post(table->sem_stop);
	return (NULL);
}

/* end_condition_reached:
*	Bu filozofu kontrol ederek, iki son koşuldan birinin
*	gerçekleşip gerçekleşmediğini kontrol eder. Eğer filozof ölmüşse,
*	tüm filozofları öldürecek olan "famine reaper"ı tetikleyen
*	bir semaforu azaltır. Eğer filozof yeterli miktarda yemek yemişse,
*	"gluttony_reaper"ı tetikleyen bir semaforu azaltır ve
*	philo_full_count'u artırır. Filozof hala hayattaysa, false değerini döndürür.
*/
static bool	end_condition_reached(t_table *table, t_philo *philo)
{
	sem_wait(philo->sem_meal);
	if (get_time_in_ms() - philo->last_meal >= table->time_to_die)
	{
		write_status(philo, true, DIED);
		sem_post(table->this_philo->sem_philo_dead);
		sem_post(philo->sem_meal);
		return (true);
	}
	if (table->must_eat_count != -1 && philo->ate_enough == false
		&& philo->times_ate >= (unsigned int)table->must_eat_count)
	{
		sem_post(philo->sem_philo_full);
		philo->ate_enough = true;
	}
	sem_post(philo->sem_meal);
	return (false);
}

/* personal_grim_reaper:
*	Grim Reaper iş parçacığının rutini.
*	Bu filozofun öldürülüp öldürülmeyeceğini ve
*	yeterli miktarda yemek yiyip yemediğini kontrol eder.
*/
void	*personal_grim_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	sem_wait(table->this_philo->sem_philo_dead);
	sem_wait(table->this_philo->sem_philo_full);
	sim_start_delay(table->start_time);
	while (!end_condition_reached(table, table->this_philo))
	{
		usleep(1000);
		continue ;
	}
	return (NULL);
}
