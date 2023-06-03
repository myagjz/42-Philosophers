/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:31:47 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 17:47:50 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* msg:
*	Konsola bir mesaj yazdırır.
*	Sağlanan çıkış numarasını döndürür.
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
*	Ayrılan belleği serbest bırakır, bir hata mesajı yazdırır
*	ve başarısızlığı belirtmek için 0 değerini döndürür.
*	Başlatma sırasında hata yönetimi için kullanılır.
*/
int	error_failure(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	return (msg(str, details, 0));
}

/* error_null:
*	Ayrılan belleği serbest bırakır, bir hata mesajı yazdırır
*	ve bir NULL işaretçi döndürür.
*	Başlatma sırasında hata yönetimi için kullanılır.
*/
void	*error_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}

/* child_exit:
*	Çocuk filozof işlemi uygun çıkış koduyla sonlandırılır.
*	Eğer çocuk bir hata ile karşılaşırsa bir hata mesajı yazdırılır.
*/
void	child_exit(t_table *table, int exit_code)
{
	sem_post(table->this_philo->sem_meal);
	pthread_join(table->this_philo->personal_grim_reaper, NULL);
	if (exit_code == CHILD_EXIT_ERR_SEM)
		msg(STR_ERR_SEM, NULL, 0);
	if (exit_code == CHILD_EXIT_ERR_PTHREAD)
		msg(STR_ERR_THREAD, NULL, 0);
	sem_close(table->this_philo->sem_forks);
	sem_close(table->this_philo->sem_philo_full);
	sem_close(table->this_philo->sem_philo_dead);
	sem_close(table->this_philo->sem_write);
	sem_close(table->this_philo->sem_meal);
	free_table(table);
	exit(exit_code);
}
