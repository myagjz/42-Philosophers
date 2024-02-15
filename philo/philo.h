/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:15:27 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:15:32 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>

typedef struct timeval	t_te;
typedef pthread_mutex_t	t_mutex;
typedef pthread_t		t_thre;

typedef struct s_time
{
	int	die;
	int	eat;
	int	sleep;
}	t_time;

typedef struct s_share
{
	int			total;
	int			count;
	int			die;
	int			print;
	t_time		t;
	t_mutex		die_mutex;
	t_mutex		*mutex;
	long long	start_time;
	t_mutex		start_mutex;
	t_mutex		print_mutex;
}	t_share;

typedef struct s_data
{
	int			sit;
	int			must_eat;
	long long	last_eat;
	t_mutex		eat_mutex;
	t_share		*share;
}	t_data;

/* philo */
int			ph_atoi(char *str);

/* philo_make */
int			make_philo(int ac, char **av, int num_philo, t_data *data);
int			share_init(t_share *share, int num_philo, char **av);
int			data_init(t_data *data, int i, int ac, char **av);

/* philo_ending */
int			end_philo(t_data *data, t_thre *tid);
void		start_monitor(t_data *data);
int			clean_all(t_data *data, int j, t_thre *tid, int total);

/* philo_sitting */
void		*sit_one(void *tmp);
void		*sit_table(void *tmp);
void		start_eating(t_data *data, int right, int left);
void		ph_mutex_lock(t_data *data, int right, int left);
void		ph_fork_and_eat(t_data *data, int right, int left);
void		start_sit(int *right, int *left, t_data *data);

/* philo_utils */
void		sit_one_util(t_data *data);
long long	time_stamp(void);
int			error_print(void);
int			error_free(t_thre *tid, t_share *share);
int			ft_usleep(int time, t_data *data);

/* philo_print */
void		print_eat(t_data *data);
void		print_sleep(t_data *data);
void		print_think(t_data *data);
void		print_fork(t_data *data);

#endif
