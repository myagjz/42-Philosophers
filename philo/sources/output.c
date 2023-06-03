/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:28:29 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 17:02:09 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* print_status_debug:
*	Filozofun durumunu daha kolay okunabilir, renkli bir
*	formatta hata ayıklamaya yardımcı olacak şekilde yazdırır.
*	Çatal alım durumları için, filozofun hangi çatalı aldığını
*	göstermek için ekstra bilgiler görüntülenir.
*/
static void	print_status_debug(t_philo *philo, char *color,
								char *str, t_status status)
{
	if (status == GOT_FORK_1)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str, philo->fork[0]);
	else if (status == GOT_FORK_2)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str, philo->fork[1]);
	else
		printf("[%10ld]\t%s%03d\t%s\e[0m\n",
			get_time_in_ms() - philo->table->start_time,
			color, philo->id + 1, str);
}

/* write_status_debug:
*	Hata ayıklama modu için durum yazmayı yeniden yönlendirir.
*	Bu seçenek için, philo.h dosyasında DEBUG_FORMATTING
*	seçeneği 1 olarak ayarlanmalıdır.
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
*	Proje konusu gereği, bir filozofun durumunu düz metin olarak yazdırır.
*
*		timestamp_in_ms X status
*/
static void	print_status(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
}

/* write_status:
*	Simülasyon hala aktif olduğu sürece bir filozofun durumunu yazdırır.
*	Farklı iş parçacıklarından gelen karışık mesajları önlemek için yazma mutex'ini kilitler.
*
*	Eğer philo.h dosyasında DEBUG_FORMATTING 1 olarak ayarlanmışsa,
*	durum hata ayıklamaya yardımcı olması için renkler
*	ve ekstra bilgilerle biçimlendirilecektir.
*	Aksi takdirde, çıktı proje konusunda istenen normal formatta olacaktır.
*/
void	write_status(t_philo *philo, bool reaper_report, t_status status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (has_simulation_stopped(philo->table) == true && reaper_report == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (DEBUG_FORMATTING == true)
	{
		write_status_debug(philo, status);
		pthread_mutex_unlock(&philo->table->write_lock);
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
	pthread_mutex_unlock(&philo->table->write_lock);
}

/* write_outcome:
*	Eğer yemek yeme sayısı belirtilmişse,
*	simülasyonun sonucunu yazdırır.
*	Sadece hata ayıklama amaçları için kullanılır.
*/
void	write_outcome(t_table *table)
{
	unsigned int	i;
	unsigned int	full_count;

	full_count = 0;
	i = 0;
	while (i < table->nb_philos)
	{
		if (table->philos[i]->times_ate >= (unsigned int)table->must_eat_count)
			full_count++;
		i++;
	}
	pthread_mutex_lock(&table->write_lock);
	printf("%d/%d philosophers had at least %d meals.\n",
		full_count, table->nb_philos, table->must_eat_count);
	pthread_mutex_unlock(&table->write_lock);
	return ;
}
