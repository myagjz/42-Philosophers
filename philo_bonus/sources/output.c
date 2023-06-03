/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:37:54 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 18:25:38 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* print_status_debug:
*	Filozofun durumunu, hata ayıklama sürecine yardımcı olmak için
*	daha okunabilir ve renkli bir formatta yazdırır.
*	Çatal alma durumları için, filozofun hangi çatalı aldığını
*	göstermek için ekstra bilgiler görüntülenir.
*/
void	print_status_debug(t_philo *philo, char *color,
								char *str, t_status status)
{
	if (status == GOT_FORK_1)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork #1\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str);
	else if (status == GOT_FORK_2)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork #2\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str);
	else
		printf("[%10ld]\t%s%03d\t%s\e[0m\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str);
}

/* write_status_debug:
*	Hata ayıklama modu için durum yazmayı yönlendirir.
*	Bu seçenek için philo_bonus.h dosyasında DEBUG_FORMATTING
*	seçeneğinin 1 olarak ayarlanması gerekmektedir.
*/
static void	write_status_debug(t_philo *philo, t_status status)
{
	if (status == DIED)
		print_status_debug(philo, RED, "died", status);
	else if (status == EATING)
		print_status_debug(philo, GREEN, "is eating", status);
	else if (status == SLEEPING)
		print_status_debug(philo, CYAN, "is sleeping", status);
	else if (status == THINKING)
		print_status_debug(philo, CYAN, "is thinking", status);
	else if (status == GOT_FORK_1)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
	else if (status == GOT_FORK_2)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
}

/* print_status:
*	Filozofun durumunu proje konusu gereği düz metin olarak yazdırır.
*		timestamp_in_ms X status
*/
void	print_status(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
}

/* write_status:
*	Bir filozofun durumunu yazdırır. Farklı süreçlerden karışık mesajların
*	oluşmaması için yazma semaforunu azaltır.
*	Eğer mesaj bir ölüm meleğinden geliyorsa,
*	mesaj terminalde görüntülendikten sonra yazma semaforu tekrar artırılmaz,
*	böylece bir filozof öldükten sonra herhangi bir
*	durum mesajının görüntülenmesi önlenir.
*
*	Eğer philo.h dosyasındaki DEBUG_FORMATTING 1 olarak ayarlanmışsa,
*	durum renklerle ve hata ayıklamaya yardımcı olacak ek bilgilerle
*	biçimlendirilecektir. Aksi takdirde çıktı,
*	proje konusunda gereken normal biçimde olacaktır.
*/
void	write_status(t_philo *philo, bool reaper_report, t_status status)
{
	sem_wait(philo->sem_write);
	if (DEBUG_FORMATTING == true)
	{
		write_status_debug(philo, status);
		if (!reaper_report)
			sem_post(philo->sem_write);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_status(philo, "has taken a fork");
	if (!reaper_report)
		sem_post(philo->sem_write);
}

/* write_outcome:
*	Eğer belirli bir yemek sayısı belirtilmişse,
*	simülasyonun sonucunu yazdırır.
*	Sadece hata ayıklama amaçlı kullanılır.
*/
void	write_outcome(t_table *table)
{
	sem_post(table->sem_write);
	sem_wait(table->sem_write);
	printf("%d/%d philosophers had at least %d meals.\n",
		table->philo_full_count, table->nb_philos, table->must_eat_count);
	sem_post(table->sem_write);
	return ;
}
