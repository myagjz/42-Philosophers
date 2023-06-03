/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:27:39 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 16:43:50 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* intit_forks:
*	Bellek tahsis eder ve çatal mutexlerini başlatır.
*	Çatal mutex dizisinin bir işaretçisini döndürür
*	veya bir hata oluştuysa NULL döndürür.
*/
static pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!forks)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (error_null(STR_ERR_MUTEX, NULL, 0));
		i++;
	}
	return (forks);
}

/* assign_forks:
*	Her bir filozofa iki adet çatal kimliği atar.
*	Çift numaralı filozoflar çatal sırasını değiştirir.
*	Bu, filozofların çatalarını alma sırasının önemli olmasından dolayıdır.
*
*	Örneğin 3 filozof için:
*		Filozof #1 (id: 0), çatal 0 ve çatal 1'i isteyecektir.
*		Filozof #2 (id: 1), çatal 1 ve çatal 2'yi isteyecektir.
*		Filozof #3 (id: 2), çatal 2 ve çatal 0'ı isteyecektir.
*
*	Eğer filozof #1 çatal 0'ı alırsa, filozof #2 çatal 1'i alır ve
*	filozof #3 çatal 2'yi alırsa, bir kilitlenme durumu oluşur.
*	Her bir filozof kendi ikinci çatalını beklerken,
*	ikinci çatal başka bir filozof tarafından kullanıldığı için kilitlenme oluşur.
*
*	Çift kimliğe sahip filozofları "solak" yapmak işe yarar:
*
*		Filozof #1 (id: 0), önce çatal 1'i alır ve ardından çatal 0'ı alır.
*		Filozof #2 (id: 1), önce çatal 1'i alır ve ardından çatal 2'yi alır.
*		Filozof #3 (id: 2), önce çatal 0'ı alır ve ardından çatal 2'yi alır.
*
*	Şimdi, filozof #1 çatal 1'i alır, filozof #3 çatal 0'ı alır ve
*	filozof #2 sabırla bekler. Çatal 2, filozof #3 tarafından alınmak için serbesttir,
*	bu yüzden o yemek yer. Bitirdiğinde filozof #1 çatal 0'ı alıp yemek yiyebilir.
*	O da bitirdiğinde filozof #2 nihayet çatal 1'i alıp yemek yiyebilir.
*/
static void	assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->table->nb_philos;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->nb_philos;
		philo->fork[1] = philo->id;
	}
}

/* init_philosophers:
*	Her bir filozof için bellek tahsis eder ve değerlerini başlatır.
*	Filozofların dizisine bir işaretçi döndürür veya başlatma başarısız olduysa NULL döndürür.
*/
static t_philo	**init_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * table->nb_philos);
	if (!philos)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_null(STR_ERR_MALLOC, NULL, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (error_null(STR_ERR_MUTEX, NULL, 0));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

/* init_global_mutexes:
*	Çatal, yazma ve simulasyon durma bayrağı için mutex kilidini başlatır.
*	Başlatma işlemi başarılı olduysa true döner, başarısız olduysa false döner.
*/
static bool	init_global_mutexes(t_table *table)
{
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (false);
	if (pthread_mutex_init(&table->sim_stop_lock, 0) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	if (pthread_mutex_init(&table->write_lock, 0) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	return (true);
}

/* init_table:
*	"Yemek masası"nı başlatır, programın tüm parametrelerini içeren veri yapısını.
*	Başlatma sırasında bir hata oluşursa, tahsis edilen masa yapısının
*	bir işaretçisini döndürür veya NULL döndürür.
*/
t_table	*init_table(int ac, char **av, int i)
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	table->nb_philos = integer_atoi(av[i++]);
	table->time_to_die = integer_atoi(av[i++]);
	table->time_to_eat = integer_atoi(av[i++]);
	table->time_to_sleep = integer_atoi(av[i++]);
	table->must_eat_count = -1;
	if (ac - 1 == 5)
		table->must_eat_count = integer_atoi(av[i]);
	table->philos = init_philosophers(table);
	if (!table->philos)
		return (NULL);
	if (!init_global_mutexes(table))
		return (NULL);
	table->sim_stop = false;
	return (table);
}
