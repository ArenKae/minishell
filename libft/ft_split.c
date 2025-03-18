/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.f>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 09:39:03 by keschouf          #+#    #+#             */
/*   Updated: 2023/06/07 08:08:50 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_charset(char c, char d)
{
	if (c == d)
		return (1);
	return (0);
}

int	count_words(char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (check_charset(s[i], c) == 1 && s[i] != '\0')
			i++;
		if (check_charset(s[i], c) == 0 && s[i] != '\0')
		{
			j++;
			while (check_charset(s[i], c) == 0 && s[i] != '\0')
				i++;
		}
	}
	return (j);
}

char	*ft_malloc_and_copy(char const *s, int start, int end)
{
	char	*tab;
	int		i;

	tab = malloc(sizeof(char) * (end - start) + 1);
	if (!tab)
		return (NULL);
	i = 0;
	while (start < end)
	{
		tab[i] = s[start];
		start++;
		i++;
	}
	tab[i] = '\0';
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		j;
	int		i;
	int		k;

	k = -1;
	tab = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		while (check_charset(s[i], c) == 1 && s[i] != '\0')
			i++;
		if (check_charset(s[i], c) == 0 && s[i] != '\0')
		{
			j = i;
			while (check_charset(s[j], c) == 0 && s[j] != '\0')
				j++;
			tab[++k] = ft_malloc_and_copy(s, i, j);
			i = j;
		}
	}
	tab[++k] = NULL;
	return (tab);
}
