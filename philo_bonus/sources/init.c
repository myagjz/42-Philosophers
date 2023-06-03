/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:37:15 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 18:04:24 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* set_local_sem_name:
*	Bir filozofun kendi yemek değişkenlerini koruyan bir
*	mutex oluşturmak için benzersiz bir semafor adı oluşturulur.
*	Adın benzersiz olması gerekmektedir, aksi takdirde tüm filozof
*	işlemleri aynı yemek semaforunu paylaşacaktır.
*	Bu semafor adını oluşturmak için standart semafor yemek adını
*	filozofun kimliğiyle birleştirmeniz yeterlidir.
*	Verilen kimlikle birlikte benzersiz semafor adını döndürür.
*	Bellek tahsisi başarısız olursa NULL döndürülür.
*/
static char	*set_local_sem_name(const char *str, unsigned int id)
{
	unsigned int	i;
	unsigned int	digit_count;
	char			*sem_name;
	char			*tmp;

	digit_count = 0;
	i = id;
	while (i)
	{
		digit_count++;
		i /= 10;
	}
	i = ft_strlen(str) + digit_count;
	sem_name = malloc (sizeof * sem_name * (i + 1));
	if (sem_name == NULL)
		return (NULL);
	sem_name[0] = '\0';
	sem_name = ft_strcat(sem_name, str);
	tmp = ft_utoa(id, digit_count);
	sem_name = ft_strcat(sem_name, tmp);
	free(tmp);
	return (sem_name);
}

/* set_philo_sem_names:
*	Bir yemek yeme süreci ve onun kişisel ölüm meleği iş parçacığı
*	arasında paylaşılacak olan semaforlar için benzersiz semafor isimleri oluşturun.
*	Başka bir süreç, başka bir yemek yeme sürecine ait semaforları açmayacak.
*/
static bool	set_philo_sem_names(t_philo *philo)
{
	philo->sem_meal_name = set_local_sem_name(SEM_NAME_MEAL, philo->id + 1);
	if (philo->sem_meal_name == NULL)
		return (false);
	return (true);
}

/* init_philosophers:
*	Her bir filozof için bellek tahsis eder ve değerlerini başlatır.
*	Ayrıca, kendi yemekle ilgili değişkenlerini korumak için
*	belirli semafor isimleri oluşturur. sem_meal için semafor isimleri,
*	semaforun başka bir filozof işlemi tarafından erişilmemesi için
*	her filozof için benzersiz olmalıdır. Filozof dizisinin bir
*	işaretçisini döndürür veya başlatma başarısız olduysa NULL döndürür.
*/
static t_philo	**init_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * (table->nb_philos + 1));
	if (!philos)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_null(STR_ERR_MALLOC, NULL, 0));
		philos[i]->table = table;
		philos[i]->id = i;
		if (!set_philo_sem_names(philos[i]))
			return (error_null(STR_ERR_MALLOC, NULL, table));
		philos[i]->times_ate = 0;
		philos[i]->nb_forks_held = 0;
		philos[i]->ate_enough = false;
		i++;
	}
	return (philos);
}

/* init_global_semaphores:
*	Çatal, yazma, doluluk ve ölüm tespiti için semaforları başlatır.
*	Bu semaforlar önce ana süreçte açılır, ancak her bir çocuk süreç
*	aynı adı taşıyan semaforları açar,
*	bu semaforlar aracılığıyla süreçler arası iletişime izin verir.
*
*	Başarılı bir şekilde başlatma gerçekleştiyse true değerini,
*	başlatma başarısız olduysa false değerini döndürür.
*/
static bool	init_global_semaphores(t_table *table)
{
	unlink_global_sems();
	table->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_forks == SEM_FAILED)
		return (sem_error_cleanup(table));
	table->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (table->sem_write == SEM_FAILED)
		return (sem_error_cleanup(table));
	table->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_philo_full == SEM_FAILED)
		return (sem_error_cleanup(table));
	table->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_philo_dead == SEM_FAILED)
		return (sem_error_cleanup(table));
	table->sem_stop = sem_open(SEM_NAME_STOP, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (table->sem_stop == SEM_FAILED)
		return (sem_error_cleanup(table));
	return (true);
}

/* init_table:
*	"Dining table" adında, programın tüm parametrelerini
*	içeren veri yapısını başlatır. Başlatma sırasında bir hata oluşursa,
*	ayrılan tablo yapısının bir işaretçisini döndürür veya NULL değerini döndürür.
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
	table->philo_full_count = 0;
	table->stop_sim = false;
	if (ac - 1 == 5)
		table->must_eat_count = integer_atoi(av[i]);
	if (!init_global_semaphores(table))
		return (NULL);
	table->philos = init_philosophers(table);
	if (!table->philos)
		return (NULL);
	table->pids = malloc(sizeof * table->pids * table->nb_philos);
	if (!table->pids)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	return (table);
}
