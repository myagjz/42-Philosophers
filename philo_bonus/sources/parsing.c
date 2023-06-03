/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:38:08 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 18:28:25 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* contains_only_digits:
*	Verilen bir dizginin sadece 0 - 9 rakamlarını içerip içermediğini kontrol eder.
*	Dizgi sadece rakamları içeriyorsa true değerini döndürür.
*	Dizgi bir rakam olmayan bir karakter içeriyorsa false değerini döndürür.
*/
static bool	contains_only_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

/* integer_atoi:
*	Sadece rakamlardan oluşan bir dizgiyi pozitif bir tamsayıya dönüştürür.
*	Dönüştürülen sayıyı 0 ile INT_MAX (tam sayı sınırlarında) arasında döndürür.
*	Dönüştürülen sayı INT_MAX'i aşarsa -1 değerini döndürür.
*/
int	integer_atoi(char *str)
{
	unsigned long long int	nb;
	int						i;

	i = 0;
	nb = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (nb > INT_MAX)
		return (-1);
	return ((int)nb);
}

/* is_valid_input:
*	Tüm gereken argümanların geçerli olup olmadığını kontrol eder.
*	Yani, yalnızca rakamlardan oluşan ve INT_MAX'i aşmayan bir dizedir.
*	Ayrıca filozof sayısının geçerli olup olmadığını da kontrol eder
*	(1 ile MAX_PHILOS arasında olmalıdır).
*	Tüm argümanlar geçerli ise true değerini,
*	bunlardan biri geçerli değilse false değerini döndürür.
*/
bool	is_valid_input(int ac, char **av)
{
	int	i;
	int	nb;

	i = 1;
	while (i < ac)
	{
		if (!contains_only_digits(av[i]))
			return (msg(STR_ERR_INPUT_DIGIT, av[i], false));
		nb = integer_atoi(av[i]);
		if (i == 1 && (nb <= 0 || nb > MAX_PHILOS))
			return (msg(STR_ERR_INPUT_POFLOW, STR_MAX_PHILOS, false));
		if (i != 1 && nb == -1)
			return (msg(STR_ERR_INPUT_DIGIT, av[i], false));
		i++;
	}
	return (true);
}
