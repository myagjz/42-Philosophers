/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:37:28 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 18:13:25 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

/* philo_open_global_semaphores:
*	Ebeveyn ve tüm çocuk süreçler arasında paylaşılan semaforları açar:
*		- Fork (çatal) semaforu,
*		- Write (yazma) semaforu,
*		- Full/philo ate enough (dolu/felsefe yeterince yedi) semaforu,
*		- Dead/philo is dead (ölü/felsefe öldü) semaforu.
*	Tüm semaforlar başarıyla açıldıysa true değerini,
*	bir semaforun açılamaması durumunda false değerini döndürür.
*/
static bool	philo_open_global_semaphores(t_philo *philo)
{
	philo->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_forks == SEM_FAILED)
		return (false);
	philo->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_write == SEM_FAILED)
		return (false);
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		return (false);
	philo->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_dead == SEM_FAILED)
		return (false);
	return (true);
}

/* philo_open_local_semaphores:
*	Yalnızca bu çocuk süreci tarafından kullanılan semaforları açar.
*	Bu semaforlar ebeveynle veya diğer çocuk süreçlerle paylaşılmaz.
*	Yalnızca bu sürecin ve ölüm meleği iş parçacıklarının belirli
*	değişkenlere erişimini düzenlemek için kullanılırlar.
*	Aşağıdaki semaforlar bu amaçla kullanılır:
*
*		- Yemek semaforu (filozofların son yemek zamanlarını tutan değişkenleri korur).
*		- Ölü semaforu (bu şekilde ölüm meleği iş parçacığı filozofun rutinini kesintiye uğratabilir).
*
*	Tüm semaforlar başarıyla açıldıysa true değerini,
*	bir semaforun açılamaması durumunda false değerini döndürür.
*/
static bool	philo_open_local_semaphores(t_philo *philo)
{
	philo->sem_meal = sem_open(philo->sem_meal_name, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_meal == SEM_FAILED)
		return (false);
	sem_unlink(philo->sem_meal_name);
	return (true);
}

/* init_philo_ipc:
*	Filozof süreçleri arasında süreçler arası iletişimi başlatır.
*	Her filozof aynı isimli semaforları açar. Bu semaforlar paylaşılan
*	bellekte depolanır ve her süreç için kopyalanmaz.
*	İsimlendirilmiş semaforlar, Linux sistemlerinde /dev/shm'de depolanır
*	ve diskteki dosyalara benzer şekilde davranır.
*
*	Her filozof süreci ayrıca kendi ölüm meleği iş parçacığını da oluşturur.
*	Bu iş parçacığı, filozofun ölüp ölmediğini veya
*	yeterince yemek yiyip yemediğini tespit eder.
*/
void	init_philo_ipc(t_table *table, t_philo *philo)
{
	if (table->nb_philos == 1)
		return ;
	sem_unlink(philo->sem_meal_name);
	if (!philo_open_global_semaphores(philo))
		child_exit(table, CHILD_EXIT_ERR_SEM);
	if (!philo_open_local_semaphores(philo))
		child_exit(table, CHILD_EXIT_ERR_SEM);
	if (pthread_create(&philo->personal_grim_reaper, NULL,
			&personal_grim_reaper, table) != 0)
		child_exit(table, CHILD_EXIT_ERR_PTHREAD);
	return ;
}
