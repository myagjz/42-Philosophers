/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 14:29:08 by myagiz            #+#    #+#             */
/*   Updated: 2023/06/03 17:05:00 by myagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* contains_only_digits:
*	Bir dizenin sadece 0 - 9 arasındaki rakamlardan oluşup oluşmadığını kontrol eder.
*	Eğer dize sadece rakamlardan oluşuyorsa, true döndürür.
*	Eğer dize rakam olmayan bir karakter içeriyorsa, false döndürür.
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
*	Sadece rakamlardan oluşan bir diziyi pozitif bir tamsayıya dönüştürür.
*	Dönüştürülen sayıyı 0 ile INT MAX arasında döndürür.
*	Dönüştürülen sayı INT MAX'i aşıyorsa, -1 döndürür.
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
*	Tüm gerekli argümanların geçerli olup olmadığını kontrol eder;
*	yani sadece rakamlardan oluşan ve INT MAX'i aşmayan bir dizedir.
*	Ayrıca filozof sayısının geçerli olup olmadığını da kontrol eder
*	(1 ile MAX_PHILOS arasında olmalıdır).
*	Tüm argümanlar geçerliyse true, bir tanesi geçerli değilse false döndürür.
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
