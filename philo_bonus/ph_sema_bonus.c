/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_sema_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:16:31 by myagiz            #+#    #+#             */
/*   Updated: 2024/02/15 17:16:33 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ph_bonus.h"

void	make_sem_file(t_data *data)
{
	int	i;

	i = 0;
	while (++i)
	{
		data->sem_name = ph_name_make("sem", i);
		data->sem = sem_open(data->sem_name,
				O_CREAT | O_EXCL, 0644, data->num_philo);
		if (data->sem != SEM_FAILED)
			break ;
		free(data->sem_name);
	}
	sem_unlink(data->sem_name);
	free(data->sem_name);
}

void	make_print_sem_file(t_data *data)
{
	int	i;

	i = 0;
	while (++i)
	{
		data->sem_name = ph_name_make("pri", i);
		data->print_sem = sem_open(data->sem_name,
				O_CREAT | O_EXCL, 0644, 1);
		if (data->print_sem != SEM_FAILED)
			break ;
		free(data->sem_name);
	}
	sem_unlink(data->sem_name);
	free(data->sem_name);
}

void	make_must_sem_file(t_data *data)
{
	int	i;

	i = 0;
	while (++i)
	{
		data->sem_name = ph_name_make("mus", i);
		data->must_sem = sem_open(data->sem_name,
				O_CREAT | O_EXCL, 0644, data->num_philo);
		if (data->must_sem != SEM_FAILED)
			break ;
		free(data->sem_name);
	}
	sem_unlink(data->sem_name);
	free(data->sem_name);
}

void	make_eat_sem_file(t_data *data)
{
	int	i;

	i = 0;
	while (++i)
	{
		data->sem_name = ph_name_make("eat", i);
		data->eat_sem = sem_open(data->sem_name,
				O_CREAT | O_EXCL, 0644, 1);
		if (data->sem != SEM_FAILED)
			break ;
		free(data->sem_name);
	}
	sem_unlink(data->sem_name);
	free(data->sem_name);
}

char	*ph_name_make(char *base, int i)
{
	char	*name;
	int		j;

	name = (char *)malloc(sizeof(char) * ((i * 3) + 1));
	if (name == NULL)
		error_print(1);
	j = 0;
	while (j < i)
	{
		name[(j * 3)] = base[0];
		name[(j * 3) + 1] = base[1];
		name[(j * 3) + 2] = base[2];
		j++;
	}
	name[(j * 3)] = 0;
	return (name);
}
