/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:37:40 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 18:20:39 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* has_simulation_stopped:
*	Simülasyonun sona erip ermediğini kontrol eder.
*	Simülasyon durması gerekiyorsa true değerini,
*	durması gerekmeyorsa false değerini döndürür.
*/
bool	has_simulation_stopped(t_table *table)
{
	bool	ret;

	sem_wait(table->sem_stop);
	ret = table->stop_sim;
	sem_post(table->sem_stop);
	return (ret);
}

/* start_simulation:
*	Her bir filozof için yeni bir çocuk süreç oluşturarak simülasyonu başlatır.
*	Süreç kimlikleri sırayla beklemek ve gerektiğinde her birine bir
*	öldürme sinyali göndermek için kaydedilir. Ayrıca filozofları
*	izlemek ve herkesin yeterince yemek yiyip yemediğini tespit etmek için
*	bir ölüm meleği iş parçacığı da oluşturur.
*
*	Simülasyon başarıyla başlatıldıysa true değerini,
*	bir hata oluştuysa false değerini döndürür.
*/
static bool	start_simulation(t_table *table)
{
	unsigned int	i;
	pid_t			pid;

	table->start_time = get_time_in_ms() + ((table->nb_philos * 2) * 10);
	i = -1;
	while (++i < table->nb_philos)
	{
		pid = fork();
		if (pid == -1)
			return (error_failure(STR_ERR_FORK, NULL, table));
		else if (pid > 0)
			table->pids[i] = pid;
		else if (pid == 0)
		{
			table->this_philo = table->philos[i];
			philosopher(table);
		}
	}
	if (start_grim_reaper_threads(table) == false)
		return (false);
	return (true);
}

/* get_child_philo:
*	Bir filozof sürecinin çıkmasını bekler.
*	Eğer filozof süreci bir hata veya ölü bir filozof ile çıkarsa,
*	diğer tüm çocuk süreçlere öldürme sinyali gönderir.
*/
static int	get_child_philo(t_table *table, pid_t *pid)
{
	int	philo_exit_code;
	int	exit_code;

	if (*pid && waitpid(*pid, &philo_exit_code, WNOHANG) != 0)
	{
		if (WIFEXITED(philo_exit_code))
		{
			exit_code = WEXITSTATUS(philo_exit_code);
			if (exit_code == CHILD_EXIT_PHILO_DEAD)
				return (kill_all_philos(table, 1));
			if (exit_code == CHILD_EXIT_ERR_PTHREAD
				|| exit_code == CHILD_EXIT_ERR_SEM)
				return (kill_all_philos(table, -1));
			if (exit_code == CHILD_EXIT_PHILO_FULL)
			{
				table->philo_full_count += 1;
				return (1);
			}
		}
	}
	return (0);
}

/* stop_simulation:
*	Simülasyon hala devam ederken,
*	her bir çocuk sürecin çıkmasını bekler ve çıkış kodunu analiz eder.
*	Aşağıdaki sonlandırma koşullarından biri gerçekleştiğinde simülasyonu sonlandırır:
*
*	bir filozof öldüğünde veya tüm filozoflar yeterli miktarda yemek yediklerinde.
*/
static int	stop_simulation(t_table	*table)
{
	unsigned int	i;
	int				exit_code;

	sim_start_delay(table->start_time);
	while (has_simulation_stopped(table) == false)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			exit_code = get_child_philo(table, &table->pids[i]);
			if (exit_code == 1 || exit_code == -1)
			{
				sem_wait(table->sem_stop);
				table->stop_sim = true;
				sem_post(table->sem_philo_full);
				sem_post(table->sem_philo_dead);
				sem_post(table->sem_stop);
				return (exit_code);
			}
			i++;
		}
	}
	return (0);
}

/* main:
*	Ana filozoflar fonksiyonu.
*/
int	main(int ac, char **av)
{
	t_table	*table;

	table = NULL;
	if (ac - 1 < 4 || ac - 1 > 5)
		return (msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!is_valid_input(ac, av))
		return (EXIT_FAILURE);
	table = init_table(ac, av, 1);
	if (!table)
		return (EXIT_FAILURE);
	if (!start_simulation(table))
		return (EXIT_FAILURE);
	if (stop_simulation(table) == -1)
		return (table_cleanup(table, EXIT_FAILURE));
	if (DEBUG_FORMATTING == 1 && table->must_eat_count >= 0)
		write_outcome(table);
	return (table_cleanup(table, EXIT_SUCCESS));
}
