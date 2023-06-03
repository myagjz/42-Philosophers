/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:31:26 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 17:43:45 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* free_table:
*	Program tarafından ayrılan tüm belleği serbest bırakır.
*	Eğer serbest bırakılacak bir şey yoksa veya tüm bellek
*	serbest bırakıldığında bir NULL işaretçi döndürür.
*/
void	*free_table(t_table *table)
{
	unsigned int	i;

	if (!table)
		return (NULL);
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			if (table->philos[i] != NULL)
			{
				if (table->philos[i]->sem_meal_name)
					free(table->philos[i]->sem_meal_name);
				free(table->philos[i]);
			}
			i++;
		}
		free(table->philos);
	}
	if (table->pids)
		free(table->pids);
	free(table);
	return (NULL);
}

/* sem_error_cleanup:
*	Tüm semaforları kapatır ve bağlantısını koparır.
*	Sadece başlatma sırasında kullanılır, bir semafor açılamazsa.
*	Başarısızlık durumunda 0 değeri döndürür.
*/
int	sem_error_cleanup(t_table *table)
{
	sem_close(table->sem_forks);
	sem_close(table->sem_write);
	sem_close(table->sem_philo_full);
	sem_close(table->sem_philo_dead);
	sem_close(table->sem_stop);
	unlink_global_sems();
	return (error_failure(STR_ERR_SEM, NULL, table));
}

/* table_cleanup:
*	Tüm semaforları kapatır, ayrılan belleği serbest bırakır
*	ve global "grim reaper" iş parçacığının sonlanmasını bekler.
*	Sağlanan çıkış kodunu döndürür.
*/
int	table_cleanup(t_table *table, int exit_code)
{
	if (table != NULL)
	{
		pthread_join(table->famine_reaper, NULL);
		pthread_join(table->gluttony_reaper, NULL);
		sem_close(table->sem_forks);
		sem_close(table->sem_write);
		sem_close(table->sem_philo_full);
		sem_close(table->sem_philo_dead);
		sem_close(table->sem_stop);
		unlink_global_sems();
		free_table(table);
	}
	return (exit_code);
}
