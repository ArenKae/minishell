/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:41:10 by kschouft          #+#    #+#             */
/*   Updated: 2023/04/21 16:56:09 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	size_t		len;
	size_t		i;

	len = ft_strlen(s2);
	if (*s2 == '\0')
		return ((char *)s1);
	if (n == 0 && len > n)
		return (NULL);
	if (len == 0)
		return ((char *)s1);
	i = 0;
	while (*s1 && i <= (n - len))
	{
		if (ft_strncmp(s1, s2, len) == 0)
			return ((char *)s1);
		s1++;
		i++;
	}
	return (NULL);
}
