/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:29:28 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 17:21:53 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* eat_sleep_routine:
*	Bir filozof yemek için hazır olduğunda, çatal mutex'lerinin
*	kilidinin açılmasını bekler ve ardından onları kilitleyerek yemek yer.
*	Ardından filozof belirli bir süre boyunca yer. Son yemeğin zamanı,
*	yemeğin başında, konunun gerekliliklerine uygun olarak kaydedilir.
*/
static void	eat_sleep_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	write_status(philo, false, GOT_FORK_2);
	write_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
	philo_sleep(philo->table, philo->table->time_to_eat);
	if (has_simulation_stopped(philo->table) == false)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	write_status(philo, false, SLEEPING);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	philo_sleep(philo->table, philo->table->time_to_sleep);
}

/* think_routine:
*	Bir filozof uyandığında, tekrar yemek yemeye başlamadan önce
*	belirli bir süre düşünecektir. time_to_think,
*	filozofun son yemekten bu yana ne kadar süre geçtiğine,
*	time_to_eat'e ve time_to_die'ye bağlı olarak hesaplanır ve
*	filozofun ne zaman tekrar acıkacağını belirler.
*	Bu, çatalın gereksiz yere bir filozof tarafından diğerlerine
*	zarar vermeden tekelleştirilmesini önlemek için filozofların
*	yeme rutinlerini dengeli bir şekilde düzenlemeye yardımcı olur.
*/
static void	think_routine(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	pthread_mutex_lock(&philo->meal_time_lock);
	time_to_think = (philo->table->time_to_die
			- (get_time_in_ms() - philo->last_meal)
			- philo->table->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
		write_status(philo, false, THINKING);
	philo_sleep(philo->table, time_to_think);
}

/* lone_philo_routine:
*	Bu rutin, yalnızca tek bir filozof olduğunda çağrılır.
*	Tek bir filozof sadece bir çatala sahiptir ve bu nedenle yemek yiyemez.
*	Filozof, o çatalı alacak, time_to_die kadar bekleyecek ve ölecektir.
*	Bu, yemek rutini içinde ikinci çatalı beklerken iş parçacığının
*	takılıp kalmamasını sağlamak için ayrı bir rutindir.
*/
static void	*lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo, false, GOT_FORK_1);
	philo_sleep(philo->table, philo->table->time_to_die);
	write_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	return (NULL);
}

/* philosopher:
*	Filozofun iş parçacığı rutini. Filozofun yemek yemesi,
*	uyuması ve düşünmesi gerekmektedir. Filozof iş parçacıkları
*	arasında çatışmaları önlemek için, çift kimliğe sahip filozoflar
*	düşünerek başlarlar ve bu onların yemek zamanını küçük bir süre geciktirir.
*	Bu, tek kimliğe sahip filozofların çatalı önce kapmasını sağlar
*	ve kilitlenmeleri engeller.
*/
void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->table->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	sim_start_delay(philo->table->start_time);
	if (philo->table->time_to_die == 0)
		return (NULL);
	if (philo->table->nb_philos == 1)
		return (lone_philo_routine(philo));
	else if (philo->id % 2)
		think_routine(philo, true);
	while (has_simulation_stopped(philo->table) == false)
	{
		eat_sleep_routine(philo);
		think_routine(philo, false);
	}
	return (NULL);
}
