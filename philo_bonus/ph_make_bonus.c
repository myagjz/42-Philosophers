/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_make_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:16:11 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:16:13 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ph_bonus.h"

void	make_philo(t_data *data)
{
	int		i;
	pid_t	*pid;

	pid = (pid_t *)malloc(sizeof(pid_t) * data->num_philo);
	if (pid == 0)
		error_print(1);
	i = 0;
	data->start_time = time_stamp() + data->num_philo * 3;
	data->last_eat = time_stamp() + data->num_philo * 3;
	while (i < data->num_philo)
	{
		pid[i] = fork();
		if (pid[i] == -1)
			exit(1);
		else if (pid[i] == 0)
		{
			data->sit = i;
			make_child(data);
			exit(0);
		}
		i++;
	}
	wait_children(data, pid);
}

void	make_child(t_data *data)
{
	t_thre	tid;

	sem_wait(data->must_sem);
	start_setting(data);
	pthread_create(&tid, 0, monitoring, data);
	if (data->sit % 2 == 1)
		ph_usleep(data->t.eat / 2);
	while (42)
	{
		ph_eat_and_fork(data);
		print_sleep(data);
		print_think(data);
	}
}

void	ph_eat_and_fork(t_data *data)
{
	sem_wait(data->sem);
	print_fork(data);
	sem_wait(data->sem);
	print_fork(data);
	sem_wait(data->eat_sem);
	data->last_eat = time_stamp();
	sem_post(data->eat_sem);
	print_eat(data);
	sem_wait(data->eat_sem);
	if (data->must_eat > 0)
		data->must_eat--;
	sem_post(data->eat_sem);
	sem_post(data->sem);
	sem_post(data->sem);
}

void	wait_children(t_data *data, pid_t *pid)
{
	int		i;
	pid_t	end_pid;
	int		status;
	t_thre	tid;

	data->pid = pid;
	pthread_create(&tid, 0, must_monitor, data);
	end_pid = waitpid(-1, &status, WUNTRACED);
	if (WIFSIGNALED(status) != 0)
		return ;
	i = 0;
	while (i < data->num_philo)
	{
		if (end_pid != pid[i])
			kill(pid[i], SIGKILL);
		else if (end_pid == pid[i])
			printf("\033[0;31m%lld %d died\n",
				time_stamp() - data->start_time, i + 1);
		i++;
	}
	free(pid);
	exit(0);
}
