/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:27:06 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 16:13:40 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* free_table:
*	Program tarafından ayrılan tüm belleği serbest bırakır.
*	Eğer serbest bırakılacak bir şey yoksa veya
*	tüm bellek serbest bırakıldıysa, bir NULL işaretçisi döndürür.
*/
void	*free_table(t_table *table)
{
	unsigned int	i;

	if (!table)
		return (NULL);
	if (table->fork_locks != NULL)
		free(table->fork_locks);
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			if (table->philos[i] != NULL)
				free(table->philos[i]);
			i++;
		}
		free(table->philos);
	}
	free(table);
	return (NULL);
}

/* destroy_mutexts:
*	Program tarafından oluşturulan her kilidi yok eder:
*	fork kilidi, yemek kilidi, yazma ve simülasyon durdurma kilidi.
*/
void	destroy_mutexes(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_destroy(&table->fork_locks[i]);
		pthread_mutex_destroy(&table->philos[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->sim_stop_lock);
}

/* msg:
*	Konsola bir ileti yazar. Sağlanan çıkış numarasını döndürür.
*	Hata yönetimi için kullanılır.
*/
int	msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

/* error_failure:
*	Ayrılan belleği serbest bırakır, bir hata iletişimini yazdırır
*	ve başarısızlığı belirtmek için 0 döndürür.
*	Başlatma sırasında hata yönetimi için kullanılır.
*/
int	error_failure(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	return (msg(str, details, 0));
}

/* error_null:
*	Ayrılan belleği serbest bırakır, bir hata iletişimini yazdırır
*	ve bir NULL işaretçisi döndürür. Başlatma sırasında hata yönetimi için kullanılır.
*/
void	*error_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}
