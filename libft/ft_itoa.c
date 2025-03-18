/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:36:27 by kschouft          #+#    #+#             */
/*   Updated: 2023/04/21 17:09:24 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_size(int n)
{
	int		count;
	long	nb;

	nb = n;
	count = 1;
	if (nb < 0)
	{
		nb = -nb;
		count++;
	}
	while (nb > 9)
	{
		count++;
		nb = nb / 10;
	}
	return (count);
}

char	*ft_itoa(int nb)
{
	char	*str;
	long	nbr;
	int		i;

	i = 0;
	nbr = nb;
	str = (char *)malloc(sizeof(char) * count_size(nb) + 1);
	if (!str)
		return (NULL);
	if (nb < 0)
		str[0] = '-';
	if (nbr < 0)
		nbr = -nbr;
	str[count_size(nb) - i++] = '\0';
	while (nbr > 9)
	{
		str[count_size(nb) - i++] = (nbr % 10) + '0';
		nbr = nbr / 10;
	}
	str[count_size(nb) - i++] = (nbr % 10) + '0';
	return (str);
}
